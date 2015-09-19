#include "Game.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

/*
draw a 1x1 square centered at zero
*/
void Game::drawSquare()
{
	static const float squareV[] =	{ .5f, .5f
									, -.5f, .5f
									, -.5f, -.5f

									, -.5f, -.5f
									, .5f, -.5f
									, .5f, .5f };

	static const float squareT[] =	{ 1.f, 1.f
									, 0.f, 1.f
									, 0.f, 0.f

									, 0.f, 0.f
									, 1.f, 0.f
									, 1.f, 1.f };


	glVertexAttribPointer(shaders.positionAttribute, 2, GL_FLOAT, false, 0, squareV);
	glVertexAttribPointer(shaders.texCoordAttribute, 2, GL_FLOAT, false, 0, squareT);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

/*
calculate and return the elapsed time, update internal counters
*/
float Game::elapsed()
{
	float ticks = SDL_GetTicks() / 1000.f;
	float diff = ticks - lastTicks;
	lastTicks = ticks;
	return diff;
}

/*
copied from slide with minor modifications
*/
GLuint Game::loadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	return textureID;
}

/*
initialization of shaders, matrix data, etc.
*/
Game::Game()
	: shaders(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl")
{
	glViewport(0, 0, 640, 360);
	projection.setOrthoProjection(-1.7777f, 1.77777f, -1.0f, 1.0f, -1.0f, 1.0f);

	shaders.setViewMatrix(view);
	shaders.setProjectionMatrix(projection);

	glUseProgram(shaders.programID);

	shipTex = loadTexture(RESOURCE_FOLDER "playerShip1_blue.png");
	bigTex = loadTexture(RESOURCE_FOLDER "meteorGrey_big4.png");
	medTex = loadTexture(RESOURCE_FOLDER "meteorGrey_med2.png");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*
drawing code that executes per-frame
*/
void Game::draw()
{
	glEnableVertexAttribArray(shaders.positionAttribute);
	glEnableVertexAttribArray(shaders.texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, medTex);
	shaders.setModelMatrix(medModel);
	drawSquare();
	glBindTexture(GL_TEXTURE_2D, shipTex);
	shaders.setModelMatrix(shipModel);
	drawSquare();
	glBindTexture(GL_TEXTURE_2D, bigTex);
	shaders.setModelMatrix(bigModel);
	drawSquare();
	glDisableVertexAttribArray(shaders.texCoordAttribute);
	glDisableVertexAttribArray(shaders.positionAttribute);
}

/*
per-frame non-drawing code for animations, logic, etc.
*/
void Game::step()
{
	float time = elapsed();

	shipModel.identity();
	shipModel.Translate(shipX, shipY, 0.f);
	shipModel.Translate(shipVariance * random(), shipVariance*random(), 0.f);
	shipModel.Scale(shipScale, shipScale, 1.f);
	shipModel.Rotate(shipRot);

	bigX += bigSpeed * time;
	bigRot += bigSpin * time;
	if (bigX < -3.f)
	{
		bigX += 6.f;
		bigY = (random() * 2.f) - 1.f;
	}
	if (bigRot > toRadians(360.f))
		bigRot -= toRadians(360.f);
	bigModel.identity();
	bigModel.Translate(bigX, bigY, 0.f);
	bigModel.Scale(bigScale, bigScale, 1.f);
	bigModel.Rotate(bigRot);

	medX += medSpeed * time;
	medRot += medSpin * time;
	if (medX < -3.f)
	{
		medX += 6.f;
		medY = (random() * 2.f) - 1.f;
	}
	if (medRot < toRadians(-360.f))
		medRot += toRadians(360.f);
	medModel.identity();
	medModel.Translate(medX, medY, 0.f);
	medModel.Scale(medScale, medScale, 1.f);
	medModel.Rotate(medRot);
}
