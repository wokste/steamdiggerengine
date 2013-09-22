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
#include "../utils/vector2.h"
#include <memory>
#include <pugixml.hpp>

class Entity;
class Creature;
class Screen;

struct EffectParams{
	Entity& entity;
	Creature* eventInstignator;
	Vector2d sourcePos;
	Vector2d targetPos;
	int targetLayer;

	EffectParams(Entity& entity, Creature* eventInstignator, Vector2d sourcePos, Vector2d targetPos, int targetLayer)
		: entity(entity), eventInstignator(eventInstignator), sourcePos(sourcePos), targetPos(targetPos), targetLayer(targetLayer){}
};

class Effect{
public:
	Effect(){};
	~Effect(){};
	virtual int run(EffectParams& params) = 0;
	static std::unique_ptr<Effect> loadEffect(pugi::xml_node& node);
};

class EffectSlot{
public:
	EffectSlot();
	~EffectSlot();
	void load(pugi::xml_node& node);
	int run(EffectParams& params);
	void add(std::unique_ptr<Effect> effect);
private:
	std::vector<std::unique_ptr<Effect>> effects;
};
