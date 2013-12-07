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
#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <iostream>

template <typename T>
class ResourceManager{
protected:
	std::vector<std::unique_ptr<T>> resources;
	std::map<std::string, int> tags;
public:
	ResourceManager(){}
	~ResourceManager(){}
	T& operator[](int id){return *resources[id];}
	int find(const std::string& tag, bool forCreation = false);
	void loadXml(const std::string& fileName);
};


/// Finds item id corresponding to the current item.
/// @param tag. The tag of the item.
/// @param forCreation. Whether the item is actually created after finding it.
/// Used to detect whether tags are unique and used.
/// @return A valid ID for the item or a negative value indicating an error.
template <typename T>
int ResourceManager<T>::find(const std::string& tag, bool forCreation){
	auto iterator = tags.find(tag);
	if (iterator == tags.end()) {
		// Item does not exist yet. Gonna create one
		int id = resources.size();
		resources.push_back(std::unique_ptr<T>(new T(id)));
		if(tag != ""){
			tags.insert(make_pair(tag, id));
		}
		return id;
	} else {
		return iterator->second;
	}
}


template <typename T>
void ResourceManager<T>::loadXml(const std::string& fileName){
	pugi::xml_document doc;

	auto result = doc.load_file(fileName.c_str());
	if (result){
		auto rootNode = doc.child("root");
		for (auto childNode : rootNode){
			int id = find(childNode.attribute("tag").as_string(), true);
			resources[id]->loadXml(childNode);
		}
	} else {
		std::cerr << result.description();
	}
}
