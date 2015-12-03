#include "PlayerController.h"

#include "SDL.h"
#include <cmath>

PlayerController::PlayerController()
{
	setPlayer(PlayerID::PLAYER_1);
}

void PlayerController::setPlayer(PlayerID id)
{
	keymap.clear();
	switch (id)
	{
	case PLAYER_1:
		keymap[STRONG_ATTACK] = SDL_SCANCODE_I;
		keymap[LIGHT_ATTACK] = SDL_SCANCODE_O;
		keymap[JUMP] = SDL_SCANCODE_P;
		keymap[UP] = SDL_SCANCODE_UP;
		keymap[DOWN] = SDL_SCANCODE_DOWN;
		keymap[LEFT] = SDL_SCANCODE_LEFT;
		keymap[RIGHT] = SDL_SCANCODE_RIGHT;
		break;
	case PLAYER_2:
		keymap[STRONG_ATTACK] = SDL_SCANCODE_Z;
		keymap[LIGHT_ATTACK] = SDL_SCANCODE_X;
		keymap[JUMP] = SDL_SCANCODE_C;
		keymap[UP] = SDL_SCANCODE_W;
		keymap[DOWN] = SDL_SCANCODE_S;
		keymap[LEFT] = SDL_SCANCODE_A;
		keymap[RIGHT] = SDL_SCANCODE_D;
		break;
	}
}

PlayerInput PlayerController::getInput()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (keys[keymap[LIGHT_ATTACK]])
		return LIGHT_ATTACK;
	else if (keys[keymap[STRONG_ATTACK]])
		return STRONG_ATTACK;
	else if (keys[keymap[JUMP]])
		return JUMP;
	return NO_INPUT;
}

PlayerInput PlayerController::getInputDirection()
{
	float xd = getInputX();
	float yd = getInputY();

	if (std::abs(xd) < std::abs(yd))
		return (yd > 0.f ? UP : DOWN);
	else if (xd == 0.f)
		return NO_DIRECTION;
	else
		return (xd > 0.f ? RIGHT : LEFT);
}

float PlayerController::getInputX()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	float v = 0.f;
	if (keys[keymap[LEFT]])
		v -= 1.f;
	if (keys[keymap[RIGHT]])
		v += 1.f;
	return v;
}

float PlayerController::getInputY()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	float v = 0.f;
	if (keys[keymap[DOWN]])
		v -= 1.f;
	if (keys[keymap[UP]])
		v += 1.f;
	return v;
}
