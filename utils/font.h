#pragma once
#include <memory>
#include <vector>
#include <string>
#include "vector2.h"

class ConfigNode;
class Texture;

struct FontChar{
	int x       :8;
	int y       :8;
	int width   :8;
	int xOffset :8;

	FontChar(const ConfigNode& node);
	~FontChar();
};

class Font{
	std::vector<FontChar> charsData;
	int height;
	int yOffset;
	int firstID;
	int lastID;

public:
	Font();
	~Font();
	std::shared_ptr<Texture> texture;
	void load(const ConfigNode& node);
	void draw(const std::string& text, Rect4i area) const;
};
