#include "armour.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"

Armour::Armour(const ConfigNode& config) : ItemDef(config){
}
