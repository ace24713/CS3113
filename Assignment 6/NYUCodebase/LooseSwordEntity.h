#ifndef LOOSE_SWORD_ENTITY_H
#define LOOSE_SWORD_ENTITY_H

#include "TexturedRectangle.h"
#include "CollisionBox.h"

class ClassDemoApp;

class LooseSwordEntity
	: public TexturedRectangle
{
public:
	LooseSwordEntity();
	void Update(ClassDemoApp* app, float elapsed);

	float velocity_x = 0.f;
	float velocity_y = 0.f;
	float acceleration_x = 0.f;
	float acceleration_y = 0.f;
	float direction = 1.f;
};

#endif // !LOOSE_SWORD_ENTITY_H
