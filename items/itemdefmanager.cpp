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
#include "itemdefmanager.h"
#include "item.h"

#include <pugixml.hpp>
#include "../utils/texture.h"
#include "../map/blocktype.h"
#include "../effects/mapeffect.h"
#include <iostream>

ItemDefManager::ItemDefManager(const std::string& fileName){
	pugi::xml_document doc;

	auto result = doc.load_file(fileName.c_str());
	if (result){
		auto rootNode = doc.child("root");
		for (auto childNode : rootNode){
			insert(std::unique_ptr<ItemType>(new ItemType(childNode)), childNode.attribute("tag").as_string());
		}
	} else {
		std::cerr << result.description();
	}
}

ItemDefManager::~ItemDefManager(){

}

/** inserts item. returns the itemID */
int ItemDefManager::insert(std::unique_ptr<ItemType> type, const std::string& tag){
	items.push_back(std::move(type));
	int id = items.size() - 1;
	if(tag != ""){
		tags.insert(make_pair(tag, id));
	}
	return id;
}

/** creates item for building block. returns the itemID */
int ItemDefManager::addBuildingBlock(int blockID, int frameID, const std::string& tag){
	auto item = std::unique_ptr<ItemType>(new ItemType(frameID));
	item->onUse.add(std::unique_ptr<Effect>(new BuildEffect(blockID)));
	return insert(std::move(item), tag);
}

int ItemDefManager::at(std::string& tag){
	auto iterator = tags.find(tag);
	if (iterator == tags.end()) {
		std::cerr << "item tag: " << tag << " not found\n";
		return -1;
	} else {
		return iterator->second;
	}
}
