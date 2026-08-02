#include "SweepConfig.h"

#include <cctype>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

namespace {
	G4String Trim(const G4String& value)
	{
		size_t begin = 0;
		size_t end = value.size();
		while (begin < end && std::isspace(static_cast<unsigned char>(value[begin]))) {
			++begin;
		}
		while (end > begin && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
			--end;
		}
		return value.substr(begin, end - begin);
	}

	std::map<G4String, G4String> ParseKeyValueFile(const G4String& path)
	{
		std::ifstream in(path);
		if (!in) {
			throw std::runtime_error(
				"Could not open sweep config file \"" + std::string(path) +
				"\". Pass a path as the first command-line argument, or create "
				"a sweep_config.txt next to the executable (see the sample "
				"shipped with this project).");
		}

		std::map<G4String, G4String> values;
		std::string line;
		int lineNumber = 0;
		while (std::getline(in, line)) {
			++lineNumber;
			const G4String trimmed = Trim(line);
			if (trimmed.empty() || trimmed[0] == '#') {
				continue;
			}

			const size_t equalsPos = trimmed.find('=');
			if (equalsPos == std::string::npos) {
				throw std::runtime_error(
					"sweep config parse error at line " + std::to_string(lineNumber) +
					": expected \"key = value\", got: " + std::string(trimmed));
			}

			const G4String key = Trim(trimmed.substr(0, equalsPos));
			const G4String value = Trim(trimmed.substr(equalsPos + 1));
			values[key] = value;
		}

		return values;
	}

	G4double ParseDouble(const std::map<G4String, G4String>& values, const G4String& key, G4double defaultValue)
	{
		const auto it = values.find(key);
		if (it == values.end()) {
			return defaultValue;
		}
		try {
			return std::stod(std::string(it->second));
		} catch (const std::exception&) {
			throw std::runtime_error("sweep config: \"" + std::string(key) + "\" is not a valid number: " + std::string(it->second));
		}
	}

	G4int ParseInt(const std::map<G4String, G4String>& values, const G4String& key, G4int defaultValue)
	{
		const auto it = values.find(key);
		if (it == values.end()) {
			return defaultValue;
		}
		try {
			return std::stoi(std::string(it->second));
		} catch (const std::exception&) {
			throw std::runtime_error("sweep config: \"" + std::string(key) + "\" is not a valid integer: " + std::string(it->second));
		}
	}

	G4String ParseString(const std::map<G4String, G4String>& values, const G4String& key, const G4String& defaultValue)
	{
		const auto it = values.find(key);
		if (it == values.end()) {
			return defaultValue;
		}
		return it->second;
	}

	std::vector<G4String> ParseMaterialList(const std::map<G4String, G4String>& values)
	{
		const auto it = values.find("materials");
		if (it == values.end() || Trim(it->second).empty()) {
			throw std::runtime_error("sweep config: \"materials\" is required, e.g. materials = TPU, HDPE");
		}

		std::vector<G4String> result;
		std::stringstream stream{std::string(it->second)};
		std::string token;
		while (std::getline(stream, token, ',')) {
			const G4String name = Trim(token);
			if (!name.empty()) {
				result.push_back(name);
			}
		}

		if (result.empty()) {
			throw std::runtime_error("sweep config: \"materials\" did not contain any material names.");
		}

		return result;
	}
}

std::vector<G4double> SweepConfig::ThicknessValuesCm() const
{
	std::vector<G4double> values;
	if (thicknessStepCm <= 0.0) {
		throw std::runtime_error("sweep config: thickness_step_cm must be > 0.");
	}

	for (G4double t = thicknessMinCm; t <= thicknessMaxCm + 1.0e-9; t += thicknessStepCm) {
		values.push_back(t);
	}
	return values;
}

std::vector<G4double> SweepConfig::EnergyValuesMeV() const
{
	std::vector<G4double> values;
	if (energyStepMeV <= 0.0) {
		throw std::runtime_error("sweep config: energy_step_MeV must be > 0.");
	}

	for (G4double e = energyMinMeV; e <= energyMaxMeV + 1.0e-9; e += energyStepMeV) {
		values.push_back(e);
	}
	return values;
}

SweepConfig LoadSweepConfig(const G4String& path)
{
	const auto values = ParseKeyValueFile(path);

	SweepConfig config;
	config.materials = ParseMaterialList(values);
	config.thicknessMinCm = ParseDouble(values, "thickness_min_cm", config.thicknessMinCm);
	config.thicknessMaxCm = ParseDouble(values, "thickness_max_cm", config.thicknessMaxCm);
	config.thicknessStepCm = ParseDouble(values, "thickness_step_cm", config.thicknessStepCm);
	config.energyMinMeV = ParseDouble(values, "energy_min_MeV", config.energyMinMeV);
	config.energyMaxMeV = ParseDouble(values, "energy_max_MeV", config.energyMaxMeV);
	config.energyStepMeV = ParseDouble(values, "energy_step_MeV", config.energyStepMeV);
	config.eventsPerPoint = ParseInt(values, "events_per_point", config.eventsPerPoint);
	config.randomSeed = ParseInt(values, "random_seed", static_cast<G4int>(config.randomSeed));
	config.outputCsvFilename = ParseString(values, "output_csv", config.outputCsvFilename);

	return config;
}
