#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <map>
#include "SDL.h"

enum PlayerID
{
	PLAYER_1,
	PLAYER_2
};

enum PlayerInput
{
	NO_INPUT,
	LIGHT_ATTACK,
	STRONG_ATTACK,
	THROW_CATCH,
	JUMP,

	NO_DIRECTION,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class PlayerController
{
public:
	PlayerController();

	void setPlayer(PlayerID id);

	PlayerInput getInput();
	PlayerInput getInputDirection();
	float getInputX();
	float getInputY();

private:
	PlayerInput readInput();
	PlayerInput lastInput;
	std::map<PlayerInput, SDL_Scancode> keymap;
};

#endif // !PLAYER_CONTROLLER_H
