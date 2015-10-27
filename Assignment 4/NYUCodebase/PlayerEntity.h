#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include "RectangleEntity.h"

class ClassDemoApp;

class PlayerEntity : public RectangleEntity
{
public:
	PlayerEntity();

	void Update(ClassDemoApp* app, float elapsed);

private:
	bool escapedLast = false;
};

#endif // !PLAYER_ENTITY_H
