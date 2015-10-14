#include "ClassDemoApp.h"

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_surface.h>
#include <SDL_image.h>

#include <algorithm>

GLuint ClassDemoApp::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);
	return textureID;
}

ClassDemoApp::ClassDemoApp()
{
	Setup();
	Reset();
}

void ClassDemoApp::Reset()
{
	gameState = STATE_TITLE_SCREEN;
	nextGameState = gameState;
	enemyLeft = true;
	nextEnemyLeft = true;
	timeInState = 0.f;
	player = PlayerEntity(spriteSheet);
	for (int x = 0; x < 12; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			EnemyEntity enemy = EnemyEntity(spriteSheet);
			enemy.rectangle.x = -300.f + x*50.f;
			enemy.rectangle.y = 340.f - y*50.f;
			enemies.push_back(enemy);
		}
	}
}

void ClassDemoApp::Setup()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	program = new ShaderProgram(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
	glViewport(0, 0, 640, 720);
	projectionMatrix.setOrthoProjection(-320, 320, -360, 360, -1.0f, 1.0f);

	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(program->programID);

	gameFont = TextureFont(LoadTexture(RESOURCE_FOLDER "pixel_font.png"), 0);
	spriteSheet = LoadTexture(RESOURCE_FOLDER "sheet.png");
}

ClassDemoApp::~ClassDemoApp()
{
	delete program;
	SDL_Quit();
}

void ClassDemoApp::Render()
{
	switch (gameState)
	{
	case STATE_TITLE_SCREEN:
		RenderTitleScreen();
		break;
	case STATE_GAME:
		RenderGame();
		break;
	case STATE_GAME_OVER:
		RenderGameOver();
		break;
	}
}

void ClassDemoApp::ProcessEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
}

//void ClassDemoApp::CheckKeys()
//{
//	const Uint8* keys = SDL_GetKeyboardState(nullptr);
//	leftMove = 0.f;
//	rightMove = 0.f;
//	if (keys[SDL_SCANCODE_W])
//		leftMove += 1.f;
//	if (keys[SDL_SCANCODE_S])
//		leftMove -= 1.f;
//	if (keys[SDL_SCANCODE_UP])
//		rightMove += 1.f;
//	if (keys[SDL_SCANCODE_DOWN])
//		rightMove -= 1.f;
//}

void ClassDemoApp::Update(float elapsed)
{
	timeInState += elapsed;
	if (nextGameState != gameState)
	{
		timeInState = 0.f;
		gameState = nextGameState;
	}
	switch (gameState)
	{
	case STATE_TITLE_SCREEN:
		UpdateTitleScreen(elapsed);
		break;
	case STATE_GAME:
		UpdateGame(elapsed);
		break;
	case STATE_GAME_OVER:
		UpdateGameOver(elapsed);
		break;
	}
}

bool ClassDemoApp::UpdateAndRender()
{
	float ticks = float(SDL_GetTicks() / 1000.f);
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	ProcessEvents();
	Update(elapsed);
	Render();

	return done;
}

void ClassDemoApp::UpdateTitleScreen(float elapsed)
{
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	if (keys[SDL_SCANCODE_SPACE])
		nextGameState = STATE_GAME;
}

void ClassDemoApp::UpdateGame(float elapsed)
{
	if (nextEnemyLeft != enemyLeft)
	{
		for (EnemyEntity& enemy : enemies)
			enemy.rectangle.y -= 10.f;
		enemyLeft = nextEnemyLeft;
	}
	player.Update(this, elapsed);
	for (BulletEntity& shot : playerShots)
		shot.Update(this, elapsed);
	for (EnemyEntity& enemy : enemies)
		enemy.Update(this, elapsed);
	std::vector<EnemyEntity>::iterator enemyEnd = std::remove_if(enemies.begin(), enemies.end(), [this](const EnemyEntity& enemy)
	{
		for (BulletEntity& shot : playerShots)
		{
			if (overlap(shot.hitbox, enemy.hitbox))
			{
				shot.rectangle.y = 400.f;	// mark bullet for removal
				return true;
			}
		}
		return false;
	});
	std::vector<BulletEntity>::iterator bulletEnd = std::remove_if(playerShots.begin(), playerShots.end(), [](const BulletEntity& shot)
	{
		return shot.rectangle.y > 370.f;
	});
	playerShots.erase(bulletEnd, playerShots.end());
	enemies.erase(enemyEnd, enemies.end());

	if (enemies.empty())
	{
		Reset();
		nextGameState = STATE_GAME;
	}
}

void ClassDemoApp::UpdateGameOver(float elapsed)
{
	if (timeInState > 4.f)
		Reset();
}

void ClassDemoApp::RenderTitleScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
	gameFont.Draw("Space Invaders", 30, -210, 25, program);
	if (fmod(timeInState, 1.5f) < .75)
		gameFont.Draw("press space to continue...", 15, -195, -25, program);
	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::RenderGame()
{
	glClear(GL_COLOR_BUFFER_BIT);
	player.Draw(program);
	for (BulletEntity& shot : playerShots)
		shot.Draw(program);
	for (EnemyEntity& enemy : enemies)
		enemy.Draw(program);
	SDL_GL_SwapWindow(displayWindow);
}

void ClassDemoApp::RenderGameOver()
{
	glClear(GL_COLOR_BUFFER_BIT);
	gameFont.Draw("GAME OVER", 50, -226, -25, program);
	SDL_GL_SwapWindow(displayWindow);
}
