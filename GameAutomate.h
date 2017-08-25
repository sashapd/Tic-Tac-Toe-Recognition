#pragma once
#include "GameState.h"

class GameAutomate
{
public:
	GameState makeTurn(GameState current_state);
	Cell getOpponentRole();
	GameAutomate(Cell role);
private:
	Cell role;
	bool blockOpponentWinningMove(GameState& game_state);
	bool checkForWinningMove(GameState game_state, int startX, int startY, int dx, int dy, Cell type);
	void fillLine(GameState& game_state, int x, int y, int dx, int dy, Cell type);
	bool makeWinningMove(GameState& game_state, Cell type);
	bool makeAutomateWinningMove(GameState& game_state);
	void makeRandomMove(GameState& game_state);
	GameState makeBestMove(GameState game_state);
	Cell getRole();
};
