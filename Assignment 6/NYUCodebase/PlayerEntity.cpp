#include "PlayerEntity.h"

#include <algorithm>
#include "SDL.h"
#include "ClassDemoApp.h"
#include "Math.h"

#define EPSILON .000001f

PlayerEntity::PlayerEntity()
{
	character.point.x = -100;
	character.point.y = -100;
	hand.point.x = -100;
	hand.point.y = -100;
	sword.point.x = -100;
	sword.point.y = -100;
	medal.point.x = -100;
	medal.point.y = -100;

	float scale = 2.f;
	float handSize = scale * .125f;
	float swordHeight = scale * .6f;
	float medalHeight = scale * .2f;
	width = scale * .5f;
	height = scale * .5f;
	acceleration_y = -40.f;
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

	character.lower = -height / 2.f;
	character.upper = height / 2.f;
	character.left = -width / 2.f;
	character.right = width / 2.f;
	hand.lower = width / 8.f;
	hand.upper = hand.lower + handSize;
	hand.left = -handSize / 2.f;
	hand.right = handSize / 2.f;
	sword.left = -handSize / 2.f;
	sword.right = handSize / 2.f;
	sword.lower = width / 8.f;
	sword.upper = sword.lower + swordHeight;
	medal.lower = -height / 4.f;
	medal.upper = medal.lower + medalHeight;
	medal.left = -medalHeight / 4.f;
	medal.right = medalHeight / 4.f;
}

void PlayerEntity::Draw(ShaderProgram* program, ShaderProgram* debug)
{
	if (state == DEAD_STATE)
	{
		program->setEffectCutoff(deadness);
		character.Draw(program);
		program->setEffectCutoff(0.f);
		return;
	}
	character.Draw(program);
	if (winned)
		medal.Draw(program);
	if (hasSword)
		sword.Draw(program);
	hand.Draw(program);
	if (debug)
	{
		glUseProgram(debug->programID);
		debug->setColor(1, 1, 1, .25);
		for (const AABB& rect : hurtboxes)
			rect.debugDraw(debug);
		debug->setColor(1, 0, 0, .25);
		for (const AABB& rect : hitboxes)
			rect.debugDraw(debug);
	}
	glUseProgram(program->programID);
}

void PlayerEntity::Update(ClassDemoApp* app, float elapsed)
{
	if (state == DEAD_STATE)
	{
		deadness += elapsed * 1.5f;
		if (deadness > 1.f)
			deadness = 1.f;
		return;
	}
	float xinput = controller.getInputX();
	if (state == HIT_STATE)
	{
		acceleration_x = xinput * 15.f;
		velocity_x *= .97f;
	}
	else if (state == CATCH_STATE)
	{
		acceleration_x = 0.f;
		velocity_x *= .5f;
	}
	else
	{
		acceleration_x = xinput * 120.f;	
		velocity_x *= .8f;
	}
	PlayerInput input = controller.getInput();

	if (xinput != 0.f)
		facing = xinput;

	Advance(elapsed);

	TileMap& tiles = *app->tiles;

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

	if (velocity_y <= 0 && (pt1 || pt3))
	{
		y = ceil(y - yr) + yr - EPSILON;
		velocity_y = 0.f;
		if (input == JUMP && (state == NEUTRAL_STATE || state == EXAUST_STATE))
			velocity_y = 16.f;
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
	if (state == NEUTRAL_STATE && hasSword && (input == STRONG_ATTACK || input == LIGHT_ATTACK))
		DoAttack(input, controller.getInputDirection());
	else if (state == NEUTRAL_STATE && input == THROW_CATCH)
		DoCatch(app, controller.getInputDirection());

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

	character.point.x = x;
	character.point.y = y;
	sword.point.x = x;
	sword.point.y = y;
	character.hMirror = facing > 0.f;
	sword.hMirror = character.hMirror;
	hand.hMirror = character.hMirror;
	PositionSword();
	hand.point = sword.point;
	hand.angle = sword.angle;
	medal.point.x = character.point.x + (facing > 0.f ? -1.f : 1.f) * width / 4.f;
	medal.point.y = character.point.y;
	medal.hMirror = character.hMirror;
}

void PlayerEntity::PositionSword()
{
	switch (state)
	{
	case NEUTRAL_STATE:
		if (facing > 0.f)
		{
			sword.point.x = x + width / 3.f;
			sword.point.y = y + -height / 2.3f;
			sword.angle = radians(30.f);
		}
		else
		{
			sword.point.x = x + -width / 3.f;
			sword.point.y = y + -height / 2.3f;
			sword.angle = radians(-30.f);
		}
		break;
	case EXAUST_STATE:
	case HIT_STATE:
		if (facing > 0.f)
		{
			sword.point.x = x + width / 2.2f;
			sword.point.y = y + -height / 2.2f;
			sword.angle = radians(45.f);
		}
		else
		{
			sword.point.x = x + -width / 2.2f;
			sword.point.y = y + -height / 2.2f;
			sword.angle = radians(-45.f);
		}
		break;
	case LIGHT_ATTACK_STATE:
	{
		float arcStart = 0.f;
		float arcEnd = 0.f;
		switch (attackDirection)
		{
		case LEFT:
			arcStart = radians(90 + 45);
			arcEnd = radians(90 - 45);
			break;
		case RIGHT:
			arcStart = radians(-90 - 45);
			arcEnd = radians(-90 + 45);
			break;
		case UP:
			arcStart = radians(-45);
			arcEnd = radians(45);
			break;
		case DOWN:
		default:
			arcStart = radians(180 - 45);
			arcEnd = radians(180 + 45);
			break;
		}
		sword.angle = lerp(arcStart, arcEnd, (lDelay - stateDelay) / lDelay);
		break;
	}
	case STRONG_ATTACK_STATE:
		switch (attackDirection)
		{
		case LEFT:
			sword.point.x = x + -width / 4.f;
			sword.angle = radians(90);
			break;
		case RIGHT:
			sword.point.x = x + width / 4.f;
			sword.angle = radians(-90);
			break;
		case UP:
			sword.point.y = y + height / 4.f;
			sword.angle = radians(0);
			break;
		case DOWN:
		default:
			sword.point.y = y + -height / 4.f;
			sword.angle = radians(180);
			break;
		}
		break;
	default:
		break;
	}
}

void PlayerEntity::UpdateState(float elapsed)
{
	if (state != NEUTRAL_STATE)
	{
		stateDelay -= elapsed;
		if (stateDelay <= 0)
		{
			switch (state)
			{
			case LIGHT_ATTACK_STATE:
				state = EXAUST_STATE;
				hitboxes.clear();
				stateDelay = lDelay;
				break;
			case STRONG_ATTACK_STATE:
				state = EXAUST_STATE;
				hitboxes.clear();
				stateDelay = sDelay;
				break;
			case CATCH_STATE:
				state = EXAUST_STATE;
				hitboxes.clear();
				stateDelay = cDelay;
				break;
			default:
				state = NEUTRAL_STATE;
				hitboxes.clear();
				break;
			}
		}
	}
}

void PlayerEntity::DoCatch(ClassDemoApp* app, PlayerInput direction)
{
	if (hasSword)
	{
		hasSword = false;
		LooseSwordEntity loose;
		loose.texture = sword.texture;
		loose.hMirror = sword.hMirror;
		if (direction == NO_DIRECTION)
			direction = (facing > 0.f ? RIGHT : LEFT);
		switch (direction)
		{
		case RIGHT:
			loose.point.x = x + width * 2.f / 3.f;
			loose.point.y = y;
			loose.velocity_x = 15.f;
			loose.velocity_y = 3.f;
			break;
		case LEFT:
			loose.point.x = x - width * 2.f / 3.f;
			loose.point.y = y;
			loose.velocity_x = -15.f;
			loose.velocity_y = 3.f;
			break;
		case UP:
			loose.point.x = x;
			loose.point.y = y + height * 2.f / 3.f;
			loose.velocity_x = 0.f;
			loose.velocity_y = 15.f;
			break;
		case DOWN:
		default:
			loose.point.x = x;
			loose.point.y = y - height * 2.f / 3.f;
			loose.velocity_x = 0.f;
			loose.velocity_y = -15.f;
			break;
		}
		app->swords.push_back(loose);
	}
	else
	{
		state = CATCH_STATE;
		stateDelay = cDelay;
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
		reach = .8f;
		breadth = 1.2f;
		state = LIGHT_ATTACK_STATE;
		stateDelay = lDelay;
		break;
	case STRONG_ATTACK:
	default:
		reach = 1.2f;
		breadth = .2f;
		state = STRONG_ATTACK_STATE;
		stateDelay = sDelay;
		break;
	}

	if (direction == NO_DIRECTION)
		direction = (facing > 0.f ? RIGHT : LEFT);

	attackDirection = direction;

	switch (direction)
	{
	case RIGHT:
		hitbox.offset.x = 1.f;
		hitbox.offset.y = 0.f;
		hitbox.width = reach;
		hitbox.height = breadth;
		break;
	case LEFT:
		hitbox.offset.x = -1.f;
		hitbox.offset.y = 0.f;
		hitbox.width = reach;
		hitbox.height = breadth;
		break;
	case UP:
		hitbox.offset.y = 1.f;
		hitbox.offset.x = 0.f;
		hitbox.height = reach;
		hitbox.width = breadth;
		break;
	case DOWN:
	default:
		hitbox.offset.y = -1.f;
		hitbox.offset.x = 0.f;
		hitbox.height = reach;
		hitbox.width = breadth;
		break;
	}

	hitboxes.push_back(hitbox);
}

void PlayerEntity::CheckCollisions(ClassDemoApp* app)
{
	for (size_t i = 0; i < app->swords.size(); ++i)
	{
		LooseSwordEntity& loose = app->swords[i];
		for (AABB& hitbox : hitboxes)
		{
			if (colliding(loose.point, hitbox))
			{
				Reflect(loose);
				return;
			}
		}
		for (AABB& hurtbox : hurtboxes)
		{
			Point back;
			back.x = loose.point.x + std::cos(loose.angle + radians(90.f)) * 1.2f;
			back.y = loose.point.y + std::sin(loose.angle + radians(90.f)) * 1.2f;
			if (state == CATCH_STATE && (colliding(loose.point, hurtbox) || colliding(back, hurtbox)))
				Catch(app, loose);
			else if (colliding(loose.point, hurtbox))
			{
				Hurt(app, loose);
				return;
			}
		}

	}
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
						Clash(app, player);
						return;
					}
				}
				for (AABB& hurtbox : hurtboxes)
				{
					if (colliding(hitbox, hurtbox))
					{
						Hurt(app, player);
						return;
					}
				}
			}
			for (AABB& hurtbox : player.hurtboxes)
				for (AABB& hurtbox2 : hurtboxes)
					if (colliding(hurtbox, hurtbox2))
						Overlap(player);
		}
	}
}

void PlayerEntity::Clash(ClassDemoApp* app, PlayerEntity& other)
{
	if (!app->sfxProcessed)
	{
		Mix_PlayChannel(-1, app->clashWav, 0);
		app->sfxProcessed = true;
	}
	else
		app->sfxProcessed = false;
	if (state == STRONG_ATTACK_STATE && other.state == LIGHT_ATTACK_STATE)
	{
		hasSword = false;
		LooseSwordEntity loose;
		loose.texture = sword.texture;
		loose.hMirror = sword.hMirror;
		switch (attackDirection)
		{
		case LEFT:
		case RIGHT:
			loose.point.x = x;
			loose.point.y = y + height;
			break;
		case DOWN:
			loose.point.x = x + width;
			loose.point.y = y;
			break;
		case UP:
		default:
			loose.point.x = x - width;
			loose.point.y = y;
			break;
		}
		other.Reflect(loose);
		app->swords.push_back(loose);
		state = NEUTRAL_STATE;
		hitboxes.clear();
	}
	float dx = x - other.x;
	float dy = y - other.y;
	float d = sqrt(dx*dx + dy*dy);
	if (d != 0.f)
	{
		dx /= d;
		dy /= d;
	}
	velocity_x = dx * 14.f;
	velocity_y = dy * 11.f;
}

void PlayerEntity::Overlap(PlayerEntity& other)
{
	float dx = x - other.x;
	if (dx > 0)
		velocity_x += 1.f;
	else if (dx < 0)
		velocity_x -= 1.f;
	else
		velocity_x = (rand() % 2 == 0 ? -1.f : 1.f);
}

void PlayerEntity::Hurt(ClassDemoApp* app, PlayerEntity& other)
{
	if (state != HIT_STATE)
	{
		Mix_PlayChannel(-1, app->hurtWav, 0);
		if (other.state == STRONG_ATTACK_STATE)
		{
			if (state == CATCH_STATE)
			{
				hasSword = true;
				other.hasSword = false;
				other.state = NEUTRAL_STATE;
				other.hitboxes.clear();
				state = EXAUST_STATE;
				stateDelay = cDelay;
			}
			else
			{
				state = DEAD_STATE;
				hitboxes.clear();
				hurtboxes.clear();
				app->PlayerDied(*this);
			}
		}
		else
		{
			state = HIT_STATE;
			stateDelay = hitstun;
			hitboxes.clear();
			float dx = x - other.x;
			float dy = y - other.y;
			float d = sqrt(dx*dx + dy*dy);
			if (d != 0.f)
			{
				dx /= d;
				dy /= d;
			}
			velocity_x = dx * 28.f;
			velocity_y = dy * 22.f + .3f * abs(velocity_x);
		}
	}
}

void PlayerEntity::Reflect(LooseSwordEntity& other)
{
	if (state == LIGHT_ATTACK_STATE)
	{
		float dx = x - other.point.x;
		float dy = y - other.point.y;
		float d = sqrt(dx*dx + dy*dy);
		if (d != 0.f)
		{
			dx /= d;
			dy /= d;
		}
		other.velocity_x = -dx * 28.f;
		other.velocity_y = -dy * 18.f + .7f * abs(other.velocity_x);
	}
}

void PlayerEntity::Catch(ClassDemoApp* app, LooseSwordEntity& other)
{
	hasSword = true;
	state = NEUTRAL_STATE;
	for (auto itr = app->swords.begin(); itr != app->swords.end(); ++itr)
	{
		if (&*itr == &other)
		{
			app->swords.erase(itr);
			break;
		}
	}
}

void PlayerEntity::Hurt(ClassDemoApp* app, LooseSwordEntity& other)
{
	if (state != HIT_STATE && (other.velocity_x*other.velocity_x + other.velocity_y*other.velocity_y > 64.f))
	{
		Mix_PlayChannel(-1, app->hurtWav, 0);
		state = DEAD_STATE;
		hitboxes.clear();
		hurtboxes.clear();
		app->PlayerDied(*this);
	}
}