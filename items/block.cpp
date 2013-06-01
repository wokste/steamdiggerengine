#include "block.h"
#include "../entities/player.h"
#include "itemdefmanager.h"

#include "../world.h"
#include "../utils/confignode.h"
#include "../screen.h"
#include "../game.h"

#include "../map/map.h"
#include "../map/mapnode.h"
#include "../map/blocktype.h"
#include "../map/lightingengine.h"
#include "../enums.h"

Block::Block(const ConfigNode& config) : ItemDef(config){
	// TODO: How should this be done?
	blockTypeID = 1;
}

double Block::use(Player& owner, ItemStack& itemStack, const Screen& screen){
	bool useFrontLayer = !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);
	int layer = (useFrontLayer ? Layer::front : Layer::back);

	if (placeAt(*owner.world, Vector2::dToI(screen.mousePos(layer)), layer)){
		itemStack.count--;
		return 0.25;
	}
	return 0;
}

bool Block::placeAt(World& world, Vector2i pos, int layer){
	auto t = world.map->getMapNode(pos.x, pos.y);

	if (t == nullptr || t->isset(layer)) // TODO: optimize
		return false;

	if (layer == Layer::front){
		// Check if their is a block behind this block. Otherway try to place it in the back.
		const BlockType& blockBack = t->getBlock(*world.map, Layer::back);
		if (blockBack.collisionType != BlockCollisionType::Solid)
			return placeAt(world, pos, Layer::back);

		if (world.areaHasEntity(pos, pos + Vector2i(1,1)))
			return false;
	}

	if (!world.map->blockAdjacent(pos.x, pos.y, layer,
			[](const BlockType& block){return (block.collisionType == BlockCollisionType::Solid);}))
		return false;

	t->setBlock(*world.map, blockTypeID, layer);
	LightingEngine::recalcAreaAround(*world.map, pos);
	return true;
}
