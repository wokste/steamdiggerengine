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

#include "scriptengine.h"

#include <iostream>
#include <scriptbuilder/scriptbuilder.h>
#include <scripthelper/scripthelper.h>
#include <scriptstdstring/scriptstdstring.h>
#include <assert.h>
#include "../game.h"

// Included for the script bindings
#include "vector2.h"
#include "../entities/entity.h"
#include "../entities/creature.h"
#include "../entities/player.h"
#include "../entities/monster.h"

#include "../attack.h"
#include "../map/mapwriter.h"

ScriptEngine::ScriptEngine(){
	int r;
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	r = engine->SetMessageCallback(asFUNCTION(ScriptEngine::messageCallback), 0, asCALL_CDECL);

	RegisterStdString(engine);
	registerClasses();
	registerInterfaces();
	registerFunctions();
}

ScriptEngine::~ScriptEngine(){
	engine->Release();
}

void ScriptEngine::registerClasses(){
	int r;
	r = engine->RegisterObjectType("Vector2i", sizeof(Vector2i),asOBJ_VALUE | asOBJ_POD | GetTypeTraits<Vector2i>()); assert(r >= 0);
	r = engine->RegisterObjectType("Vector2d", sizeof(Vector2d),asOBJ_VALUE | asOBJ_POD | GetTypeTraits<Vector2d>()); assert(r >= 0);

	r = engine->RegisterObjectType("World", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectType("Entity", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectType("Creature", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectType("Player", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectType("Monster", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectBehaviour("Creature", asBEHAVE_IMPLICIT_REF_CAST, "Entity@ f()", asFUNCTION((&ScriptEngine::cast<Creature,Entity>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Player", asBEHAVE_IMPLICIT_REF_CAST, "Creature@ g()", asFUNCTION((&ScriptEngine::cast<Player,Creature>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Monster", asBEHAVE_IMPLICIT_REF_CAST, "Creature@ g()", asFUNCTION((&ScriptEngine::cast<Monster,Creature>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	r = engine->RegisterObjectType("MapWriter", sizeof(MapWriter), asOBJ_VALUE); assert(r >= 0);
	r = engine->RegisterObjectType("Attack", sizeof(Attack), asOBJ_VALUE); assert(r >= 0);
}

void ScriptEngine::registerFunctions(){
	int r;
	r = engine->RegisterGlobalFunction("void print(string)", asFUNCTION(Console::print), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("void error(string)", asFUNCTION(Console::error), asCALL_CDECL); assert(r >= 0);

	// Vector2's
	r = engine->RegisterObjectProperty("Vector2i", "int x", asOFFSET(Vector2i,x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Vector2i", "int y", asOFFSET(Vector2i,y)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Vector2d", "int x", asOFFSET(Vector2d,x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Vector2d", "int y", asOFFSET(Vector2d,y)); assert( r >= 0 );
	r = engine->RegisterObjectMethod("Vector2d", "Vector2i floor()", asFUNCTION((Vector2::floorVec)), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = engine->RegisterObjectMethod("Vector2d", "Vector2i ceil()", asFUNCTION((Vector2::ceilVec)), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = engine->RegisterObjectMethod("Vector2d", "int length()", asFUNCTION((Vector2::length)), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = engine->RegisterObjectMethod("Vector2d", "Vector2d normalize()", asFUNCTION((Vector2::normalize)), asCALL_CDECL_OBJFIRST); assert(r >= 0);

	// Entity and derived classes
	r = engine->RegisterObjectMethod("Entity", "void push(Vector2d, double)", asMETHOD(Entity, push), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Entity", "World@ world", asOFFSET(Entity,world)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Entity", "Vector2d pos", asOFFSET(Entity,pos)); assert( r >= 0 );
	r = engine->RegisterObjectMethod("Player", "void selectItem(int)", asMETHOD(Player, selectItem), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("Player", "bool pickupItem(int, int)", asMETHOD(Player, pickupItem), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Monster", "Creature traget", asOFFSET(Monster,target)); assert( r >= 0 );

	// World

	// MapWriter
	r = engine->RegisterObjectMethod("MapWriter", "bool place(Vector2i, int, int)", asMETHOD(MapWriter, place), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("MapWriter", "bool damage(Vector2i, int, Attack attack)", asMETHOD(MapWriter, place), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("MapWriter", "bool solid(Vector2i, int)", asMETHOD(MapWriter, place), asCALL_THISCALL); assert( r >= 0 );
}
void ScriptEngine::registerInterfaces(){
	int r;
	r = engine->RegisterInterface("IItem"); assert(r >= 0);
	r = engine->RegisterInterfaceMethod("IItem", "int use(Entity@, Creature@, Vector2d, Vector2d, int)"); assert(r >= 0);
}

void ScriptEngine::loadScript(const std::string& filename){
	CScriptBuilder builder;
	int r;
	r = builder.StartNewModule(engine, "MyModule");assert(r >= 0);
	r = builder.AddSectionFromFile(GameGlobals::fileSystem.fullpath(filename).c_str());
	if( r < 0 ){
		std::cout << "Failed to load or proprocess: " << filename << ".\n";
		return;
	}
	r = builder.BuildModule();
	if( r < 0 ){
		std::cout << "Failed to build: " << filename << ".\n";
		return;
	}
}

void ScriptEngine::messageCallback(const asSMessageInfo *msg, void *param){
	const char *type = "Script Error: ";
	if( msg->type == asMSGTYPE_WARNING )
		type = "Script Warning: ";
	else if( msg->type == asMSGTYPE_INFORMATION )
		type = "Script Info: ";
	std::cout << msg->section << " (" << msg->row << ", " << msg->col << "): " << type << msg->message << '\n';
}

void Console::print(std::string& str){
	std::cout << str << '\n';
}

void Console::error(std::string& str){
	std::cout << str << '\n';
}
