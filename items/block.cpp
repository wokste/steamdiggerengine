#include "block.h"
#include "../entities/player.h"

#include "../world.h"
#include "../map.h"
#include "../utils/confignode.h"
#include "../screen.h"

Block::Block(){
}

bool Block::use(Player& owner, ItemStack& itemStack, Screen& screen){
	bool useFrontLayer = !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);
	int layerNum = useFrontLayer ? 0 : 1;

	Vector2i mousePos = Vector2::dToI(screen.mousePos(layerNum));
	auto dist = Vector2::length(owner.pos - Vector2::iToD(mousePos));
	//if (dist > 80)
	//	return false;

	auto t = world->map->tileRef(mousePos.x, mousePos.y, layerNum);

	if (t == nullptr || t->blockId != 0)
		return false;

	if (useFrontLayer && world->areaHasEntity(mousePos, mousePos + Vector2i(1,1)))
		return false;

	// TODO: Fix buffer overflows
	//if (
	//	(map->tile(x+1,y  )->blockId == 0) && (map->tile(x-1,  y)->blockId == 0) &&
	//	(map->tile(x  ,y+1)->blockId == 0) && (map->tile(x  ,y-1)->blockId == 0))
	//	return false;

	t->setBlock(this);
	itemStack.count--;
	return true;
}

void Block::load(ConfigNode& config){
	ItemDef::load(config);

	collisionType = getBlockCollisionType(config.getString("collision", "Air"));
	materialType = getBlockMaterialType(config.getString("material", "None"));
	startFrame = config.getInt("frame-start", -1);
	numFrames = config.getInt("frame-count", 1);
	timeToMine = config.getDouble("time-to-mine", -1);
}
