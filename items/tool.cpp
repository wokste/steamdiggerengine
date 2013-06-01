#include "tool.h"
#include "../entities/player.h"
#include "../entities/projectile.h"

#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"
#include "../game.h"
#include "../map/blocktype.h"

#include "../map/map.h"
#include "../map/mapnode.h"
#include "../map/lightingengine.h"
#include "../enums.h"

Tool::Tool(const ConfigNode& config) : ItemDef(config){
}

double Tool::use(Player& owner, ItemStack& itemStack, const Screen& screen){
	const BlockType& block = mineAt(*owner.world, Vector2::dToI(screen.mousePos()), Layer::back);
	//if (block != owner.world->map->blockDefs[0]){
		//TODO: add item in inventory
	//	return block.timeToMine;
	//}
	return 0;
}

const BlockType& Tool::mineAt(World& world, Vector2i pos, int layer){
	auto t = world.map->getMapNode(pos.x, pos.y);

	if (t == nullptr || !t->isset(layer))
		return world.map->blockDefs[0];

	if (layer == Layer::back){
		if (t->isset(Layer::front))
			return mineAt(world, pos, Layer::front);

		if (!world.map->blockAdjacent(pos.x, pos.y, layer,
				[](const BlockType& block){return (block.collisionType == BlockCollisionType::Air);}))
			return world.map->blockDefs[0];
	}
	const BlockType& block = t->getBlock(*world.map, layer);
	t->setBlock(*world.map, 0, layer);
	LightingEngine::recalcAreaAround(*world.map, pos);
	return block;
}
