#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <array>
#include <vector>

class ShaderProgram;

// names correspond rougly to numpad positions
// number corresponds to position in spritemap

enum TILE
{
	TILE_0 = 15,
	TILE_7 = 0,
	TILE_8 = 1,
	TILE_9 = 2,
	TILE_4 = 8,
	TILE_5 = 9,
	TILE_6 = 10,
	TILE_1 = 16,
	TILE_2 = 17,
	TILE_3 = 18,
	TILE_79 = 4,
	TILE_46 = 12,
	TILE_13 = 20,
	TILE_UNKNOWN
	//TILE_09 = 5,
	//TILE_08 = 6,
	//TILE_07 = 7
};

class TileMap
{
public:
	static const int SPRITES_PER_ROW = 8;
	static const int SPRITES_PER_COLUMN = 3;
	//static const int MAP_WIDTH = 1024;
	//static const int MAP_HEIGHT = 768;
	static const int MAP_WIDTH = 30;
	static const int MAP_HEIGHT = 30;
	static const int LOWER_CUTOFF = 4;
	static const int UPPER_CUTOFF = 4;

	template<typename T>
	using MapArray = std::array<std::array<T, MAP_HEIGHT>, MAP_WIDTH>;
	
	TileMap() {}
	void setSpriteSheet(unsigned sheet) { sprites = sheet; }
	void generate();
	void Render(ShaderProgram& program);

	bool occupied(int x, int y) const { return collisionData[x][y]; }

private:
	void randomize();
	void smooth();
	void cull();
	void tileify();
	void prepRenderData();

	TILE computeTile(int configuration);
	int computeConfiguration(int x, int y);

	MapArray<bool> collisionData;
	MapArray<TILE> tileData;
	unsigned sprites;
	std::vector<float> texCoords;
	std::vector<float> vertexCoords;
};

#endif // !TILE_MAP_H
