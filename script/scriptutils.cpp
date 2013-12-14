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

#include "src/script/scriptutils.h"

#include "src/utils/vector2.h"
#include "src/entities/entity.h"
#include "src/entities/creature.h"
#include "src/entities/player.h"
#include "src/entities/monster.h"

#include "src/entities/entityiterator.h"
#include "src/utils/filesystem.h"

#include "src/utils/attack.h"
#include "src/map/mapwriter.h"
#include "src/world.h"
#include "src/items/itemdefmanager.h"
#include "src/map/blocktype.h"
#include "src/utils/sound.h"
#include <new>

void ScriptUtils::print(std::string& str){
	std::cout << str << '\n';
}

void ScriptUtils::error(std::string& str){
	std::cout << str << '\n';
}

MapWriter* ScriptUtils::getMapWriter(World& world){
	return new MapWriter(world);
}

void ScriptUtils::Vector2iNew(void *memory){
	new(memory) Vector2i();
}

Vector2i ScriptUtils::Vector2iNewXY(int x, int y){
	return Vector2i(x,y);
}

Vector2i ScriptUtils::Vector2iAdd(Vector2i* v1, Vector2i v2) {
	return *v1+v2;
}

Vector2i ScriptUtils::Vector2iSubstract(Vector2i* v1, Vector2i v2) {
	return *v1-v2;
}

void ScriptUtils::Vector2iDel(void *memory){
	((Vector2i*)memory)->~Vector2i();
}

void ScriptUtils::Vector2dNew(void *memory){
	new(memory) Vector2d();
}

Vector2d ScriptUtils::Vector2dNewXY(double x, double y){
	return Vector2d(x,y);
}

Vector2d ScriptUtils::Vector2dAdd(Vector2d* v1, Vector2d v2) {
	return *v1+v2;
}

Vector2d ScriptUtils::Vector2dSubstract(Vector2d* v1, Vector2d v2) {
	return *v1-v2;
}

void ScriptUtils::Vector2dDel(void *memory){
	((Vector2d*)memory)->~Vector2d();
}

int ScriptUtils::XmlGetInt(pugi::xml_node* asThis, std::string name){
	return asThis->attribute(name.c_str()).as_int();
}
bool ScriptUtils::XmlGetBool(pugi::xml_node* asThis, std::string name){
	return asThis->attribute(name.c_str()).as_bool();
}
float ScriptUtils::XmlGetFloat(pugi::xml_node* asThis, std::string name){
	return asThis->attribute(name.c_str()).as_float();
}
std::string ScriptUtils::XmlGetString(pugi::xml_node* asThis, std::string name){
	return asThis->attribute(name.c_str()).as_string();
}

bool ScriptUtils::healCreature(Creature* asThis, std::string name, int amount){
	if (name == "hp")
		return asThis->HP.heal(amount);
	return 0;
}

void ScriptUtils::playSound(std::string soundName){
	SoundSystem::playSound(SoundSystem::loadSound(soundName));
}

int ScriptUtils::blockIdByTag(std::string tag){
	return g_BlockDefs.find(tag);
}

int ScriptUtils::itemIdByTag(std::string tag){
	return g_ItemDefs.find(tag);
}

EntityIterator* ScriptUtils::entitiesInArea(World* asThis, Vector2d centerPosition, Vector2d size){
	return new EntityIterator(asThis->entities->begin(), asThis->entities->end(), Rect4d(centerPosition - size * 0.5, size));
}
