#include "tool.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../map.h"
#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"

Tool::Tool(){
}

bool Tool::use(Player& owner, ItemStack& itemStack, Screen& screen){
	bool useFrontLayer = !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);
	int layerNum = useFrontLayer ? 0 : 1;

	Vector2i mousePos = Vector2::dToI(screen.mousePos(layerNum));
	auto dist = Vector2::length(owner.pos - Vector2::iToD(mousePos));
	//if (dist > 80)
	//	return false;

	auto t = world->map->tileRef(mousePos.x, mousePos.y, layerNum);

	if (t == nullptr || t->blockId == 0)
		return false;

	t->blockId = 0;
	t->frame = -1;

	return true;
}

void Tool::load(ConfigNode& config){
	ItemDef::load(config);
}
