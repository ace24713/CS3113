#include "TileMap.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "ShaderProgram.h"
#include <fstream>
#include <sstream>

using namespace std;

void TileMap::load(const std::string& filename)
{
	ifstream infile(filename);
	string line;
	while (getline(infile, line))
	{
		if (line == "[layer]")
			readLayerData(infile);
	}
	prepRenderData();
}

void TileMap::readLayerData(std::ifstream& stream)
{
	string line;
	while (getline(stream, line))
	{
		if (line == "")
			break;
		istringstream sStream(line);
		string key;
		string value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data")
		{
			for (int y = 0; y < MAP_HEIGHT; ++y)
			{
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < MAP_WIDTH; ++x)
				{
					getline(lineStream, tile, ',');
					int val = atoi(tile.c_str());
					switch (val)
					{
					case 0:
					case 4:
					case 12:
					case 14:
					case 15:
					case 16:
					case 20:
					case 22:
					case 23:
					case 24:
						// background tile
						collisionData[x][MAP_HEIGHT - 1 - y] = false;
						break;
					default:
						collisionData[x][MAP_HEIGHT - 1 - y] = true;
						break;
					}
					tileData[x][MAP_HEIGHT - 1 - y] = val;
				}
			}
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
			int t = tileData[x][y];
			if (t != 0)
			{
				--t;
				int xc = t % SPRITES_PER_ROW;
				int yc = SPRITES_PER_COLUMN - 1 - (t / SPRITES_PER_ROW);
				vertexCoords.insert(vertexCoords.end(), {
					xf + 1, yf + 1,
					xf, yf + 1,
					xf, yf,

					xf, yf,
					xf + 1, yf,
					xf + 1, yf + 1
				});
				texCoords.insert(texCoords.end(), {
					uDelta*(xc + 1), vDelta*(yc + 1),
					uDelta*(xc), vDelta*(yc + 1),
					uDelta*(xc), vDelta*(yc),

					uDelta*(xc), vDelta*(yc),
					uDelta*(xc + 1), vDelta*(yc),
					uDelta*(xc + 1), vDelta*(yc + 1),
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