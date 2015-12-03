#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include "RectangleEntity.h"
#include "PlayerController.h"
#include "CollisionBox.h"
#include <vector>

class ClassDemoApp;

enum PlayerState
{
	HIT_STATE,
	NEUTRAL_STATE,
	LIGHT_ATTACK_STATE,
	STRONG_ATTACK_STATE
};

class PlayerEntity : public RectangleEntity
{
public:
	PlayerEntity();

	void setPlayer(PlayerID id) { controller.setPlayer(id); }

	void Update(ClassDemoApp* app, float elapsed);
	void Draw(ShaderProgram* program);
	void CheckCollisions(ClassDemoApp* app);

private:
	void UpdateState(float elapsed);
	void DoAttack(PlayerInput attack, PlayerInput direction);
	void Hurt(PlayerEntity& other);
	void Clash(PlayerEntity& other);

	float lDelay = .2;
	float sDelay = .3;
	float hitstun = .3;

	std::vector<AABB> hitboxes;
	std::vector<AABB> hurtboxes;
	PlayerState state;
	float stateDelay = 0;

	float facing = 1.f;


	PlayerController controller;
};

#endif // !PLAYER_ENTITY_H
