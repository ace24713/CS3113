#include "BulletEntity.h"

#include "Math.h"

BulletEntity::BulletEntity(unsigned int textureID)
	: SheetSpriteEntity(textureID, 1024, 1024, 835, 565, 13, 37)
{
	rectangle.width = 13;
	rectangle.height = 37;
	rectangle.direction_y = 1.f;
	rectangle.speed = 10.f;
}

void BulletEntity::Update(ClassDemoApp * app, float elapsed)
{
	if (rectangle.speed > 10.5f)
		rectangle.speed = std::min(rectangle.speed + 2000.f * elapsed, 2000.f);
	else
		rectangle.speed += 1.f * elapsed;
	rectangle.height = std::max(13.f, 37.f * rectangle.speed / 1000.f);
	rectangle.Advance(elapsed);

	hitbox.x = rectangle.x;
	hitbox.y = rectangle.y;
	hitbox.width = rectangle.width;
	hitbox.height = rectangle.height;
}