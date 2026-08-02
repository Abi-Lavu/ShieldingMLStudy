// ---------------------------------------------------------------------------
// AerospaceShieldingSim -- student edition
//
// Workflow for testing a material:
//   1. New composition/density?  Add a row to MaterialLibrary.h, rebuild,
//      then run `./sim --list-materials` to see the name it generated for
//      you (material names are derived from composition, not typed in).
//   2. New thickness/energy range, or just want to try a material that's
//      already in the library?  Edit sweep_config.txt, rerun -- no rebuild.
//
// Output is one CSV row per (material, thickness, proton energy) combination
// with the organ dose per proton, the number the material-comparison plots
// actually care about.
// ---------------------------------------------------------------------------

#include <fstream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include "QBBC.hh"

#include "action.h"
#include "construction.h"
#include "generator.h"
#include "MaterialLibrary.h"
#include "RunAction.h"
#include "SimulationConfig.h"
#include "SweepConfig.h"

namespace {
	struct SweepPoint {
		G4String shieldMaterial;
		G4double shieldThickness;
		G4double protonEnergy;
	};

	void ConfigureBatchVerbosity()
	{
		auto* uiManager = G4UImanager::GetUIpointer();
		uiManager->ApplyCommand("/run/verbose 0");
		uiManager->ApplyCommand("/event/verbose 0");
		uiManager->ApplyCommand("/tracking/verbose 0");
	}

	void WriteSweepHeader(std::ofstream& out)
	{
		out << "shield_material,"
		    << "shield_thickness_cm,"
		    << "proton_energy_MeV,"
		    << "number_of_primary_protons,"
		    << "organ_dose_per_proton_Gy,"
		    << "organ_energy_deposited_total_MeV,"
		    << "organ_mass_kg,"
		    << "scored_organ_events"
		    << '\n';
	}

	void WriteSweepRow(std::ofstream& out, const SweepPoint& point, const RunSummary& summary, G4int numberOfEvents)
	{
		out << point.shieldMaterial << ','
		    << point.shieldThickness / cm << ','
		    << point.protonEnergy / MeV << ','
		    << numberOfEvents << ','
		    << (summary.organDose / gray) / numberOfEvents << ','
		    << summary.organEnergyDeposited / MeV << ','
		    << summary.organMass / kg << ','
		    << summary.scoredOrganEvents
		    << '\n';
	}

	void ApplySweepPoint(DetectorConstruction* detectorConstruction, PrimaryGenerator* primaryGenerator, const SweepPoint& point)
	{
		detectorConstruction->SetShieldMaterialAndThickness(point.shieldMaterial, point.shieldThickness);
		primaryGenerator->SetParticleType("proton");
		primaryGenerator->SetBeamEnergy(point.protonEnergy);
	}

	void RunParameterSweep(G4RunManager* runManager,
	                       DetectorConstruction* detectorConstruction,
	                       PrimaryGenerator* primaryGenerator,
	                       RunAction* runAction,
	                       const SweepConfig& sweep)
	{
		std::ofstream out(sweep.outputCsvFilename);
		if (!out) {
			throw std::runtime_error("Unable to open sweep CSV output file: " + std::string(sweep.outputCsvFilename));
		}

		out << std::setprecision(10);
		WriteSweepHeader(out);

		const auto thicknesses = sweep.ThicknessValuesCm();
		const auto energies = sweep.EnergyValuesMeV();

		for (const auto& material : sweep.materials) {
			for (const auto thicknessCm : thicknesses) {
				for (const auto energyMeV : energies) {
					const SweepPoint point{material, thicknessCm * cm, energyMeV * MeV};
					ApplySweepPoint(detectorConstruction, primaryGenerator, point);
					runManager->BeamOn(sweep.eventsPerPoint);
					WriteSweepRow(out, point, runAction->GetRunSummary(), sweep.eventsPerPoint);
				}
			}
		}
	}
}

int main(int argc, char** argv)
{
	ValidateMaterialLibrary();

	if (argc > 1 && G4String(argv[1]) == "--list-materials") {
		PrintMaterialLibrary();
		return 0;
	}

	const G4String sweepConfigPath = (argc > 1) ? argv[1] : "sweep_config.txt";
	const SweepConfig sweep = LoadSweepConfig(sweepConfigPath);

	auto config = std::make_shared<SimulationConfig>();
	// Starting geometry/beam values; the sweep below overrides material,
	// thickness, and energy for each point. Adjust detector/beam geometry
	// here if you need to (e.g. detector size, beam radius).
	config->shield.material = sweep.materials.front();
	config->shield.thickness = sweep.thicknessMinCm * cm;
	config->shield.width = 16.0 * cm;
	config->shield.height = 16.0 * cm;
	config->shield.distanceToDetector = 2 * mm;

	config->beam.particleType = "proton";
	config->beam.energy = sweep.energyMinMeV * MeV;
	config->beam.position = G4ThreeVector(0.0, 0.0, -20.0 * cm);
	config->beam.direction = G4ThreeVector(0.0, 0.0, 1.0);
	config->beam.radius = 2.0 * mm;

	config->detector.thickness = 5.0 * cm;
	config->detector.width = 10.0 * cm;
	config->detector.height = 10.0 * cm;
	config->detector.position = G4ThreeVector(0.0, 0.0, 8.0 * cm);

	config->run.numberOfEvents = sweep.eventsPerPoint;
	config->run.outputCsvFilename = sweep.outputCsvFilename;
	config->run.randomSeed = sweep.randomSeed;

	G4Random::setTheSeed(config->run.randomSeed);

	auto* runManager = new G4RunManager();
	auto* detectorConstruction = new DetectorConstruction(config);

	runManager->SetUserInitialization(detectorConstruction);
	runManager->SetUserInitialization(new QBBC());
	runManager->SetUserInitialization(new ActionInitialization(config));
	runManager->SetNumberOfThreads(8);
	runManager->Initialize();
	ConfigureBatchVerbosity();

	auto* primaryGenerator = PrimaryGenerator::Instance();
	auto* runAction = RunAction::Instance();
	if (primaryGenerator == nullptr || runAction == nullptr) {
		throw std::runtime_error("Simulation actions were not initialized.");
	}

	RunParameterSweep(runManager, detectorConstruction, primaryGenerator, runAction, sweep);

	delete runManager;
	return 0;
}
