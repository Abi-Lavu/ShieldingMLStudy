#ifndef CONSTRUCTION_H
#define CONSTRUCTION_H

#include "G4LogicalVolume.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"

#include "SimulationConfig.h"

#include <memory>

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
	explicit DetectorConstruction(std::shared_ptr<SimulationConfig> config);
	~DetectorConstruction() override = default;

	G4VPhysicalVolume* Construct() override;
	void ConstructSDandField() override;

	void SetShieldMaterial(const G4String& material);
	void SetShieldThickness(G4double thickness);
	void SetShieldMaterialAndThickness(const G4String& material, G4double thickness);
	void SetShieldWidth(G4double width);
	void SetShieldHeight(G4double height);
	void SetShieldDistanceToDetector(G4double distance);

	void SetOrganThickness(G4double thickness);
	void SetDetectorDimensions(G4double width, G4double height);
	void SetDetectorPosition(const G4ThreeVector& position);

	const SimulationConfig& GetConfig() const;

private:
	void MarkGeometryChanged();
	G4double OrganVolume() const;

	std::shared_ptr<SimulationConfig> fConfig;
	G4LogicalVolume* fOrganLogical;
	bool fGeometryConstructed;
};

#endif
