#include "block.h"
#include "../entities/player.h"

#include "../world.h"
#include "../map.h"
#include "../utils/confignode.h"

Block::Block(const std::string& name){
}

bool Block::use(Player& owner, ItemStack& itemStack, Vector2i mousePos){
	auto dist = Vector2::length(owner.pos - Vector2::iToD(mousePos));
	if (dist > 80)
		return false;

	Map* map = world->map;
	int x = mousePos.x / map->tileSize.x;
	int y = mousePos.y / map->tileSize.y;

	if (map->tile(x,y)->blockId != 0)
		return false;

	Vector2i tilePx(x * map->tileSize.x, y * map->tileSize.y);

	if (world->areaHasEntity(tilePx, tilePx + map->tileSize))
		return false;

	// TODO: Fix buffer overflows
	if (
		(map->tile(x+1,y  )->blockId == 0) && (map->tile(x-1,  y)->blockId == 0) &&
		(map->tile(x  ,y+1)->blockId == 0) && (map->tile(x  ,y-1)->blockId == 0))
		return false;

	map->setTile(x, y, ID);
	itemStack.count--;
	return true;
}

void Block::load(ConfigNode& config){
	ItemDef::load(config);

	collisionType = getBlockCollisionType(config.getString("collision", "Air"));
	materialType = getBlockMaterialType(config.getString("material", "None"));
	frameType = getBlockFrameType(config.getString("frame-method", "None"));
	startFrame = config.getInt("frame-start");
	//timeToMine = config.getDouble("time-to-mine");
}
