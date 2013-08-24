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
#include "game.h"
#include "utils/texture.h"
#include "items/itemdefmanager.h"
#include "map/blocktype.h"
#include <iostream>
#include <cstdlib>

std::shared_ptr<Texture> GameGlobals::tileSet;
std::unique_ptr<ItemDefManager> GameGlobals::itemDefs;
std::unique_ptr<BlockTypeManager> GameGlobals::blockDefs;
FileSystem GameGlobals::fileSystem;
std::mt19937 GameGlobals::rnd;

void GameGlobals::init(){
	tileSet.reset(new Texture(GameGlobals::fileSystem.fullpath("tileset.png")));
	itemDefs.reset(new ItemDefManager(fileSystem.fullpath("tools.xml")));
	blockDefs.reset(new BlockTypeManager(fileSystem.fullpath("blocks.xml")));

	// Actions that require the itemDefs to be loaded
	blockDefs->postLoad();
}

FileSystem::FileSystem(){
	dataDir = "data/";
}

std::string FileSystem::fullpath(const std::string& resourcename) const{
	std::cout << dataDir << " + " << resourcename << " \n";
	return dataDir + resourcename;
}
