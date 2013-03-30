#include "attack.h"
#include "utils/confignode.h"

Attack::Attack() :
	damage(0),
	push(0),
	type(0){
}

void Attack::load(ConfigNode& config){
	damage = config.getInt("damage");
	push = config.getInt("push",0);
	type = config.getInt("type",0);
}
