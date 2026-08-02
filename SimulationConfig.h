#ifndef SIMULATION_CONFIG_H
#define SIMULATION_CONFIG_H

#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"

struct ShieldConfig {
	// Must match a "name" entry in MaterialLibrary.h.
	G4String material = "HDPE";
	G4double thickness = 1.0 * cm;
	G4double width = 16.0 * cm;
	G4double height = 16.0 * cm;
	G4double distanceToDetector = 1.0 * cm;
};

struct BeamConfig {
	G4String particleType = "proton";
	G4double energy = 100.0 * MeV;
	G4ThreeVector position = G4ThreeVector(0.0, 0.0, -20.0 * cm);
	G4ThreeVector direction = G4ThreeVector(0.0, 0.0, 1.0);
	G4double radius = 0.0;
};

// The detector is a single block of soft tissue (G4_TISSUE_SOFT_ICRP) --
// there is no separate skin layer.
struct DetectorConfig {
	G4double thickness = 10.0 * cm;
	G4double width = 10.0 * cm;
	G4double height = 10.0 * cm;
	G4ThreeVector position = G4ThreeVector(0.0, 0.0, 8.0 * cm);
};

struct RunConfig {
	G4int numberOfEvents = 100000;
	G4String outputCsvFilename = "dose_results.csv";
	long randomSeed = 12345;
};

struct SimulationConfig {
	ShieldConfig shield;
	BeamConfig beam;
	DetectorConfig detector;
	RunConfig run;
};

#endif
