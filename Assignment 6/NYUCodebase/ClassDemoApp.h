#ifndef CLASS_DEMO_APP_H
#define CLASS_DEMO_APP_H

#include "ShaderProgram.h"
#include "RectangleEntity.h"
#include "Matrix.h"
#include "PlayerEntity.h"
#include "TileMap.h"
#include <vector>
#include "TextureFont.h"
#include "LooseSwordEntity.h"
#include <SDL_mixer.h>
#include "TexturedRectangle.h"

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

	void Reset();
	void Setup();
	void ProcessEvents();
	bool UpdateAndRender();

	void Render();
	void Update(float elapsed);

	void PlayerDied(PlayerEntity& player);

	GLuint LoadTexture(const char* image_path);

	void SetTitleDimensions(TexturedRectangle& rectangle);

	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	Matrix projectionMatrix;
	Matrix viewMatrix;

	ShaderProgram* textureProgram;
	ShaderProgram* nontextureProgram;

	unsigned int tileSheet;
	unsigned int sword;
	unsigned int blueCharacter;
	unsigned int blueHand;
	unsigned int greenCharacter;
	unsigned int greenHand;
	unsigned int fontSheet;
	unsigned int medal;

	unsigned int titleCard;
	unsigned int blueVictory;
	unsigned int greenVictory;

	std::vector<PlayerEntity> players;
	std::vector<LooseSwordEntity> swords;
	TileMap* tiles = nullptr;
	TextureFont font;
	TexturedRectangle titleTexture;
	TexturedRectangle blueVictoryTexture;
	TexturedRectangle greenVictoryTexture;

	Mix_Chunk* stabWav;
	Mix_Chunk* swingWav;
	Mix_Chunk* hurtWav;
	Mix_Chunk* throwWav;
	Mix_Chunk* winWav;
	Mix_Chunk* clashWav;

	Mix_Music* songOgg;

	bool sfxProcessed = false;
	int lastWinner = -1;
	bool gameDecided = false;
	bool title = true;
	bool escProcessed = false;

	float timeLeftOver;
};
#endif // !CLASS_DEMO_APP_H
