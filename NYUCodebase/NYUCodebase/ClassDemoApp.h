#ifndef CLASS_DEMO_APP_H
#define CLASS_DEMO_APP_H

#include "ShaderProgram.h"
#include "RectangleEntity.hpp"
#include "Matrix.h"

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

	void Reset();

	void CheckKeys();
private:

	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;

	Matrix projectionMatrix;
	Matrix viewMatrix;

	RectangleEntity leftPaddle;
	RectangleEntity rightPaddle;
	RectangleEntity ball;

	float leftMove;
	float rightMove;

	ShaderProgram* program;
};
#endif // !CLASS_DEMO_APP_H
