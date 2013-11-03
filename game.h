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

#include <string>
#include <memory>
#include <random>
class ItemDefManager;
class World;
class BlockTypeManager;
class TileSet;
class ScriptEngine;

class FileSystem{
public:
	FileSystem();

	std::string fullpath(const std::string& resourcename) const;
private:
	std::string dataDir;
};

class GameGlobals{
public:
	static void init();
	static void clear();

	static std::unique_ptr<ItemDefManager> itemDefs;
	static FileSystem fileSystem;
	static std::mt19937 rnd;
	static std::unique_ptr<BlockTypeManager> blockDefs;
	static std::unique_ptr<ScriptEngine> scriptEngine;
	static std::shared_ptr<TileSet> tileSet;
	static bool paused;
};
