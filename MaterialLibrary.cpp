#include "MaterialLibrary.h"

#include "G4SystemOfUnits.hh"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
	// Appends "<symbol><percent>" to `out` if percent > 0, trimming a
	// trailing ".0" so whole-number percentages print as e.g. "C86" not
	// "C86.0".
	void AppendElement(std::ostringstream& out, const char* symbol, G4double percent)
	{
		if (percent <= 0.0) {
			return;
		}

		std::ostringstream num;
		num << std::fixed << std::setprecision(1) << percent;
		std::string text = num.str();
		if (text.size() >= 2 && text[text.size() - 1] == '0' && text[text.size() - 2] == '.') {
			text = text.substr(0, text.size() - 2);
		}

		out << symbol << text;
	}

	const MaterialDefinition* FindDefinition(const G4String& materialName)
	{
		for (const auto& def : MaterialLibrary()) {
			if (CompositionName(def) == materialName) {
				return &def;
			}
		}
		return nullptr;
	}

	G4String AvailableNamesList()
	{
		std::ostringstream out;
		bool first = true;
		for (const auto& def : MaterialLibrary()) {
			if (!first) {
				out << ", ";
			}
			out << CompositionName(def);
			first = false;
		}
		return out.str();
	}

	G4int CountNonzeroElements(const MaterialDefinition& def)
	{
		G4int count = 0;
		if (def.percentC > 0.0) ++count;
		if (def.percentH > 0.0) ++count;
		if (def.percentO > 0.0) ++count;
		if (def.percentN > 0.0) ++count;
		if (def.percentSi > 0.0) ++count;
		return count;
	}
}

G4String CompositionName(const MaterialDefinition& def)
{
	// Hill notation: Carbon first, Hydrogen second, everything else
	// alphabetical (N, O, Si).
	std::ostringstream out;
	AppendElement(out, "C", def.percentC);
	AppendElement(out, "H", def.percentH);
	AppendElement(out, "N", def.percentN);
	AppendElement(out, "O", def.percentO);
	AppendElement(out, "Si", def.percentSi);
	return out.str();
}

void ValidateMaterialLibrary()
{
	const auto& library = MaterialLibrary();
	for (size_t i = 0; i < library.size(); ++i) {
		for (size_t j = i + 1; j < library.size(); ++j) {
			if (CompositionName(library[i]) == CompositionName(library[j])) {
				throw std::runtime_error(
					"MaterialLibrary.h has two entries with the same composition (\"" +
					std::string(CompositionName(library[i])) +
					"\"). Give them different percentages, or remove the duplicate.");
			}
		}
	}
}

void PrintMaterialLibrary()
{
	std::cout << "Materials available in MaterialLibrary.h:\n";
	std::cout << "  " << std::left << std::setw(28) << "name (use this in config)" << "density (g/cm3)\n";
	for (const auto& def : MaterialLibrary()) {
		std::cout << "  " << std::left << std::setw(28) << CompositionName(def) << def.density_g_cm3 << '\n';
	}
}

G4Material* BuildLibraryMaterial(G4NistManager* nist, const G4String& materialName)
{
	// Already built earlier in this run (e.g. reused across sweep points)?
	G4Material* existing = G4Material::GetMaterial(materialName, false);
	if (existing != nullptr) {
		return existing;
	}

	const MaterialDefinition* def = FindDefinition(materialName);
	if (def == nullptr) {
		throw std::runtime_error(
			"Unknown shield material \"" + std::string(materialName) +
			"\". Available materials (run ./sim --list-materials to see this "
			"list any time): " + std::string(AvailableNamesList()));
	}

	const G4double total = def->percentC + def->percentH + def->percentO + def->percentN + def->percentSi;
	if (total <= 0.0) {
		throw std::runtime_error(
			"Material \"" + std::string(materialName) + "\" has no nonzero elemental percentages set.");
	}

	const G4int nComponents = CountNonzeroElements(*def);
	auto* material = new G4Material(CompositionName(*def), def->density_g_cm3 * g / cm3, nComponents);

	// Fractions are renormalized by `total` so entries that don't sum to
	// exactly 100 still work; Geant4 requires the mass fractions passed to
	// AddElement to sum to 1.0.
	if (def->percentC > 0.0) {
		material->AddElement(nist->FindOrBuildElement("C"), def->percentC / total);
	}
	if (def->percentH > 0.0) {
		material->AddElement(nist->FindOrBuildElement("H"), def->percentH / total);
	}
	if (def->percentO > 0.0) {
		material->AddElement(nist->FindOrBuildElement("O"), def->percentO / total);
	}
	if (def->percentN > 0.0) {
		material->AddElement(nist->FindOrBuildElement("N"), def->percentN / total);
	}
	if (def->percentSi > 0.0) {
		material->AddElement(nist->FindOrBuildElement("Si"), def->percentSi / total);
	}

	return material;
}
