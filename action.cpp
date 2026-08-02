#include "action.h"

#include "generator.h"
#include "RunAction.h"

#include <utility>

ActionInitialization::ActionInitialization(std::shared_ptr<SimulationConfig> config)
	: fConfig(std::move(config))
{}

void ActionInitialization::Build() const
{
	SetUserAction(new RunAction(fConfig));
	SetUserAction(new PrimaryGenerator(fConfig));
}
