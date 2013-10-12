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
#include "blocktype.h"

#include "lightingengine.h"
#include "../enums.h"
#include "../game.h"
#include "../items/itemdefmanager.h"
#include <iostream>

BlockType::BlockType(pugi::xml_node& configNode){
	collisionType = getBlockCollisionType(configNode.attribute("collision").as_string("Air"));
	HP = configNode.attribute("hp").as_int(-1);
	lightColor = LightingEngine::makeColor(configNode.attribute("light").as_string("0"));
	artificial = configNode.attribute("artificial").as_bool(false);
	unsigned char blockedLightByte = configNode.attribute("blocked-light").as_int(100);
	blockedLight = sf::Color(blockedLightByte,blockedLightByte,blockedLightByte);

	for( auto childNode : configNode) {
		models.push_back(VertexArray(childNode,*GameGlobals::tileSet));
	}
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
		for (auto childNode : parentNode){
			BlockType block = BlockType(childNode);
			std::string dropTag = childNode.attribute("drop").as_string("DEFAULT");
			if (dropTag == "DEFAULT"){
				int blockID = blocks.size();
				int iconFrame = childNode.child("frame").attribute("id").as_int();
				int dropID = GameGlobals::itemDefs->addBuildingBlock(blockID, iconFrame, childNode.attribute("tag").as_string());
				block.drops.emplace_back(dropID);
			} else if (dropTag != ""){
				block.drops.emplace_back(dropTag, 1.0, 1);
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
