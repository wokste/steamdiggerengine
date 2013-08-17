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
#include <vector>
#include "../utils/vector2.h"

class World;

class ItemReference{
public:
	inline ItemReference(int id) : id(id){}
	inline ItemReference(std::string tag) : id(-1), tag(tag){}
	int getID() const;
private:
	mutable int id;
	mutable std::string tag;
};

struct Drop{
	Drop(int id);
	Drop(std::string tag);
private:
	ItemReference ref;
	double chance;
	int minSize;
	int maxSize;

	friend class DropList;
};

class DropList : public std::vector<Drop>{
public:
	void dropStuff(World& world, Vector2d pos) const;
};
