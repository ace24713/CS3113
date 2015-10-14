#ifndef ENEMY_ENTITY_H
#define ENEMY_ENTITY_H

#include "SheetSpriteEntity.h"
#include "CollisionRectangle.h"

class ClassDemoApp;

class EnemyEntity : public SheetSpriteEntity
{
public:
	EnemyEntity() {}
	EnemyEntity(unsigned int textureID);

	void Update(ClassDemoApp* app, float elapsed);

	CollisionRectangle hitbox;
};

#endif // !ENEMY_ENTITY_H
