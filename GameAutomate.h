#pragma once
#include "GameState.h"

class GameAutomate
{
public:
	GameState makeTurn(GameState current_state);
	GameAutomate(Cell role);
private:
	Cell role;
	GameState makeBestMove(GameState game_state);
	int negamax(GameState state, Cell role, Cell opponent_role);
	Cell getRole();
	Cell getOpponentRole();
};
