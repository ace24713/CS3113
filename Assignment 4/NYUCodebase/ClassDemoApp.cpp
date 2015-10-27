#include "ClassDemoApp.h"

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_surface.h>
#include <SDL_image.h>

#include <algorithm>

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
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	textureProgram = new ShaderProgram(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
	nontextureProgram = new ShaderProgram(RESOURCE_FOLDER "vertex.glsl", RESOURCE_FOLDER "fragment.glsl");
	glViewport(0, 0, 640, 720);
	projectionMatrix.setOrthoProjection(-8, 8, -9, 9, -1.0f, 1.0f);

	textureProgram->setViewMatrix(viewMatrix);
	nontextureProgram->setViewMatrix(viewMatrix);
	textureProgram->setProjectionMatrix(projectionMatrix);
	nontextureProgram->setProjectionMatrix(projectionMatrix);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(textureProgram->programID);

	tileSheet = LoadTexture(RESOURCE_FOLDER "platformertiles.png");
	tiles.setSpriteSheet(tileSheet);
	tiles.generate();
	timeLeftOver = 0.f;
}

ClassDemoApp::~ClassDemoApp()
{
	delete textureProgram;
	delete nontextureProgram;
	SDL_Quit();
}

void ClassDemoApp::Render()
{
	textureProgram->setViewMatrix(viewMatrix);
	nontextureProgram->setViewMatrix(viewMatrix);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(textureProgram->programID);
	tiles.Render(*textureProgram);
	glUseProgram(nontextureProgram->programID);
	player.Draw(nontextureProgram);
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
	player.Update(this, elapsed);
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