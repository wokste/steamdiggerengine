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
#include "src/items/item.h"
#include "src/entities/creature.h"
#include "src/screen.h"
#include <pugixml.hpp>
#include "src/enums.h"
#include <SFML/Window/Keyboard.hpp>
#include "src/script/scriptengine.h"
#include <iostream>
#include <assert.h>

ItemType::ItemType(){
	maxStack=250;
	consumable=true;
	useTime=0.4;
	frameID=0;
}

void ItemType::loadFromXml(pugi::xml_node& configNode){
	maxStack=configNode.attribute("stack").as_int(1);
	consumable=configNode.attribute("consumable").as_bool(false);
	useTime=configNode.attribute("use-time").as_double(0.4);
	frameID=configNode.attribute("frame").as_int();

	auto scriptName = configNode.attribute("class").as_string();
	if (scriptName != ""){
		itemScript = g_ScriptEngine.createObject(scriptName);
		if (itemScript){
			asIScriptContext *context = g_ScriptEngine.context;
			context->Prepare(itemScript->GetObjectType()->GetMethodByDecl("void load(Xml@)"));
			context->SetObject(itemScript);
			context->SetArgObject(0, &configNode);
			int r = context->Execute();
			assert( r == asEXECUTION_FINISHED );
		}
	}
}

// For blocks
void ItemType::loadBlock(const std::string& objectTypeName, int blockID, int newFrameID){
	frameID=newFrameID;

	itemScript = g_ScriptEngine.createObject(objectTypeName);
	if (itemScript){
		asIScriptContext *context = g_ScriptEngine.context;
		context->Prepare(itemScript->GetObjectType()->GetMethodByDecl("void setBlock(int)"));
		context->SetObject(itemScript);
		context->SetArgDWord(0, blockID);
		int r = context->Execute();
		assert( r == asEXECUTION_FINISHED );
	}
}

ItemType::~ItemType(){
	if (itemScript)
		itemScript->Release();
}

int ItemType::use(Creature& user, const Screen& screen){
	if (itemScript == nullptr)
		return 0;

	int layer = screen.getSelectedLayer();
	Vector2d mousePos = screen.mousePos(layer);

	asIScriptContext *context = g_ScriptEngine.context;
	asIScriptFunction *func = itemScript->GetObjectType()->GetMethodByDecl("int useItem(Creature@, Vector2d, int)");
	assert(func);
	context->Prepare(func);
	context->SetObject(itemScript);
	context->SetArgObject(0, &user);
	context->SetArgObject(1, &mousePos);
	context->SetArgDWord(2, layer);
	int r = context->Execute();
	if( r == asEXECUTION_FINISHED ){
		return context->GetReturnDWord();
	}
	return 0;
}
