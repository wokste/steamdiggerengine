#include "block.h"
#include "../entities/player.h"
#include "itemdefmanager.h"

#include "../world.h"
#include "../map/map.h"
#include "../utils/confignode.h"
#include "../screen.h"

Block::Block(const ConfigNode& config) : ItemDef(config){
	collisionType = getBlockCollisionType(config.getString("collision", "Air"));
	materialType = getBlockMaterialType(config.getString("material", "None"));
	startFrame = config.getInt("frame-start", -1);
	numFrames = config.getInt("frame-count", 1);
	timeToMine = config.getDouble("time-to-mine", -1);
}


int Block::use(Player& owner, ItemStack& itemStack, const Screen& screen){
	bool useFrontLayer = !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);
	int layer = (useFrontLayer ? 0 : 1);

	if (placeAt(owner.world, Vector2::dToI(screen.mousePos(layer)), layer)){
		itemStack.count--;
		return 250;
		// TODO: configurable time to mine
	}
	return 0;
}

bool Block::placeAt(World* world, Vector2i pos, int layer){
	auto t = world->map->tileRef(pos.x, pos.y, layer);

	if (t == nullptr || t->blockId >= 0)
		return false;

	if (layer == 0){
		// Check if their is a block behind this block. Otherway try to place it in the back.
		Block* blockBack = world->map->blockRef(pos.x, pos.y, 1);
		if (!blockBack || blockBack->collisionType != BlockCollisionType::Solid)
			return placeAt(world, pos, 1);

		if (world->areaHasEntity(pos, pos + Vector2i(1,1)))
			return false;
	}

	if (!world->map->blockAdjacent(pos.x, pos.y, layer, BlockCollisionType::Solid))
		return false;

	t->setBlock(this);

	return true;
}
