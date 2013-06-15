#include "mineeffect.h"
#include "../entities/player.h"

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

bool MineEffect::use(Entity& owner, Vector2d sourcePos, Vector2d targetPos, int targetLayer){
	Map& map = *owner.world->map;
	const Vector2i pos = Vector2::dToI(targetPos);
	MapNode* node = map.getMapNode(pos.x, pos.y);

	if (!node || !node->isset(targetLayer)){
		return false;
	}

	if (targetLayer == Layer::back && node->isset(Layer::front)){
		targetLayer = Layer::front;
	}

	if (!map.blockAdjacent(pos.x, pos.y, targetLayer,
			[](const BlockType& block){return (block.collisionType == BlockCollisionType::Air);}))
		return false;

	const BlockType& minedBlock = node->getBlock(map, targetLayer);
	node->setBlock(map, 0, targetLayer);
	LightingEngine::recalcAreaAround(map, pos);

	//TODO: add item in inventory
	return true;
}
