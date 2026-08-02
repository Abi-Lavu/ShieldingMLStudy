#ifndef ACTION_H
#define ACTION_H

#include "G4VUserActionInitialization.hh"

#include "SimulationConfig.h"

#include <memory>

class ActionInitialization : public G4VUserActionInitialization {
public:
	explicit ActionInitialization(std::shared_ptr<SimulationConfig> config);
	~ActionInitialization() override = default;

	void Build() const override;

private:
	std::shared_ptr<SimulationConfig> fConfig;
};

#endif
