#include "tool.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"
#include "../game.h"
#include "itemdefmanager.h"
#include "block.h"

#include "../map/map.h"
#include "../map/mapnode.h"
#include "../map/lightingengine.h"
#include "../enums.h"

Tool::Tool(const ConfigNode& config) : ItemDef(config){
}

double Tool::use(Player& owner, ItemStack& itemStack, const Screen& screen){
	Block* block = mineAt(owner.world, Vector2::dToI(screen.mousePos()), Layer::back);
	if (block != nullptr){
		owner.inventory.add(block->ID);
		return block->timeToMine;
	}
	return 0;
}

Block* Tool::mineAt(World* world, Vector2i pos, int layer){
	auto t = world->map->getMapNode(pos.x, pos.y);

	if (t == nullptr || !t->isset(layer))
		return nullptr;

	if (layer == Layer::back){
		// Check if their is a block in fron of this block. In this case mine the block in front.
		Block* blockFront = t->getBlock(world->game->itemDefs.get(), Layer::front);
		if (blockFront)
			return mineAt(world, pos, Layer::front);

		if (!world->map->blockAdjacent(pos.x, pos.y, layer,
				[](const Block* block){return (block == nullptr) || (block->collisionType == BlockCollisionType::Air);}))
			return nullptr;
	}
	Block* block = t->getBlock(world->game->itemDefs.get(), layer);
	t->setBlock(nullptr, layer);
	LightingEngine::recalcAreaAround(*world->map, pos);
	return block;
}
