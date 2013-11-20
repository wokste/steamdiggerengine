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

class Map;
class Attack;
#include "src/utils/vector2.h"
#include "src/utils/scriptengine.h"

class MapWriter : public refCounted{
	World& world;
	Rect4i area;
	void recalcLighting(); // Implicitely called at destruction
	void pointChanged(Vector2i pos);
	void destroy(Vector2i pos, int layer, const int damageType, int recursionTTL = 160, int blockFlags = 0);
public:
	MapWriter(World& world);
	~MapWriter();
	bool place(Vector2i pos, int layer, int blockNum);
	bool damage(Vector2i pos, int layer, const int damage, const int damageType);
	bool solid(Vector2i topLeftPos, Vector2i size, int layer);
};
