#include "construction.h"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "MaterialLibrary.h"
#include "TissueSensitiveDetector.h"

#include <utility>

namespace {
	void AssignRegion(const G4String& regionName, G4LogicalVolume* logicalVolume)
	{
		auto* region = G4RegionStore::GetInstance()->FindOrCreateRegion(regionName);
		while (region->GetNumberOfRootVolumes() > 0) {
			auto root = region->GetRootLogicalVolumeIterator();
			region->RemoveRootLogicalVolume(*root, false);
		}

		region->ClearMap();
		region->AddRootLogicalVolume(logicalVolume);
	}
}

DetectorConstruction::DetectorConstruction(std::shared_ptr<SimulationConfig> config)
	: fConfig(std::move(config)),
	  fOrganLogical(nullptr),
	  fGeometryConstructed(false)
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	fOrganLogical = nullptr;

	G4NistManager* nist = G4NistManager::Instance();
	G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material* shieldMaterial = BuildLibraryMaterial(nist, fConfig->shield.material);
	G4Material* tissueMaterial = nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP");

	const G4double organHalfX = 0.5 * fConfig->detector.width;
	const G4double organHalfY = 0.5 * fConfig->detector.height;
	const G4double organHalfZ = 0.5 * fConfig->detector.thickness;

	G4Box* solidWorld = new G4Box("solidWorld", 1 * m, 1 * m, 1 * m);
	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMaterial, "logicWorld");
	G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "physWorld", nullptr, false, 0, true);

	G4Box* shieldSolid = new G4Box("shieldSolid",
	                               0.5 * fConfig->shield.width,
	                               0.5 * fConfig->shield.height,
	                               0.5 * fConfig->shield.thickness);
	G4LogicalVolume* shieldLogical = new G4LogicalVolume(shieldSolid, shieldMaterial, "shield");

	const G4double organFrontZ = fConfig->detector.position.z() - organHalfZ;
	const G4double shieldCenterZ = organFrontZ - fConfig->shield.distanceToDetector - 0.5 * fConfig->shield.thickness;
	new G4PVPlacement(nullptr,
	                  G4ThreeVector(fConfig->detector.position.x(), fConfig->detector.position.y(), shieldCenterZ),
	                  shieldLogical,
	                  "shield",
	                  logicWorld,
	                  false,
	                  0,
	                  true);

	// Detector is a single block of soft tissue -- no skin layer.
	G4Box* organSolid = new G4Box("organSolid", organHalfX, organHalfY, organHalfZ);
	fOrganLogical = new G4LogicalVolume(organSolid, tissueMaterial, "organ");
	new G4PVPlacement(nullptr,
	                  fConfig->detector.position,
	                  fOrganLogical,
	                  "organ",
	                  logicWorld,
	                  false,
	                  0,
	                  true);

	AssignRegion("organRegion", fOrganLogical);

	auto shieldVis = new G4VisAttributes(G4Colour(0.70, 0.70, 0.75, 0.45));
	shieldVis->SetForceSolid(true);
	shieldLogical->SetVisAttributes(shieldVis);

	auto organVis = new G4VisAttributes(G4Colour(0.85, 0.15, 0.18, 0.65));
	organVis->SetForceSolid(true);
	fOrganLogical->SetVisAttributes(organVis);

	fGeometryConstructed = true;
	return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
	if (fOrganLogical == nullptr) {
		return;
	}

	auto sdManager = G4SDManager::GetSDMpointer();
	auto tissueSD = dynamic_cast<TissueSensitiveDetector*>(sdManager->FindSensitiveDetector("tissueSD", false));
	if (tissueSD == nullptr) {
		tissueSD = new TissueSensitiveDetector("tissueSD");
		sdManager->AddNewDetector(tissueSD);
	}

	const G4double density = fOrganLogical->GetMaterial()->GetDensity();
	tissueSD->RegisterVolume("organ", OrganVolume() * density);

	SetSensitiveDetector(fOrganLogical, tissueSD);
}

void DetectorConstruction::SetShieldMaterial(const G4String& material)
{
	if (fConfig->shield.material == material) {
		return;
	}

	fConfig->shield.material = material;
	MarkGeometryChanged();
}

void DetectorConstruction::SetShieldThickness(G4double thickness)
{
	if (fConfig->shield.thickness == thickness) {
		return;
	}

	fConfig->shield.thickness = thickness;
	MarkGeometryChanged();
}

void DetectorConstruction::SetShieldMaterialAndThickness(const G4String& material, G4double thickness)
{
	const bool changed = fConfig->shield.material != material || fConfig->shield.thickness != thickness;
	fConfig->shield.material = material;
	fConfig->shield.thickness = thickness;

	if (changed) {
		MarkGeometryChanged();
	}
}

void DetectorConstruction::SetShieldWidth(G4double width)
{
	if (fConfig->shield.width == width) {
		return;
	}

	fConfig->shield.width = width;
	MarkGeometryChanged();
}

void DetectorConstruction::SetShieldHeight(G4double height)
{
	if (fConfig->shield.height == height) {
		return;
	}

	fConfig->shield.height = height;
	MarkGeometryChanged();
}

void DetectorConstruction::SetShieldDistanceToDetector(G4double distance)
{
	if (fConfig->shield.distanceToDetector == distance) {
		return;
	}

	fConfig->shield.distanceToDetector = distance;
	MarkGeometryChanged();
}

void DetectorConstruction::SetOrganThickness(G4double thickness)
{
	if (fConfig->detector.thickness == thickness) {
		return;
	}

	fConfig->detector.thickness = thickness;
	MarkGeometryChanged();
}

void DetectorConstruction::SetDetectorDimensions(G4double width, G4double height)
{
	if (fConfig->detector.width == width && fConfig->detector.height == height) {
		return;
	}

	fConfig->detector.width = width;
	fConfig->detector.height = height;
	MarkGeometryChanged();
}

void DetectorConstruction::SetDetectorPosition(const G4ThreeVector& position)
{
	if (fConfig->detector.position == position) {
		return;
	}

	fConfig->detector.position = position;
	MarkGeometryChanged();
}

const SimulationConfig& DetectorConstruction::GetConfig() const
{
	return *fConfig;
}

void DetectorConstruction::MarkGeometryChanged()
{
	if (!fGeometryConstructed) {
		return;
	}

	auto* runManager = G4RunManager::GetRunManager();
	if (runManager != nullptr) {
		runManager->ReinitializeGeometry();
	}
}

G4double DetectorConstruction::OrganVolume() const
{
	return fConfig->detector.width * fConfig->detector.height * fConfig->detector.thickness;
}
