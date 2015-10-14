#include "PlayerEntity.h"

#include <algorithm>
#include "SDL.h"
#include "ClassDemoApp.h"
#include "Math.h"

PlayerEntity::PlayerEntity(unsigned int textureID)
	: SheetSpriteEntity(textureID, 1024, 1024, 211, 941, 98, 75)
{
	rectangle.width = 65.f;
	rectangle.height = 50.f;
	rectangle.rotation = radians(180.f);
	rectangle.y = -300.f;
	rectangle.direction_x = 1.f;
	hitbox.width = 20.f;
	hitbox.height = 45.f;
	bulletCooldown = .5f;
}

void PlayerEntity::Update(ClassDemoApp* app, float elapsed)
{
	bulletCooldown -= elapsed;

	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	float accel = std::min(elapsed * 6.f, 1.f);
	float deccel = std::min(elapsed * 14.f, 1.f);
	if (keys[SDL_SCANCODE_LEFT])
		rectangle.speed = lerp(rectangle.speed, -1000.f, accel);
	else if (keys[SDL_SCANCODE_RIGHT])
		rectangle.speed = lerp(rectangle.speed, 1000.f, accel);
	rectangle.speed = lerp(rectangle.speed, 0.f, deccel);

	rectangle.Advance(elapsed);
	rectangle.x = clamp(rectangle.x, -287, 287);

	hitbox.x = rectangle.x;
	hitbox.y = rectangle.y;

	if (bulletCooldown < 0.f && keys[SDL_SCANCODE_SPACE])
	{
		bulletCooldown = .5f;
		BulletEntity shot = BulletEntity(textureID);
		shot.rectangle.x = rectangle.x;
		shot.rectangle.y = rectangle.y + rectangle.height;
		app->playerShots.push_back(shot);
	}

	//app->nextGameState = STATE_GAME_OVER;
}