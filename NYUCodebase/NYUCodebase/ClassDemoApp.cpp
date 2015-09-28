#include "ClassDemoApp.h"

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>

ClassDemoApp::ClassDemoApp()
{
	Setup();
	Reset();
}

void ClassDemoApp::Reset()
{
	leftPaddle.width = 1.f / 16.f;
	leftPaddle.height = 1.f / 3.f;
	leftPaddle.x = -1.7777f + leftPaddle.width / 2.f;
	leftPaddle.y = 0.f;
	leftPaddle.direction_y = 1.f;
	rightPaddle.width = 1.f / 16.f;
	rightPaddle.height = 1.f / 3.f;
	rightPaddle.x = 1.7777f - rightPaddle.width / 2.f;
	rightPaddle.y = 0.f;
	rightPaddle.direction_y = 1.f;
	ball.width = 1.f / 16.f;
	ball.height = 1.f / 16.f;
	ball.direction_x = 1.f;
	ball.direction_y = 0.f;
	ball.speed = 1.f;
	ball.x = 0.f;
	ball.y = 0.f;
	ball.spin = 0.f;
	ball.rotation = 0.f;
}

void ClassDemoApp::Setup()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	program = new ShaderProgram(RESOURCE_FOLDER "vertex.glsl", RESOURCE_FOLDER "fragment.glsl");
	glViewport(0, 0, 640, 360);
	projectionMatrix.setOrthoProjection(-1.7777f, 1.77777f, -1.0f, 1.0f, -1.0f, 1.0f);

	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);

	glUseProgram(program->programID);
}

ClassDemoApp::~ClassDemoApp()
{
	delete program;
	SDL_Quit();
}

void ClassDemoApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	leftPaddle.Draw(program);
	rightPaddle.Draw(program);
	ball.Draw(program);
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

void ClassDemoApp::CheckKeys()
{
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	leftMove = 0.f;
	rightMove = 0.f;
	if (keys[SDL_SCANCODE_W])
		leftMove += 1.f;
	if (keys[SDL_SCANCODE_S])
		leftMove -= 1.f;
	if (keys[SDL_SCANCODE_UP])
		rightMove += 1.f;
	if (keys[SDL_SCANCODE_DOWN])
		rightMove -= 1.f;
}

void ClassDemoApp::Update(float elapsed)
{
	ball.Advance(elapsed);
	leftPaddle.Advance(elapsed);
	rightPaddle.Advance(elapsed);

	CheckKeys();
	leftPaddle.speed = leftMove * 2.f;
	rightPaddle.speed = rightMove * 2.f;

	if (rightPaddle.y + rightPaddle.height / 2.f > 1.f)
		rightPaddle.y = 1.f - rightPaddle.height / 2.f;
	else if (rightPaddle.y - rightPaddle.height / 2.f < -1.f)
		rightPaddle.y = -1.f + rightPaddle.height / 2.f;

	if (leftPaddle.y + leftPaddle.height / 2.f > 1.f)
		leftPaddle.y = 1.f - leftPaddle.height / 2.f;
	else if (leftPaddle.y - leftPaddle.height / 2.f < -1.f)
		leftPaddle.y = -1.f + leftPaddle.height / 2.f;

	// collision check, left paddle
	if (ball.direction_x < 0.f &&
		ball.x < leftPaddle.x + leftPaddle.width / 2.f &&
		ball.y > leftPaddle.y - leftPaddle.height / 2.f &&
		ball.y < leftPaddle.y + leftPaddle.height / 2.f)
	{
		ball.direction_x = 1.f;
		ball.spin = -4.f * leftPaddle.speed;
	}
	// collision check, right paddle
	else if (ball.direction_x > 0.f &&
			 ball.x > rightPaddle.x - rightPaddle.width / 2.f &&
			 ball.y > rightPaddle.y - rightPaddle.height / 2.f &&
			 ball.y < rightPaddle.y + rightPaddle.height / 2.f)
	{
		ball.direction_x = -1.f;
		ball.spin = 4.f * rightPaddle.speed;
	}


	// top wall check
	if (ball.y > 1.f)
	{
		ball.y = 1.f;
		ball.direction_y = -1.f * abs(ball.direction_y);
	}
	// bottom wall check
	else if (ball.y < -1.f)
	{
		ball.y = -1.f;
		ball.direction_y = 1.f * abs(ball.direction_y);
	}

	ball.direction_y += elapsed * ball.spin / 4.f;

	// check for point scored, no point display though
	if (ball.x < -1.7777f)
	{
		Reset();
		ball.direction_x = 1.f;
	}
	else if (ball.x > 1.7777f)
	{
		Reset();
		ball.direction_x = -1.f;
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