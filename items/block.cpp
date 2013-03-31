#include "block.h"
#include "../entities/player.h"

#include "../world.h"
#include "../map.h"

Block::Block(const std::string& name){
}

bool Block::use(Player& owner, ItemStack& item, Vector2i mousePos){
	auto dist = Vector2::length(owner.pos - Vector2::iToD(mousePos));
	if (dist > 80)
		return false;

	Map* map = world->map;
	int x = mousePos.x / map->tileWidth;
	int y = mousePos.y / map->tileHeight;

	if (map->tile(x,y)->blockId != 0)
		return 0;

	if (
		(map->tile(x+1,y  )->blockId == 0) && (map->tile(x-1,  y)->blockId == 0) &&
		(map->tile(x  ,y+1)->blockId == 0) && (map->tile(x  ,y-1)->blockId == 0))
		return 0;

	map->setTile(x, y, ID);

	return true;
}
