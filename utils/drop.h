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
