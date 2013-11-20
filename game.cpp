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
#include "src/game.h"
#include "src/utils/texture.h"
#include "src/script/scriptengine.h"
#include "src/items/itemdefmanager.h"
#include "src/map/blocktype.h"
#include <iostream>
#include <cstdlib>
#include <dirent.h>

std::shared_ptr<TileSet> GameGlobals::tileSet;
std::unique_ptr<ItemDefManager> GameGlobals::itemDefs;
std::unique_ptr<BlockTypeManager> GameGlobals::blockDefs;
std::unique_ptr<ScriptEngine> GameGlobals::scriptEngine;
FileSystem GameGlobals::fileSystem;
std::mt19937 GameGlobals::rnd;
bool GameGlobals::paused;

void GameGlobals::init(){
	scriptEngine.reset(new ScriptEngine());
	tileSet.reset(new TileSet(Vector2i(8,8), GameGlobals::fileSystem.fullpath("tileset.png")));
	itemDefs.reset(new ItemDefManager(fileSystem.fullpath("tools.xml")));
	blockDefs.reset(new BlockTypeManager(fileSystem.fullpath("blocks.xml")));

	// Actions that require the itemDefs to be loaded
	blockDefs->postLoad();

	paused = false;
}

FileSystem::FileSystem() : dataDir("data/"){
}

std::string FileSystem::fullpath(const std::string& resourcename) const{
	return dataDir + resourcename;
}

// Inspired by http://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Simple-Directory-Lister.html
std::vector<std::string> FileSystem::getList(std::string extention) const{
	std::vector<std::string> fileNames;
	DIR *dir;
	dirent *directoryEntry;

	dir = opendir(dataDir.c_str());
	if (dir != nullptr){
		while (directoryEntry = readdir(dir)){
			std::string name = directoryEntry->d_name;
			if (name.length() > extention.length() && name.compare(name.length() - extention.length(),extention.length(),extention) == 0)
				fileNames.push_back(name);
		}
		closedir(dir);
	} else
		std::cerr << "Couldn't open file directory";

	return fileNames;
}

void GameGlobals::clear(){
	blockDefs.reset();
	itemDefs.reset();
	tileSet.reset();
	scriptEngine.reset();
}
