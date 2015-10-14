#ifndef CLASS_DEMO_APP_H
#define CLASS_DEMO_APP_H

#include "ShaderProgram.h"
#include "RectangleEntity.h"
#include "TextureFont.h"
#include "SheetSpriteEntity.h"
#include "Matrix.h"
#include "PlayerEntity.h"
#include <vector>
#include "BulletEntity.h"
#include "EnemyEntity.h"

struct SDL_Window;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

enum GameState
{
	STATE_TITLE_SCREEN,
	STATE_GAME,
	STATE_GAME_OVER
};

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

	void Reset();

	GLuint LoadTexture(const char* image_path);

	void UpdateTitleScreen(float elapsed);
	void UpdateGame(float elapsed);
	void UpdateGameOver(float elapsed);

	void RenderTitleScreen();
	void RenderGame();
	void RenderGameOver();

	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	Matrix projectionMatrix;
	Matrix viewMatrix;

	ShaderProgram* program;

	GameState nextGameState;
	GameState gameState;

	float timeInState;
	unsigned int spriteSheet;

	TextureFont gameFont;
	PlayerEntity player;
	std::vector<BulletEntity> playerShots;
	std::vector<EnemyEntity> enemies;

	bool enemyLeft;
	bool nextEnemyLeft;
};
#endif // !CLASS_DEMO_APP_H
