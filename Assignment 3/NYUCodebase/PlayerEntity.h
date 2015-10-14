#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include "SheetSpriteEntity.h"
#include "CollisionRectangle.h"

class ClassDemoApp;

class PlayerEntity : public SheetSpriteEntity
{
public:
	PlayerEntity() {}
	PlayerEntity(unsigned int textureID);

	void Update(ClassDemoApp* app, float elapsed);
private:
	CollisionRectangle hitbox;
	float bulletCooldown;
};

#endif // !PLAYER_ENTITY_H
