#ifndef TISSUE_SENSITIVE_DETECTOR_H
#define TISSUE_SENSITIVE_DETECTOR_H

#include "G4String.hh"
#include "G4Types.hh"
#include "G4VSensitiveDetector.hh"

#include <map>

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class TissueSensitiveDetector : public G4VSensitiveDetector {
public:
	explicit TissueSensitiveDetector(const G4String& name);
	~TissueSensitiveDetector() override = default;

	void RegisterVolume(const G4String& volumeName, G4double mass);

	void Initialize(G4HCofThisEvent*) override;
	G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
	void EndOfEvent(G4HCofThisEvent*) override;

private:
	std::map<G4String, G4double> fMassByVolume;
	std::map<G4String, G4double> fEventEnergyByVolume;
};

#endif
