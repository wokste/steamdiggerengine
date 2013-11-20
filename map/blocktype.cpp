/*
The MIT License (MIT)

Copyright (c) 2013, Steven Wokke

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "src/map/blocktype.h"

#include "src/map/lightingengine.h"
#include "src/enums.h"
#include "src/game.h"
#include "src/items/itemdefmanager.h"
#include <iostream>
#include <sstream>
#include "src/utils/sound.h"

BlockType::BlockType(pugi::xml_node& configNode){
	collisionType = getBlockCollisionType(configNode.attribute("collision").as_string("Air"));
	HP = configNode.attribute("hp").as_int(-1);
	lightColor = LightingEngine::makeColor(configNode.attribute("light").as_string("0"));
	unsigned char blockedLightByte = configNode.attribute("blocked-light").as_int(100);
	blockedLight = sf::Color(blockedLightByte,blockedLightByte,blockedLightByte);
	mineSound = SoundSystem::loadSound(configNode.attribute("mine_sound").as_string());
	flags = getFlags(configNode.attribute("attach").as_string());

	for( auto childNode : configNode.children("frame")) {
		models.push_back(VertexArray(childNode,*GameGlobals::tileSet));
	}
}

int BlockType::getFlags(std::string text){
	int returnValue = 0;
	std::stringstream ss(text);
	const std::vector<std::pair<std::string, int>> flags = {{"top",FLAG_ATTACH_TOP}, {"bottom",FLAG_ATTACH_BOTTOM}, {"front",FLAG_ATTACH_FRONT}, {"back",FLAG_ATTACH_BACK}, {"left",FLAG_ATTACH_LEFT}, {"right",FLAG_ATTACH_RIGHT}};

	while (!ss.eof()){
		std::string flagname;
		std::getline(ss,flagname, ' ');
		for(auto& flag : flags){
			if (flagname == flag.first)
				returnValue |= flag.second;
		}
	}
	return returnValue;
}

int BlockType::getModelId() const{
	// TODO: Make it possible to have rare variants
	if (models.empty())
		return -1;
	return rand() % models.size();
}

BlockTypeManager::BlockTypeManager(std::string fileName){
	pugi::xml_document doc;
	auto result = doc.load_file(fileName.c_str());
	if (result){
		auto parentNode = doc.child("root");
		for (auto blockNode : parentNode){
			BlockType block = BlockType(blockNode);
			auto dropNode = blockNode.child("drops");
			if (dropNode){
				block.drops.load(dropNode);
			}

			if (!dropNode || dropNode.attribute("drop_this").as_bool()) {
				int blockID = blocks.size();
				int iconFrame = blockNode.child("frame").attribute("id").as_int();
				int dropID = GameGlobals::itemDefs->addBuildingBlock(blockID, iconFrame, blockNode.attribute("tag").as_string());
				block.drops.emplace_back(dropID);
			}
			blocks.push_back(block);
		}
	} else {
		std::cerr << result.description();
	}
}

void BlockTypeManager::postLoad(){
	for(auto& block: blocks){
		block.drops.postLoad();
	}
}

BlockTypeManager::~BlockTypeManager(){
}

BlockType& BlockTypeManager::operator[](int id){
	return blocks[id];
}
