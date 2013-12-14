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
#include <assert.h>

namespace ScriptUtils{
	template<class A, class B>
	B* cast(A* a){
		return a;
	}

	void print(std::string& str);

	void error(std::string& str);

	MapWriter* getMapWriter(World& world);

	void Vector2iNew(void *memory);
	Vector2i Vector2iNewXY(int x, int y);
	Vector2i Vector2iAdd(Vector2i* v1, Vector2i v2);
	Vector2i Vector2iSubstract(Vector2i* v1, Vector2i v2);
	void Vector2iDel(void *memory);
	void Vector2dNew(void *memory);
	Vector2d Vector2dNewXY(double x, double y);
	Vector2d Vector2dAdd(Vector2d* v1, Vector2d v2);
	Vector2d Vector2dSubstract(Vector2d* v1, Vector2d v2);
	void Vector2dDel(void *memory);
	int XmlGetInt(pugi::xml_node* asThis, std::string name);
	bool XmlGetBool(pugi::xml_node* asThis, std::string name);
	float XmlGetFloat(pugi::xml_node* asThis, std::string name);
	std::string XmlGetString(pugi::xml_node* asThis, std::string name);
	bool healCreature(Creature* asThis, std::string name, int amount);
	void playSound(std::string soundName);
	int blockIdByTag(std::string tag);
	int itemIdByTag(std::string tag);
	EntityIterator* entitiesInArea(World* asThis, Vector2d centerPosition, Vector2d size);

	template <class T>
	void registerEntity (asIScriptEngine* engine, const char* className) {
		int r;
		r = engine->RegisterObjectMethod(className, "void push(Vector2d, double)", asMETHOD(T, push), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectProperty(className, "World@ world", asOFFSET(T,world)); assert( r >= 0 );
		r = engine->RegisterObjectProperty(className, "Vector2d pos", asOFFSET(T,pos)); assert( r >= 0 );
	}

	template <class T>
	void registerCreature (asIScriptEngine* engine, const char* className) {
		int r;
		r = engine->RegisterObjectMethod(className, "bool heal(string, int)", asFUNCTION(ScriptUtils::healCreature), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
		r = engine->RegisterObjectMethod(className, "void damage(Creature@, int, int)", asMETHOD(T, takeDamage), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectMethod(className, "bool aggressiveTo(Creature@)", asMETHOD(T, aggressiveTo), asCALL_THISCALL); assert( r >= 0 );
		registerEntity<T>(engine, className);
	}
};
