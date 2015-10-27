#include "PlayerEntity.h"

#include <algorithm>
#include "SDL.h"
#include "ClassDemoApp.h"
#include "Math.h"

#define EPSILON .000001f

PlayerEntity::PlayerEntity()
{
	
	width = .5f;
	height = .5f;
	acceleration_y = -30.f;
	x = 15.5f;
	y = 15.5f;
}

void PlayerEntity::Update(ClassDemoApp* app, float elapsed)
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_LEFT])
		acceleration_x = -90.f;
	else if (keys[SDL_SCANCODE_RIGHT])
		acceleration_x = 90.f;
	else
		acceleration_x = 0.f;
	velocity_x *= .8f;

	Uint32 mouse = SDL_GetMouseState(NULL, NULL);

	Advance(elapsed);

	TileMap& tiles = app->tiles;

	float xr = width / 2.f;
	float yr = height / 2.f;

	/*
	point locations:

	789
	456
	123
	
	test 1,3 for floor
	test 4,6 for wall
	test 8 for ceil
	test 5 for stuck
	*/

	bool pt4 = tiles.occupied(int(x - xr), int(y));
	bool pt6 = tiles.occupied(int(x + xr), int(y));
	if (pt4)
	{
		x = ceil(x - xr) + xr + EPSILON;
		velocity_x = 0.f;
	}
	if (pt6)
	{
		x = floor(x + xr) - xr - EPSILON;
		velocity_x = 0.f;
	}

	bool pt1 = tiles.occupied(int(x - xr), int(y - yr));
	bool pt3 = tiles.occupied(int(x + xr), int(y - yr));
	bool pt8 = tiles.occupied(int(x), int(y + yr));

	if (velocity_y < 0 && (pt1 || pt3))
	{
		y = ceil(y - yr) + yr + EPSILON;
		velocity_y = 0.f;
		if (keys[SDL_SCANCODE_SPACE])
			velocity_y = 10.f;
	}
	if (pt8)
	{
		y = floor(y + yr) - yr - EPSILON;
		velocity_y = 0.f;
	}

	bool pt5 = tiles.occupied(int(x), int(y));
	bool escaped = keys[SDL_SCANCODE_ESCAPE] == 0;
	if (pt5 || (escaped && !escapedLast))
	{
		velocity_x = 0.f;
		velocity_y = 0.f;
		x = float(rand() % TileMap::MAP_WIDTH) + .5f;
		y = float(rand() % TileMap::MAP_HEIGHT) + .5f;
	}
	escapedLast = escaped;

	float hmax = 10.f;
	float vmax = 20.f;

	if (velocity_x > hmax)
		velocity_x = hmax;
	else if (velocity_x < -hmax)
		velocity_x = -hmax;
	if (velocity_y > vmax)
		velocity_y = vmax;
	else if (velocity_y < -vmax)
		velocity_y = -vmax;

	app->viewMatrix.setPosition(-x, -y, 0.f);
}