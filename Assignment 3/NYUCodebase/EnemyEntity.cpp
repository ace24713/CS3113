#include "EnemyEntity.h"

#include "ClassDemoApp.h"
#include "Math.h"

EnemyEntity::EnemyEntity(unsigned int textureID)
	: SheetSpriteEntity(textureID, 1024, 1024, 425, 384, 93, 84)
{
	rectangle.width = 46.f;
	rectangle.height = 42.f;
	rectangle.direction_x = 1.f;
	rectangle.rotation = radians(180.f);
	hitbox.width = 25.f;
	hitbox.height = 22.f;
}

void EnemyEntity::Update(ClassDemoApp * app, float elapsed)
{
	float speed = 20.f + (5 * 12 - app->enemies.size())*1.2f;
	if (app->enemyLeft)
		rectangle.speed = speed;
	else
		rectangle.speed = -speed;

	Advance(elapsed);

	if (rectangle.x > 300.f)
		app->nextEnemyLeft = false;
	else if (rectangle.x < -300.f)
		app->nextEnemyLeft = true;

	if (rectangle.y < -250.f)
		app->nextGameState = STATE_GAME_OVER;

	hitbox.x = rectangle.x;
	hitbox.y = rectangle.y;
}
