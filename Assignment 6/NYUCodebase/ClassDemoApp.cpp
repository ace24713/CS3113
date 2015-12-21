#include "ClassDemoApp.h"

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_surface.h>
#include <SDL_image.h>

#include <algorithm>
#include <ctime>
#include <cstdlib>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

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
}

void ClassDemoApp::Setup()
{
	std::srand(int(std::time(NULL)));
	SDL_Init(SDL_INIT_VIDEO);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	stabWav = Mix_LoadWAV("Stab.wav");
	swingWav = Mix_LoadWAV("Swing.wav");
	throwWav = Mix_LoadWAV("Throw.wav");
	hurtWav = Mix_LoadWAV("Hurt.wav");
	winWav = Mix_LoadWAV("Win.wav");
	clashWav = Mix_LoadWAV("Clash.wav");

	songOgg = Mix_LoadMUS("song.ogg");

	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 800, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	textureProgram = new ShaderProgram(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
	nontextureProgram = new ShaderProgram(RESOURCE_FOLDER "vertex.glsl", RESOURCE_FOLDER "fragment.glsl");
	glViewport(0, 0, 1600, 800);
	projectionMatrix.setOrthoProjection(0, 20, 0, 10, -1.0f, 1.0f);

	textureProgram->setViewMatrix(viewMatrix);
	nontextureProgram->setViewMatrix(viewMatrix);
	textureProgram->setProjectionMatrix(projectionMatrix);
	nontextureProgram->setProjectionMatrix(projectionMatrix);

	textureProgram->setEffectCutoff(0.f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(textureProgram->programID);

	tileSheet = LoadTexture(RESOURCE_FOLDER "platformertiles.png");
	sword = LoadTexture(RESOURCE_FOLDER "sword.png");
	blueCharacter = LoadTexture(RESOURCE_FOLDER "blue_blob.png");
	blueHand = LoadTexture(RESOURCE_FOLDER "blue_hand.png");
	greenCharacter = LoadTexture(RESOURCE_FOLDER "green_blob.png");
	greenHand = LoadTexture(RESOURCE_FOLDER "green_hand.png");
	fontSheet = LoadTexture(RESOURCE_FOLDER "font1.png");
	medal = LoadTexture(RESOURCE_FOLDER "medal.png");
	titleCard = LoadTexture(RESOURCE_FOLDER "title.png");
	blueVictory = LoadTexture(RESOURCE_FOLDER "blue_victory.png");
	greenVictory = LoadTexture(RESOURCE_FOLDER "green_victory.png");

	titleTexture.texture = titleCard;
	blueVictoryTexture.texture = blueVictory;
	greenVictoryTexture.texture = greenVictory;
	SetTitleDimensions(titleTexture);
	SetTitleDimensions(blueVictoryTexture);
	SetTitleDimensions(greenVictoryTexture);

	font = TextureFont(fontSheet, .2f);

	timeLeftOver = 0.f;
	Mix_VolumeMusic(60);
	Mix_PlayMusic(songOgg, -1);

	Reset();
	gameDecided = true;	// in title card
}

void ClassDemoApp::SetTitleDimensions(TexturedRectangle& rectangle)
{
	rectangle.left = 0.f;
	rectangle.lower = 0.f;
	rectangle.right = 20.f;
	rectangle.upper = 10.f;
	rectangle.angle = 0.f;
	rectangle.hMirror = false;
	rectangle.point.x = 0.f;
	rectangle.point.y = 0.f;
}

void ClassDemoApp::Reset()
{
	if (tiles)
		delete tiles;
	tiles = new TileMap();
	gameDecided = false;

	tiles->setSpriteSheet(tileSheet);

	swords.clear();
	players.clear();
	players.resize(2);
	players[0].setPlayer(PLAYER_1);
	players[1].setPlayer(PLAYER_2);
	players[0].x = 3.5f;
	players[0].y = 6.5f;
	players[1].x = 16.5f;
	players[1].y = 6.5f;

	players[0].setCharacter(blueCharacter);
	players[0].setHand(blueHand);
	players[0].setSword(sword);
	players[0].setMedal(medal);
	players[0].setWinned(lastWinner == 0);
	players[1].setCharacter(greenCharacter);
	players[1].setHand(greenHand);
	players[1].setSword(sword);
	players[1].setMedal(medal);
	players[1].setWinned(lastWinner == 1);
	switch (std::rand() % 3)
	{
	case 0:
		tiles->load("map1.txt");
		break;
	case 1:
		tiles->load("map2.txt");
		break;
	case 2:
	default:
		tiles->load("map3.txt");
		break;
	}
}

ClassDemoApp::~ClassDemoApp()
{
	delete textureProgram;
	delete nontextureProgram;
	delete tiles;

	Mix_FreeChunk(swingWav);
	Mix_FreeChunk(stabWav);
	Mix_FreeChunk(throwWav);
	Mix_FreeChunk(hurtWav);
	Mix_FreeChunk(winWav);
	Mix_FreeChunk(clashWav);
	Mix_FreeMusic(songOgg);

	SDL_Quit();
}

void ClassDemoApp::Render()
{
	textureProgram->setViewMatrix(viewMatrix);
	nontextureProgram->setViewMatrix(viewMatrix);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(textureProgram->programID);
	if (title)
	{
		titleTexture.Draw(textureProgram);
	}
	else
	{
		tiles->Render(*textureProgram);
		//glUseProgram(nontextureProgram->programID);
		for (PlayerEntity& player : players)
			player.Draw(textureProgram, nullptr);
		for (LooseSwordEntity& sword : swords)
			sword.Draw(textureProgram);
		if (gameDecided)
		{
			if (lastWinner == 0)
				blueVictoryTexture.Draw(textureProgram);
			else
				greenVictoryTexture.Draw(textureProgram);
		}
	}
	SDL_GL_SwapWindow(displayWindow);
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

void ClassDemoApp::Update(float elapsed)
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (!title)
	{

		float avgx = 0;
		float avgy = 0;
		for (PlayerEntity& player : players)
			player.Update(this, elapsed);
		for (LooseSwordEntity& sword : swords)
			sword.Update(this, elapsed);
		for (PlayerEntity& player : players)
			player.CheckCollisions(this);
		viewMatrix.identity();
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			if (!escProcessed)
			{
				Reset();
				title = true;
				escProcessed = true;
				gameDecided = true;
			}
		}
		else
			escProcessed = false;
	}
	else
	{
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			if (!escProcessed)
				done = true;
		}
		else
			escProcessed = false;
	}

	if (gameDecided && keys[SDL_SCANCODE_SPACE])
	{
		Reset();
		title = false;
	}
}

bool ClassDemoApp::UpdateAndRender()
{
	float ticks = float(SDL_GetTicks() / 1000.f);
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	ProcessEvents();

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS)
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	while (fixedElapsed >= FIXED_TIMESTEP)
	{
		fixedElapsed -= FIXED_TIMESTEP;
		Update(FIXED_TIMESTEP);
	}
	timeLeftOver = fixedElapsed;

	Render();

	return done;
}

void ClassDemoApp::PlayerDied(PlayerEntity& player)
{
	if (!gameDecided)
	{
		gameDecided = true;
		Mix_PlayChannel(-1, winWav, 0);
		int looser = -1;
		for (int i = 0; i < int(players.size()); ++i)
		{
			if (&players[i] == &player)
			{
				looser = i;
				break;
			}
		}
		lastWinner = 1 - looser;
		players[lastWinner].setWinned(true);
	}
}