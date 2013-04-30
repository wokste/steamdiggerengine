#include "tool.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../map/map.h"
#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"

Tool::Tool(const ConfigNode& config) : ItemDef(config){
}

int Tool::use(Player& owner, ItemStack& itemStack, const Screen& screen){
	int itemID = mineAt(owner.world, Vector2::dToI(screen.mousePos(-1)), 1);
	if (itemID != -1){
		owner.inventory.add(itemID);
		return 500;
		// TODO: Mining speed
	}
	return 0;
}

//TODO: return a Block* instead of an int
int Tool::mineAt(World* world, Vector2i pos, int layer){
	auto t = world->map->tileRef(pos.x, pos.y, layer);

	if (t == nullptr || t->blockId < 0)
		return -1;

	if (layer == 1){
		// Check if their is a block in fron of this block. In this case mine the block in front.
		Block* blockFront = world->map->blockRef(pos.x, pos.y, 0);
		if (blockFront)
			return mineAt(world, pos, 0);

		if (!world->map->blockAdjacent(pos.x, pos.y, layer, BlockCollisionType::Air))
			return -1;
	}
	int blockId = t->blockId;
	t->setBlock(nullptr);
	return blockId;
}
