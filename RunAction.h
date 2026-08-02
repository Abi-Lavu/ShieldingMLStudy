#ifndef RUN_ACTION_H
#define RUN_ACTION_H

#include "G4String.hh"
#include "G4Types.hh"
#include "G4UserRunAction.hh"

#include "SimulationConfig.h"

#include <map>
#include <memory>
#include <vector>

struct DoseRecord {
	G4int eventID;
	G4String volumeName;
	G4double energyDeposited;
	G4double mass;
	G4double dose;
};

// Everything a student needs to read off after a run: how much dose landed
// in the organ, and how much of that is "per proton" (i.e. per primary
// particle fired), which is the number that's actually comparable across
// runs with different event counts.
struct RunSummary {
	G4double organEnergyDeposited = 0.0;
	G4double organMass = 0.0;
	G4double organDose = 0.0;
	G4int scoredOrganEvents = 0;
};

class RunAction : public G4UserRunAction {
public:
	explicit RunAction(std::shared_ptr<SimulationConfig> config);
	~RunAction() override = default;

	void BeginOfRunAction(const G4Run*) override;
	void EndOfRunAction(const G4Run*) override;

	void SetOutputCsvFilename(const G4String& filename);
	void SetVolumeMass(const G4String& volumeName, G4double mass);
	void RecordDose(G4int eventID, const G4String& volumeName, G4double energyDeposited, G4double mass);
	RunSummary GetRunSummary() const;

	static RunAction* Instance();

private:
	std::shared_ptr<SimulationConfig> fConfig;
	std::vector<DoseRecord> fRecords;
	std::map<G4String, G4double> fEnergyByVolume;
	std::map<G4String, G4double> fMassByVolume;
	static RunAction* fInstance;
};

#endif
