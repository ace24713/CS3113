#include "ShaderProgram.h"
#include "Matrix.h"
#include <cstdlib>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

inline float toRadians(float deg)
{
	return (deg * 3.14159f) / 180.f;
}

inline float random()
{
	return float(rand() % 10000) / float(10000);
}

class Game
{
public:
	Game();

	void draw();
	void step();
private:
	static GLuint loadTexture(const char* path);
	void drawSquare();
	float elapsed();

	ShaderProgram shaders;
	Matrix shipModel;
	Matrix bigModel;
	Matrix medModel;
	Matrix view;
	Matrix projection;

	GLuint shipTex;
	GLuint bigTex;
	GLuint medTex;
	float lastTicks = 0.f;

	float shipRot = toRadians(90.f);
	float shipX = -1.f;
	float shipY = 0.f;
	float shipVariance = .01f;
	float shipScale = .5;

	float bigX = -3.f;
	float bigY = 0.f;
	float bigRot = 0.f;
	float bigScale = .7f;
	float bigSpeed = -1.f;
	float bigSpin = toRadians(15.f);

	float medX = -3.f;
	float medY = 0.f;
	float medRot = 0.f;
	float medScale = .3f;
	float medSpeed = -.8f;
	float medSpin = toRadians(-30.f);
};