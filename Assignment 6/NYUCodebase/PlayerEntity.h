#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include "RectangleEntity.h"
#include "PlayerController.h"
#include "CollisionBox.h"
#include <vector>
#include "TexturedRectangle.h"
#include "LooseSwordEntity.h"

class ClassDemoApp;

enum PlayerState
{
	HIT_STATE,
	NEUTRAL_STATE,
	EXAUST_STATE,
	LIGHT_ATTACK_STATE,
	STRONG_ATTACK_STATE,
	CATCH_STATE,
	DEAD_STATE
};

class PlayerEntity : public RectangleEntity
{
public:
	PlayerEntity();

	void setPlayer(PlayerID id) { controller.setPlayer(id); }

	void Update(ClassDemoApp* app, float elapsed);
	void Draw(ShaderProgram* program, ShaderProgram* debug = nullptr);
	void CheckCollisions(ClassDemoApp* app);
	void setSword(unsigned texture) { sword.texture = texture; }
	void setHand(unsigned texture) { hand.texture = texture; }
	void setCharacter(unsigned texture) { character.texture = texture; }
	void setMedal(unsigned texture) { medal.texture = texture; }
	void setWinned(bool win) { winned = win; }

private:
	void UpdateState(float elapsed);
	void DoCatch(ClassDemoApp* app, PlayerInput direction);
	void DoAttack(PlayerInput attack, PlayerInput direction);
	void Overlap(PlayerEntity& other);
	void Hurt(ClassDemoApp* app, PlayerEntity& other);
	void Clash(ClassDemoApp* app, PlayerEntity& other);
	void PositionSword();
	void Reflect(LooseSwordEntity& other);
	void Catch(ClassDemoApp* app, LooseSwordEntity& other);
	void Hurt(ClassDemoApp* app, LooseSwordEntity& other);

	float cDelay = .3f;
	float lDelay = .2f;
	float sDelay = .3f;
	float hitstun = .3f;

	float deadness = 0.f;

	std::vector<AABB> hitboxes;
	std::vector<AABB> hurtboxes;
	PlayerState state;
	PlayerInput attackDirection;
	float stateDelay = 0;
	bool hasSword = true;
	bool winned = false;

	float facing = 1.f;

	TexturedRectangle hand;
	TexturedRectangle sword;
	TexturedRectangle character;
	TexturedRectangle medal;

	PlayerController controller;
};

#endif // !PLAYER_ENTITY_H
