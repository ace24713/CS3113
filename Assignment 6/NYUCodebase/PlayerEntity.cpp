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

	AABB box;
	box.center.x = x;
	box.center.y = y;
	box.offset.x = 0;
	box.offset.y = 0;
	box.width = width;
	box.height = height;
	hurtboxes.push_back(box);

	state = NEUTRAL_STATE;
}

void PlayerEntity::Draw(ShaderProgram* program)
{
	program->setColor(1, 1, 1);
	for (const AABB& rect : hurtboxes)
		rect.debugDraw(program);
	program->setColor(1, 0, 0);
	for (const AABB& rect : hitboxes)
		rect.debugDraw(program);
}

void PlayerEntity::Update(ClassDemoApp* app, float elapsed)
{
	float xinput = controller.getInputX();
	if (state == HIT_STATE)
	{
		acceleration_x = xinput * 10.f;
		velocity_x *= .9;
	}
	else
	{
		acceleration_x = xinput * 90.f;
		velocity_x *= .8f;
	}
	PlayerInput input = controller.getInput();

	if (xinput != 0.f)
		facing = xinput;

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
		if (input == JUMP && state == NEUTRAL_STATE)
			velocity_y = 10.f;
	}
	if (pt8)
	{
		y = floor(y + yr) - yr - EPSILON;
		velocity_y = 0.f;
	}

	//bool pt5 = tiles.occupied(int(x), int(y));
	//if (pt5)
	//{
	//	velocity_x = 0.f;
	//	velocity_y = 0.f;
	//	x = float(rand() % TileMap::MAP_WIDTH) + .5f;
	//	y = float(rand() % TileMap::MAP_HEIGHT) + .5f;
	//}

	float hmax = 40.f;
	float vmax = 40.f;

	if (velocity_x > hmax)
		velocity_x = hmax;
	else if (velocity_x < -hmax)
		velocity_x = -hmax;
	if (velocity_y > vmax)
		velocity_y = vmax;
	else if (velocity_y < -vmax)
		velocity_y = -vmax;

	UpdateState(elapsed);
	if (state == NEUTRAL_STATE && (input == STRONG_ATTACK || input == LIGHT_ATTACK))
		DoAttack(input, controller.getInputDirection());

	for (AABB& box : hurtboxes)
	{
		box.center.x = x;
		box.center.y = y;
	}
	for (AABB& box : hitboxes)
	{
		box.center.x = x;
		box.center.y = y;
	}
}

void PlayerEntity::UpdateState(float elapsed)
{
	if (state != NEUTRAL_STATE)
	{
		stateDelay -= elapsed;
		if (stateDelay <= 0)
		{
			state = NEUTRAL_STATE;
			hitboxes.clear();
		}
	}
}

void PlayerEntity::DoAttack(PlayerInput attack, PlayerInput direction)
{
	hitboxes.clear();
	AABB hitbox;
	float reach;
	float breadth;
	switch (attack)
	{
	case LIGHT_ATTACK:
		reach = .4;
		breadth = .6;
		state = LIGHT_ATTACK_STATE;
		stateDelay = lDelay;
		break;
	case STRONG_ATTACK:
	default:
		reach = .6;
		breadth = .1;
		state = STRONG_ATTACK_STATE;
		stateDelay = sDelay;
		break;
	}

	if (direction == NO_DIRECTION)
		direction = (facing > 0.f ? RIGHT : LEFT);

	switch (direction)
	{
	case RIGHT:
		hitbox.offset.x = .5f;
		hitbox.offset.y = 0.f;
		hitbox.width = reach;
		hitbox.height = breadth;
		break;
	case LEFT:
		hitbox.offset.x = -.5f;
		hitbox.offset.y = 0.f;
		hitbox.width = reach;
		hitbox.height = breadth;
		break;
	case UP:
		hitbox.offset.y = .5f;
		hitbox.offset.x = 0.f;
		hitbox.height = reach;
		hitbox.width = breadth;
		break;
	case DOWN:
	default:
		hitbox.offset.y = -.5f;
		hitbox.offset.x = 0.f;
		hitbox.height = reach;
		hitbox.width = breadth;
		break;
	}

	hitboxes.push_back(hitbox);
}

void PlayerEntity::CheckCollisions(ClassDemoApp* app)
{
	for (PlayerEntity& player : app->players)
	{
		if (&player != this)
		{
			for (AABB& hitbox : player.hitboxes)
			{
				for (AABB& hitbox2 : hitboxes)
				{
					if (colliding(hitbox, hitbox2))
					{
						Clash(player);
						return;
					}
				}
				for (AABB& hurtbox : hurtboxes)
				{
					if (colliding(hitbox, hurtbox))
					{
						Hurt(player);
						return;
					}
				}
			}
		}
	}
}

void PlayerEntity::Clash(PlayerEntity& other)
{
	float dx = x - other.x;
	float dy = y - other.y;
	float d = sqrt(dx*dx + dy*dy);
	dx /= d;
	dy /= d;
	velocity_x = dx * 10.f;
	velocity_y = dy * 10.f;
}

void PlayerEntity::Hurt(PlayerEntity& other)
{
	state = HIT_STATE;
	stateDelay = hitstun;
	hitboxes.clear();
	float dx = x - other.x;
	float dy = y - other.y;
	float d = sqrt(dx*dx + dy*dy);
	dx /= d;
	dy /= d;
	velocity_x = dx * 20.f;
	velocity_y = dy * 20.f;
}