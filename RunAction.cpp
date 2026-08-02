#include "RunAction.h"

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

#include <utility>

RunAction* RunAction::fInstance = nullptr;

RunAction::RunAction(std::shared_ptr<SimulationConfig> config)
	: fConfig(std::move(config))
{
	fInstance = this;
}

RunAction* RunAction::Instance()
{
	return fInstance;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
	fRecords.clear();
	fEnergyByVolume.clear();
	fMassByVolume.clear();
}

void RunAction::SetOutputCsvFilename(const G4String& filename)
{
	fConfig->run.outputCsvFilename = filename;
}

void RunAction::SetVolumeMass(const G4String& volumeName, G4double mass)
{
	if (mass > 0.0) {
		fMassByVolume[volumeName] = mass;
	}
}

void RunAction::RecordDose(G4int eventID, const G4String& volumeName, G4double energyDeposited, G4double mass)
{
	if (energyDeposited <= 0.0 || mass <= 0.0) {
		return;
	}

	const G4double dose = energyDeposited / mass;

	fRecords.push_back({eventID, volumeName, energyDeposited, mass, dose});
	fEnergyByVolume[volumeName] += energyDeposited;
	fMassByVolume[volumeName] = mass;
}

void RunAction::EndOfRunAction(const G4Run*)
{
}

RunSummary RunAction::GetRunSummary() const
{
	RunSummary summary;

	const auto organEnergy = fEnergyByVolume.find("organ");
	if (organEnergy != fEnergyByVolume.end()) {
		summary.organEnergyDeposited = organEnergy->second;
	}

	const auto organMass = fMassByVolume.find("organ");
	if (organMass != fMassByVolume.end()) {
		summary.organMass = organMass->second;
	}

	if (summary.organMass > 0.0) {
		summary.organDose = summary.organEnergyDeposited / summary.organMass;
	}

	for (const auto& record : fRecords) {
		if (record.volumeName == "organ") {
			++summary.scoredOrganEvents;
		}
	}

	return summary;
}
