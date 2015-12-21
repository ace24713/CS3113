#include "LooseSwordEntity.h"
#include "Math.h"
#include "ClassDemoApp.h"

#define EPSILON .000001f

LooseSwordEntity::LooseSwordEntity()
{
	left = -.125f;
	right = .125f;
	lower = -1.2f;
	upper = 0.f;
	acceleration_y = -40.f;
}

void LooseSwordEntity::Update(ClassDemoApp * app, float elapsed)
{
	float x_before = point.x;
	float y_before = point.y;
	point.x += velocity_x * elapsed;
	point.y += velocity_y * elapsed;
	velocity_x += acceleration_x * elapsed;
	velocity_y += acceleration_y * elapsed;
	if (velocity_x != 0.f)
		direction = (velocity_x > 0.f ? 1.f : -1.f);
	if (velocity_x == 0 && velocity_y == 0)
		angle = (direction > 0 ? 0.f : radians(180.f));
	else
		angle = toAngle(velocity_x, velocity_y) - radians(90.f);

	velocity_x *= .99f;

	TileMap& tiles = *app->tiles;
	// check horizontal collision
	if (velocity_x != 0.f && tiles.occupied(int(point.x), int(y_before)))
	{
		if (velocity_x > 0.f)
			point.x = floor(point.x) - EPSILON;
		else
			point.x = ceil(point.x) + EPSILON;
		velocity_x = 0.f;
	}
	// check vertical collision
	if (tiles.occupied(int(point.x), int(point.y)))
	{
		if (velocity_y > 0.f)
			point.y = floor(point.y) - EPSILON;
		else
			point.y = ceil(point.y) + EPSILON;
		velocity_y = 0.f;
		velocity_x /= 2.f;
	}
}
