#ifndef BULLET_ENTITY_H
#define BULLET_ENTITY_H

#include "SheetSpriteEntity.h"
#include "CollisionRectangle.h"

class ClassDemoApp;

class BulletEntity : public SheetSpriteEntity
{
public:
	BulletEntity() {}
	BulletEntity(unsigned int textureID);

	void Update(ClassDemoApp* app, float elapsed);
	CollisionRectangle hitbox;
};

#endif // !BULLET_ENTITY_H
