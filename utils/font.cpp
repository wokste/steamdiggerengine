#include "font.h"
#include "confignode.h"
#include "texture.h"
#include "../game.h"

FontChar::FontChar(const ConfigNode& node){
	x = node.getInt("x");
	y = node.getInt("y");
	width = node.getInt("width");
	xOffset = node.getInt("xoffset");
}

FontChar::~FontChar(){}

Font::Font(){
	height  = 0;
	yOffset = 0;
	firstID = 0;
	lastID  = -1;
}

Font::~Font(){
}

void Font::load(const ConfigNode& config){
	const std::string textureName = config.getString("texture");
	texture.reset(new Texture(GameGlobals::fileSystem.fullpath(textureName)));

	const_cast<ConfigNode&>(config).getNode("chars").forEachNode([&](const ConfigNode& charNode){
		charsData.emplace_back(charNode);
	});

	height  = config.getInt("height");
	yOffset = config.getInt("yoffset");
	firstID = config.getInt("start");
	lastID  = firstID + charsData.size() - 1;
}

void Font::draw(const std::string& text, Rect4i area) const{
	Vector2i pos(area.left,area.top);

	for (auto& ascii : text){
		if (ascii < firstID || ascii > lastID){
			// TODO: Add an assert
			continue;
		}
		auto charData = charsData[ascii - firstID];

		texture->draw(Vector2i(charData.x,charData.y),Vector2i(charData.width,height), pos);

		pos.x += charData.xOffset;
	}
}
