#ifndef SWEEP_CONFIG_H
#define SWEEP_CONFIG_H

#include "G4String.hh"
#include "G4Types.hh"

#include <vector>

// ---------------------------------------------------------------------------
// SweepConfig
//
// Everything a student needs to change to run a new sweep -- which
// material(s), what thicknesses, what proton energies -- is read from a
// plain-text config file at runtime (default: sweep_config.txt next to the
// executable). Editing that file never requires rebuilding the program.
//
// Only the material's ELEMENTAL COMPOSITION (see MaterialLibrary.h) needs a
// rebuild, since that's a one-line addition to a C++ array rather than a
// simulation-logic change.
// ---------------------------------------------------------------------------
struct SweepConfig {
	std::vector<G4String> materials;

	G4double thicknessMinCm = 1.0;
	G4double thicknessMaxCm = 10.0;
	G4double thicknessStepCm = 1.0;

	G4double energyMinMeV = 10.0;
	G4double energyMaxMeV = 200.0;
	G4double energyStepMeV = 10.0;

	G4int eventsPerPoint = 100000;
	long randomSeed = 12345;
	G4String outputCsvFilename = "../data/shielding_results.csv";

	std::vector<G4double> ThicknessValuesCm() const;
	std::vector<G4double> EnergyValuesMeV() const;
};

// Reads a key = value config file. Blank lines and lines starting with '#'
// are ignored. See sweep_config.txt for the format and a full example.
// Throws std::runtime_error with a clear message if the file can't be opened
// or a required key is missing/malformed.
SweepConfig LoadSweepConfig(const G4String& path);

#endif
