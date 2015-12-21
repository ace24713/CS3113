#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <array>
#include <vector>
#include <fstream>
#include "Math.h"

class ShaderProgram;

class TileMap
{
public:
	static const int SPRITES_PER_ROW = 8;
	static const int SPRITES_PER_COLUMN = 3;
	//static const int MAP_WIDTH = 1024;
	//static const int MAP_HEIGHT = 768;
	static const int MAP_WIDTH = 20;
	static const int MAP_HEIGHT = 10;

	template<typename T>
	using MapArray = std::array<std::array<T, MAP_HEIGHT>, MAP_WIDTH>;
	
	TileMap() {}
	void setSpriteSheet(unsigned sheet) { sprites = sheet; }
	void load(const std::string& filename);
	void Render(ShaderProgram& program);

	bool occupied(int x, int y) const
	{
		return collisionData[clamp(x, 0, MAP_WIDTH - 1)][clamp(y, 0, MAP_HEIGHT - 1)];
	}

private:

	void prepRenderData();
	void readLayerData(std::ifstream& stream);

	MapArray<bool> collisionData;
	MapArray<int> tileData;
	unsigned sprites;
	std::vector<float> texCoords;
	std::vector<float> vertexCoords;
};

#endif // !TILE_MAP_H
