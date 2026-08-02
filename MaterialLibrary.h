#ifndef MATERIAL_LIBRARY_H
#define MATERIAL_LIBRARY_H

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4String.hh"
#include "G4Types.hh"

#include <vector>

// ---------------------------------------------------------------------------
// MaterialDefinition
//
// One shield material = one row: a density plus elemental mass percentages.
// There is deliberately no "name" field -- the material's identifier is
// generated FROM the composition (see CompositionName below), so a material
// can never end up mislabeled relative to what it's actually made of.
//
// Percentages are percent BY MASS and don't need to add up to exactly 100
// (they get renormalized), but they should be close to 100 or your density
// won't mean what you think it means. Leave an element at 0.0 if the
// material doesn't contain it.
// ---------------------------------------------------------------------------
struct MaterialDefinition {
	G4double density_g_cm3;
	G4double percentC = 0.0;
	G4double percentH = 0.0;
	G4double percentO = 0.0;
	G4double percentN = 0.0;
	G4double percentSi = 0.0;
};

// ---------------------------------------------------------------------------
// THE MATERIAL LIBRARY
//
// This is the ONLY place you need to touch to test a new shield material.
// Add a row below with the material's density and elemental mass
// percentages, save, rebuild (one command, see README_STUDENTS.md).
//
// Run `./sim --list-materials` after rebuilding to see the generated name
// for your new material -- that's the string you put in sweep_config.txt.
//
// You do NOT need to touch construction.cpp, sim.cpp, or anything else.
// ---------------------------------------------------------------------------
inline const std::vector<MaterialDefinition>& MaterialLibrary()
{
	static const std::vector<MaterialDefinition> library = {
		// density(g/cm3)   C%     H%     O%     N%     Si%
		{0.95,               85.7,  14.3,  0.0,   0.0,   0.0},   // HDPE-like
		{1.42,               69.1,  2.6,   20.6,  7.7,   0.0},   // Polyimide-like
		{0.97,               32.4,  8.1,   21.6,  0.0,   37.9},  // PDMS-like
		{1.20,               61.3,  11.9,  16.2,  10.6,  0.0},   // TPU-like

		// Add your own material here, e.g.:
		// {1.10,            50.0,  10.0,  30.0,  10.0,  0.0},
	};
	return library;
}

// Generates a material's identifier from its composition, e.g. a material
// that's 61.3% C / 11.9% H / 10.6% N / 16.2% O becomes "C61.3H11.9N10.6O16.2"
// (element order follows Hill notation: C, H, then the rest alphabetically).
// Zero-percent elements are omitted. This string is what you use in
// sweep_config.txt's "materials =" line, and is also the underlying
// G4Material's name.
G4String CompositionName(const MaterialDefinition& def);

// Checks the library for two entries that would generate the same
// CompositionName (i.e. accidental duplicate/near-duplicate rows). Throws
// std::runtime_error listing the collision if one is found. Call this once
// at startup.
void ValidateMaterialLibrary();

// Prints every material in the library with its generated name and density,
// for `./sim --list-materials`.
void PrintMaterialLibrary();

// Builds (or returns the cached) G4Material for a composition name generated
// by CompositionName above. Throws std::runtime_error with a helpful message
// (including the list of valid names) if the name doesn't match any library
// entry.
G4Material* BuildLibraryMaterial(G4NistManager* nist, const G4String& materialName);

#endif
