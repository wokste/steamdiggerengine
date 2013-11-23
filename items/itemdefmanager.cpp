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
#include "src/items/itemdefmanager.h"
#include "src/items/item.h"

#include <pugixml.hpp>
#include "src/utils/texture.h"
#include "src/map/blocktype.h"
#include <iostream>

ItemDefManager::ItemDefManager(){

}

ItemDefManager::~ItemDefManager(){

}

void ItemDefManager::loadFromXml(const std::string& fileName){
	pugi::xml_document doc;

	auto result = doc.load_file(fileName.c_str());
	if (result){
		auto rootNode = doc.child("root");
		for (auto childNode : rootNode){
			int id = find(childNode.attribute("tag").as_string(), true);
			items[id]->loadFromXml(childNode);
		}
	} else {
		std::cerr << result.description();
	}
}

/// Finds item id corresponding to the current item.
/// @param tag. The tag of the item.
/// @param forCreation. Whether the item is actually created after finding it.
/// Used to detect whether tags are unique and used.
/// @return A valid ID for the item or a negative value indicating an error.
int ItemDefManager::find(const std::string& tag, bool forCreation){
	auto iterator = tags.find(tag);
	if (iterator == tags.end()) {
		items.push_back(std::unique_ptr<ItemType>(new ItemType()));
		int id = items.size() - 1;
		if(tag != ""){
			tags.insert(make_pair(tag, id));
		}
		return id;
	} else {
		return iterator->second;
	}
}
