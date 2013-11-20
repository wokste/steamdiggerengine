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

#include "src/script/scriptengine.h"

#include <iostream>
#include <scriptbuilder/scriptbuilder.h>
#include <scripthelper/scripthelper.h>
#include <scriptstdstring/scriptstdstring.h>
#include <assert.h>
#include "src/game.h"

// Included for the script bindings
#include "src/utils/vector2.h"
#include "src/entities/entity.h"
#include "src/entities/creature.h"
#include "src/entities/player.h"
#include "src/entities/monster.h"

#include "src/utils/attack.h"
#include "src/map/mapwriter.h"
#include "src/world.h"
#include <new>

namespace ScriptUtils{
	template<class A, class B>
	B* cast(A* a){
		return a;
	}

	void print(std::string& str){
		std::cout << str << '\n';
	}

	void error(std::string& str){
		std::cout << str << '\n';
	}

	MapWriter* getMapWriter(World& world){
		return new MapWriter(world);
	}

	void Vector2iNew(void *memory){new(memory) Vector2i();}
	Vector2i Vector2iNewXY(int x, int y){return Vector2i(x,y);}
	Vector2i Vector2iAdd(Vector2i& v1, Vector2i v2) {return v1+v2;} // The first value is the object and passed by reference, the second is a parameter and passed by value.
	void Vector2iDel(void *memory){((Vector2i*)memory)->~Vector2i();}
	void Vector2dNew(void *memory){new(memory) Vector2d();}
	void Vector2dDel(void *memory){((Vector2d*)memory)->~Vector2d();}

	int XmlGetInt(pugi::xml_node& node, std::string name){
		return node.attribute(name.c_str()).as_int();
	}
	bool XmlGetBool(pugi::xml_node& node, std::string name){
		return node.attribute(name.c_str()).as_bool();
	}
	float XmlGetFloat(pugi::xml_node& node, std::string name){
		return node.attribute(name.c_str()).as_float();
	}
	std::string XmlGetString(pugi::xml_node& node, std::string name){
		return node.attribute(name.c_str()).as_string();
	}

	bool healCreature(Creature& c, std::string name, int amount){
		if (name == "hp")
			return c.HP.heal(amount);
		return 0;
	}

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
		registerEntity<T>(engine, className);
	}
};

ScriptEngine::ScriptEngine(){
	int r;
	engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	r = engine->SetMessageCallback(asFUNCTION(ScriptEngine::messageCallback), 0, asCALL_CDECL);

	RegisterStdString(engine);
	registerClasses();
	registerInterfaces();
	registerFunctions();
	context = engine->CreateContext();

	loadScripts();
}

ScriptEngine::~ScriptEngine(){
	context->Release();
	engine->Release();
}

void ScriptEngine::registerClasses(){
	int r;
	r = engine->RegisterObjectType("Vector2i", sizeof(Vector2i),asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | GetTypeTraits<Vector2i>()); assert(r >= 0);
	r = engine->RegisterObjectType("Vector2d", sizeof(Vector2d),asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asOBJ_APP_CLASS_ALIGN8  | GetTypeTraits<Vector2d>()); assert(r >= 0);
	r = engine->RegisterObjectType("Xml", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);

	r = engine->RegisterObjectType("World", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectType("Entity", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectType("Creature", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectType("Player", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectType("Monster", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
	r = engine->RegisterObjectBehaviour("Creature", asBEHAVE_IMPLICIT_REF_CAST, "Entity@ f()", asFUNCTION((&ScriptUtils::cast<Creature,Entity>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Player", asBEHAVE_IMPLICIT_REF_CAST, "Creature@ g()", asFUNCTION((&ScriptUtils::cast<Player,Creature>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Monster", asBEHAVE_IMPLICIT_REF_CAST, "Creature@ g()", asFUNCTION((&ScriptUtils::cast<Monster,Creature>)), asCALL_CDECL_OBJLAST); assert( r >= 0 );

	r = engine->RegisterObjectType("MapWriter",0, asOBJ_REF); assert(r >= 0);
}

void ScriptEngine::registerFunctions(){
	int r;
	r = engine->RegisterGlobalFunction("void print(string)", asFUNCTION(ScriptUtils::print), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("void error(string)", asFUNCTION(ScriptUtils::error), asCALL_CDECL); assert(r >= 0);

	// XML
	r = engine->RegisterObjectMethod("Xml", "int getInt(string)", asFUNCTION(ScriptUtils::XmlGetInt), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("Xml", "bool getBool(string)", asFUNCTION(ScriptUtils::XmlGetBool), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("Xml", "float getFloat(string)", asFUNCTION(ScriptUtils::XmlGetFloat), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("Xml", "string getString(string)", asFUNCTION(ScriptUtils::XmlGetString), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

	// Vector2's
	r = engine->RegisterObjectBehaviour("Vector2i", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptUtils::Vector2iNew), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Vector2i", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptUtils::Vector2iDel), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Vector2i", "int x", asOFFSET(Vector2i,x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Vector2i", "int y", asOFFSET(Vector2i,y)); assert( r >= 0 );
	r = engine->RegisterObjectMethod("Vector2i", "Vector2i opAdd(Vector2i) const",  asFUNCTION(ScriptUtils::Vector2iAdd), asCALL_CDECL_OBJFIRST); assert( r >= 0 );
	r = engine->RegisterGlobalFunction("Vector2i makeVector2i(int, int)", asFUNCTION(ScriptUtils::Vector2iNewXY), asCALL_CDECL); assert( r >= 0 );


	r = engine->RegisterObjectBehaviour("Vector2d", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptUtils::Vector2dNew), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("Vector2d", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptUtils::Vector2dDel), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Vector2d", "double x", asOFFSET(Vector2d,x)); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Vector2d", "double y", asOFFSET(Vector2d,y)); assert( r >= 0 );
	r = engine->RegisterObjectMethod("Vector2d", "Vector2i floor()", asFUNCTION((Vector2::floorVec)), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = engine->RegisterObjectMethod("Vector2d", "Vector2i ceil()", asFUNCTION((Vector2::ceilVec)), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = engine->RegisterObjectMethod("Vector2d", "double length()", asFUNCTION((Vector2::length)), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = engine->RegisterObjectMethod("Vector2d", "double lengthTo(Vector2d)", asFUNCTION((Vector2::lengthTo)), asCALL_CDECL_OBJFIRST); assert(r >= 0);
	r = engine->RegisterObjectMethod("Vector2d", "Vector2d normalize()", asFUNCTION((Vector2::normalize)), asCALL_CDECL_OBJFIRST); assert(r >= 0);

	// Entity and derived classes
	ScriptUtils::registerEntity<Entity>(engine, "Entity");
	ScriptUtils::registerCreature<Creature>(engine, "Creature");
	ScriptUtils::registerCreature<Player>(engine, "Player");
	ScriptUtils::registerCreature<Monster>(engine, "Monster");

	r = engine->RegisterObjectMethod("Player", "void selectItem(int)", asMETHOD(Player, selectItem), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("Player", "bool pickupItem(int, int)", asMETHOD(Player, pickupItem), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectProperty("Monster", "Creature traget", asOFFSET(Monster,target)); assert( r >= 0 );

	// World
	r = engine->RegisterObjectMethod("World", "MapWriter@ getMapWriter()", asFUNCTION((ScriptUtils::getMapWriter)), asCALL_CDECL_OBJFIRST); assert( r >= 0 );

	// MapWriter
	r = engine->RegisterObjectBehaviour("MapWriter", asBEHAVE_ADDREF, "void f()", asMETHOD(MapWriter,addref), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("MapWriter", asBEHAVE_RELEASE, "void f()", asMETHOD(MapWriter,release), asCALL_THISCALL); assert( r >= 0);
	r = engine->RegisterObjectMethod("MapWriter", "bool place(Vector2i, int, int)", asMETHOD(MapWriter, place), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("MapWriter", "bool damage(Vector2i, int, int damage, int damageType)", asMETHOD(MapWriter, damage), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("MapWriter", "bool solid(Vector2i, Vector2i, int)", asMETHOD(MapWriter, solid), asCALL_THISCALL); assert( r >= 0 );
}
void ScriptEngine::registerInterfaces(){
	int r;
	r = engine->RegisterInterface("IItem"); assert(r >= 0);
	r = engine->RegisterInterfaceMethod("IItem", "int useItem(Creature@, Vector2d, int)"); assert(r >= 0);
}

void ScriptEngine::loadScripts(){
	CScriptBuilder builder;
	int r;
	r = builder.StartNewModule(engine, "MyModule");assert(r >= 0);
	auto list = GameGlobals::fileSystem.getList(".as");
	for (auto filename : list){
		r = builder.AddSectionFromFile(GameGlobals::fileSystem.fullpath(filename).c_str());
		if( r < 0 ){
			std::cout << "Failed to load or proprocess: " << filename << ".\n";
			return;
		}
	}
	r = builder.BuildModule();
	if( r < 0 ){
		std::cout << "Failed to build scripts.\n";
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

asIScriptObject* ScriptEngine::createObject(const std::string& name){
	asIObjectType* objectType = engine->GetModule("MyModule")->GetObjectTypeByName (name.c_str());
	auto object = (asIScriptObject*)engine->CreateScriptObject(objectType);
	if (!object)
		std::cout << "failed to create script object " << name << "\n";
	return object;
}
