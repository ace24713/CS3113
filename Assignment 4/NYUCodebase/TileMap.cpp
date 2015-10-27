#include "TileMap.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "ShaderProgram.h"

void TileMap::generate()
{
	randomize();
	for (size_t i = 0; i < 5; ++i)
		smooth();
	cull();
	tileify();
	prepRenderData();
}

void TileMap::randomize()
{
	srand(time(NULL));
	for (int x = 0; x < MAP_WIDTH; ++x)
		for (int y = 0; y < MAP_HEIGHT; ++y)
			collisionData[x][y] = (rand() % 2 == 0);
	for (int i = 0; i < std::max(MAP_WIDTH, MAP_HEIGHT); ++i)
	{
		if (i < MAP_WIDTH)
		{
			collisionData[i][0] = true;
			collisionData[i][MAP_HEIGHT - 1] = true;
		}
		if (i < MAP_HEIGHT)
		{
			collisionData[0][i] = true;
			collisionData[MAP_WIDTH - 1][i] = true;
		}
	}
}

void TileMap::smooth()
{
	MapArray<bool> previous = collisionData;
	for (int x = 1; x < MAP_WIDTH-1; ++x)
	{
		for (int y = 1; y < MAP_HEIGHT-1; ++y)
		{
			int count = 0;
			for (int xd = -1; xd <= 1; ++xd)
				for (int yd = -1; yd <= 1; ++yd)
					if ((xd != 0 || yd != 0) && previous[x + xd][y + yd])
						++count;
			if (count > UPPER_CUTOFF)
				collisionData[x][y] = true;
			else if (count < LOWER_CUTOFF)
				collisionData[x][y] = false;
		}
	}
}

TILE TileMap::computeTile(int configuration)
{
	static const int northbit = 1 << 0;
	static const int eastbit = 1 << 1;
	static const int westbit = 1 << 2;
	static const int southbit = 1 << 3;

	switch (configuration)
	{
	case (southbit | eastbit) :
		return TILE_7;
	case (eastbit | southbit | westbit) :
		return TILE_8;
	case (westbit | southbit) :
		return TILE_9;
	case (northbit | southbit | eastbit) :
		return TILE_4;
	case (northbit | southbit | eastbit | westbit) :
		return TILE_5;
	case (northbit | southbit | westbit) :
		return TILE_6;
	case (northbit | eastbit) :
		return TILE_1;
	case (eastbit | westbit | northbit) :
		return TILE_2;
	case (westbit | northbit) :
		return TILE_3;
	case (southbit) :
		return TILE_79;
	case (southbit | northbit) :
		return TILE_46;
	case (northbit) :
		return TILE_13;
	default:
		return TILE_UNKNOWN;
	}
}

int TileMap::computeConfiguration(int x, int y)
{
	int left = std::max(0, x - 1);
	int right = std::min(MAP_WIDTH - 1, x + 1);
	int below = std::max(0, y - 1);
	int above = std::min(MAP_HEIGHT - 1, y + 1);

	bool north = collisionData[x][above];
	bool south = collisionData[x][below];
	bool west = collisionData[left][y];
	bool east = collisionData[right][y];
	int configuration = (north << 0) | (east << 1) | (west << 2) | (south << 3);
	return configuration;
}

void TileMap::cull()
{
	bool noneCulled = true;
	do
	{
		noneCulled = true;
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			for (int y = 0; y < MAP_HEIGHT; ++y)
			{
				if (collisionData[x][y])
				{
					if (computeTile(computeConfiguration(x, y)) == TILE_UNKNOWN)
					{
						collisionData[x][y] = false;
						noneCulled = false;
					}
				}
			}
		}
	} while (!noneCulled);
}

void TileMap::tileify()
{
	for (int x = 0; x < MAP_WIDTH; ++x)
	{
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			if (collisionData[x][y])
				tileData[x][y] = computeTile(computeConfiguration(x, y));
			else
				tileData[x][y] = TILE_0;
		}
	}
}

void TileMap::prepRenderData()
{
	float uDelta = 1.f / float(SPRITES_PER_ROW);
	float vDelta = 1.f / float(SPRITES_PER_COLUMN);
	for (int x = 0; x < MAP_WIDTH; ++x)
	{
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			float xf = float(x);
			float yf = float(y);
			TILE t = tileData[x][y];
			if (t != TILE_0)
			{
				int xc = t % SPRITES_PER_ROW;
				int yc = t / SPRITES_PER_ROW;
				vertexCoords.insert(vertexCoords.end(), {
					xf + 1, yf + 1,
					xf, yf + 1,
					xf, yf,

					xf, yf,
					xf + 1, yf,
					xf + 1, yf + 1
				});
				texCoords.insert(texCoords.end(), {
					uDelta*(xc + 1), vDelta*(yc),
					uDelta*(xc), vDelta*(yc),
					uDelta*(xc), vDelta*(yc + 1),

					uDelta*(xc), vDelta*(yc + 1),
					uDelta*(xc + 1), vDelta*(yc + 1),
					uDelta*(xc + 1), vDelta*(yc),
				});
			}
		}
	}
}

void TileMap::Render(ShaderProgram& program)
{
	program.setModelMatrix(Matrix());
	glBindTexture(GL_TEXTURE_2D, sprites);
	glEnableVertexAttribArray(program.texCoordAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexCoords.data());
	glDrawArrays(GL_TRIANGLES, 0, texCoords.size() / 2);
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);
}