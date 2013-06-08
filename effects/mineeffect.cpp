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

MineEffect::MineEffect(const ConfigNode& config){
}

bool MineEffect::use(Entity& owner, const Screen& screen){
	const Map& map = *owner.world->map;
	const Vector2i pos = Vector2::dToI(screen.mousePos());
	MapNode* node = world.map->getMapNode(pos.x, pos.y);
	int layer = Layer::front;
	
	if (!node || !node->isset(layer)){
		// Nothing to mine.
		return false;
	}
	
	if (layer == Layer::back && node->isset(Layer::front)){
		layer = Layer::front;
	} 
	
	if (!map.blockAdjacent(pos.x, pos.y, layer,
			[](const BlockType& block){return (block.collisionType == BlockCollisionType::Air);}))
		return map.blockDefs[0];
	
	const BlockType& minedBlock = node->getBlock(map, layer);
	node->setBlock(map, 0, layer);
	LightingEngine::recalcAreaAround(map, pos);
	
	//TODO: add item in inventory
	return true;
}