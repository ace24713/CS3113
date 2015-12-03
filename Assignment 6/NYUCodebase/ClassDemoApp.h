#ifndef CLASS_DEMO_APP_H
#define CLASS_DEMO_APP_H

#include "ShaderProgram.h"
#include "RectangleEntity.h"
#include "Matrix.h"
#include "PlayerEntity.h"
#include "TileMap.h"
#include <vector>

struct SDL_Window;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class ClassDemoApp
{
public:
	ClassDemoApp();
	~ClassDemoApp();

	void Setup();
	void ProcessEvents();
	bool UpdateAndRender();

	void Render();
	void Update(float elapsed);

	GLuint LoadTexture(const char* image_path);

	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	Matrix projectionMatrix;
	Matrix viewMatrix;

	ShaderProgram* textureProgram;
	ShaderProgram* nontextureProgram;

	unsigned int tileSheet;

	std::vector<PlayerEntity> players;
	TileMap tiles;

	float timeLeftOver;
};
#endif // !CLASS_DEMO_APP_H
