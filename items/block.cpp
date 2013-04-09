#include "block.h"
#include "../entities/player.h"

#include "../world.h"
#include "../map.h"
#include "../utils/confignode.h"
#include "../screen.h"

Block::Block(){
}

bool Block::use(Player& owner, ItemStack& itemStack, Screen& screen){
	Vector2i mousePos = Vector2::dToI(screen.mousePos(0));

	auto dist = Vector2::length(owner.pos - Vector2::iToD(mousePos));
	//if (dist > 80)
	//	return false;

	Map* map = world->map;

	if (map->tile(mousePos.x,mousePos.y, 0)->blockId != 0)
		return false;

	//Vector2i tilePx(mousePos.x, mousePos.y);

	//if (world->areaHasEntity(Vector2.iToD(mousePos), Vector2.iToD(mousePos + Vector2i(1,1))))
	//	return false;

	// TODO: Fix buffer overflows
	//if (
	//	(map->tile(x+1,y  )->blockId == 0) && (map->tile(x-1,  y)->blockId == 0) &&
	//	(map->tile(x  ,y+1)->blockId == 0) && (map->tile(x  ,y-1)->blockId == 0))
	//	return false;

	map->setTile(mousePos.x, mousePos.y, 0, ID);
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
