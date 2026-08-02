#ifndef GENERATOR_H
#define GENERATOR_H

#include "G4ParticleGun.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "SimulationConfig.h"

#include <memory>

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
	explicit PrimaryGenerator(std::shared_ptr<SimulationConfig> config);
	~PrimaryGenerator() override;

	void GeneratePrimaries(G4Event*) override;

	void SetParticleType(const G4String& particleType);
	void SetBeamEnergy(G4double energy);
	void SetBeamPosition(const G4ThreeVector& position);
	void SetBeamDirection(const G4ThreeVector& direction);
	void SetBeamRadius(G4double radius);

	const SimulationConfig& GetConfig() const;
	static PrimaryGenerator* Instance();

private:
	G4ThreeVector SampleBeamPosition() const;
	void ConfigureParticleGun();

	std::shared_ptr<SimulationConfig> fConfig;
	G4ParticleGun* fParticleGun;
	static PrimaryGenerator* fInstance;
};

#endif
