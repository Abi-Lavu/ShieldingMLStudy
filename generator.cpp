#include "generator.h"

#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include <cmath>
#include <stdexcept>
#include <utility>

PrimaryGenerator* PrimaryGenerator::fInstance = nullptr;

PrimaryGenerator::PrimaryGenerator(std::shared_ptr<SimulationConfig> config)
	: fConfig(std::move(config)),
	  fParticleGun(new G4ParticleGun(1))
{
	fInstance = this;
}

PrimaryGenerator::~PrimaryGenerator()
{
	if (fInstance == this) {
		fInstance = nullptr;
	}

	delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
	ConfigureParticleGun();
	fParticleGun->SetParticlePosition(SampleBeamPosition());
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGenerator::SetParticleType(const G4String& particleType)
{
	if (particleType == "electron") {
		fConfig->beam.particleType = "e-";
		return;
	}

	fConfig->beam.particleType = particleType;
}

void PrimaryGenerator::SetBeamEnergy(G4double energy)
{
	fConfig->beam.energy = energy;
}

void PrimaryGenerator::SetBeamPosition(const G4ThreeVector& position)
{
	fConfig->beam.position = position;
}

void PrimaryGenerator::SetBeamDirection(const G4ThreeVector& direction)
{
	fConfig->beam.direction = direction.unit();
}

void PrimaryGenerator::SetBeamRadius(G4double radius)
{
	fConfig->beam.radius = radius;
}

const SimulationConfig& PrimaryGenerator::GetConfig() const
{
	return *fConfig;
}

PrimaryGenerator* PrimaryGenerator::Instance()
{
	return fInstance;
}

G4ThreeVector PrimaryGenerator::SampleBeamPosition() const
{
	if (fConfig->beam.radius <= 0.0) {
		return fConfig->beam.position;
	}

	const G4ThreeVector direction = fConfig->beam.direction.unit();
	const G4ThreeVector reference = std::abs(direction.dot(G4ThreeVector(0.0, 0.0, 1.0))) < 0.99
		? G4ThreeVector(0.0, 0.0, 1.0)
		: G4ThreeVector(0.0, 1.0, 0.0);

	const G4ThreeVector axis1 = direction.cross(reference).unit();
	const G4ThreeVector axis2 = direction.cross(axis1).unit();
	const G4double radius = fConfig->beam.radius * std::sqrt(G4UniformRand());
	const G4double phi = 2.0 * CLHEP::pi * G4UniformRand();

	return fConfig->beam.position + radius * std::cos(phi) * axis1 + radius * std::sin(phi) * axis2;
}

void PrimaryGenerator::ConfigureParticleGun()
{
	G4String particleName = fConfig->beam.particleType;
	if (particleName == "electron") {
		particleName = "e-";
	}

	G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
	if (particle == nullptr) {
		throw std::runtime_error("Unknown particle type requested: " + std::string(fConfig->beam.particleType));
	}

	fParticleGun->SetParticleDefinition(particle);
	fParticleGun->SetParticleEnergy(fConfig->beam.energy);
	fParticleGun->SetParticleMomentumDirection(fConfig->beam.direction.unit());
}
