#pragma once
#include "mapnode.h"

class MapGenerator;

class Chunk{
public:
	static constexpr int width = 16; // a power of 2
	static constexpr int widthMask = width - 1;

	static constexpr int height = 16; // a power of 2
	static constexpr int heightMask = height - 1;
	MapNode nodes[width][height];

	Chunk(const MapGenerator& generator, const Vector2i& topLeft);
	void render(const sf::Color& skyColor, Texture& texture, const Vector2i& topLeft) const;
};

// == Constraints ==
static_assert(!(Chunk::width  & Chunk::widthMask),  "Chunk::width must be a power of 2");
static_assert(!(Chunk::height & Chunk::heightMask), "Chunk::height must be a power of 2");
