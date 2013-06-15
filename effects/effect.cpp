#include "effect.h"
#include "mapeffect.h"

#include "../utils/confignode.h"

std::unique_ptr<Effect> Effect::loadEffect(ConfigNode& config){
	std::unique_ptr<Effect> effect;
	std::string typeName = config.getString("type","");

	if (typeName == "mine")
		effect.reset(new MineEffect(config));
	//if (typeName == "build")
	//	effect.reset(new BuildEffect(config));

	return effect;
}
