#include "TissueSensitiveDetector.h"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4TouchableHandle.hh"

#include "RunAction.h"

TissueSensitiveDetector::TissueSensitiveDetector(const G4String& name)
	: G4VSensitiveDetector(name)
{}

void TissueSensitiveDetector::RegisterVolume(const G4String& volumeName, G4double mass)
{
	fMassByVolume[volumeName] = mass;
}

void TissueSensitiveDetector::Initialize(G4HCofThisEvent*)
{
	fEventEnergyByVolume.clear();

	auto* runAction = RunAction::Instance();
	if (runAction == nullptr) {
		return;
	}

	for (const auto& entry : fMassByVolume) {
		runAction->SetVolumeMass(entry.first, entry.second);
	}
}

G4bool TissueSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
	const G4double energyDeposited = step->GetTotalEnergyDeposit();
	if (energyDeposited <= 0.0) {
		return false;
	}

	const auto* logicalVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
	const G4String& volumeName = logicalVolume->GetName();

	if (fMassByVolume.find(volumeName) == fMassByVolume.end()) {
		return false;
	}

	fEventEnergyByVolume[volumeName] += energyDeposited;
	return true;
}

void TissueSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
	auto* runAction = RunAction::Instance();
	const auto* currentEvent = G4RunManager::GetRunManager()->GetCurrentEvent();
	if (runAction == nullptr || currentEvent == nullptr) {
		return;
	}

	const G4int eventID = currentEvent->GetEventID();
	for (const auto& entry : fEventEnergyByVolume) {
		const auto& volumeName = entry.first;
		const G4double mass = fMassByVolume[volumeName];
		runAction->RecordDose(eventID, volumeName, entry.second, mass);
	}
}
