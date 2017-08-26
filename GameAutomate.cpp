#include "GameAutomate.h"
#include <iostream>

GameAutomate::GameAutomate(Cell role) : role(role) {}

GameState GameAutomate::makeTurn(GameState current_state)
{
	if (current_state.isOsWin()
		|| current_state.isXsWin()
		|| current_state.isDraw())
	{
		return current_state;
	}

	int moves_left =
		current_state.getCellCountOfType(role == X ? O : X)
		+ (role == X ? 1 : 0)
		- current_state.getCellCountOfType(role);

	if (moves_left != 1)
	{
		std::cout << "Move count: " << moves_left << std::endl;
		return current_state;
	}

	return makeBestMove(current_state);
}

Cell GameAutomate::getOpponentRole()
{
	return role == O ? X : O;
}

bool GameAutomate::blockOpponentWinningMove(GameState& game_state)
{
	return makeWinningMove(game_state, getOpponentRole());
}

bool GameAutomate::checkForWinningMove(GameState game_state, int startX, int startY, int dx, int dy, Cell type)
{
	int count = 0;
	for (int i = 0; i < 3; ++i)
	{
		auto cell = game_state.getCell(startY + dy * i, startX + dx * i);
		if (cell == (type == O ? X : O))
			return false;
		if (cell == NONE)
			count++;
	}
	return count == 1;
}

void GameAutomate::fillLine(GameState& game_state, int x, int y, int dx, int dy, Cell type)
{
	for (int i = 0; i < 3; ++i)
	{
		auto cell = game_state.getCell(y + dy * i, x + dx * i);
		if (cell == NONE)
		{
			game_state.setCell(y + dy * i, x + dx * i, role);
		}
	}
}

bool GameAutomate::makeWinningMove(GameState& game_state, Cell type)
{
	for (int r = 0; r < GameState::kRowCount; ++r)
		if (checkForWinningMove(game_state, 0, r, 1, 0, type))
		{
			fillLine(game_state, 0, r, 1, 0, type);
			return true;
		}
	for (int c = 0; c < GameState::kColumnsCount; ++c)
		if (checkForWinningMove(game_state, c, 0, 0, 1, type))
		{
			fillLine(game_state, c, 0, 0, 1, type);
			return true;
		}
	if (checkForWinningMove(game_state, 0, 0, 1, 1, type))
	{
		fillLine(game_state, 0, 0, 1, 1, type);
		return true;
	}
	if (checkForWinningMove(game_state, 2, 0, -1, 1, type))
	{
		fillLine(game_state, 2, 0, -1, 1, type);
		return true;
	}
	return false;
}

bool GameAutomate::makeAutomateWinningMove(GameState& game_state)
{
	return makeWinningMove(game_state, role);
}

void GameAutomate::makeRandomMove(GameState& game_state)
{
	int pos = 1;
	int seed = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			Cell cell = game_state.getCell(i, j);
			int val = pos * pos;
			seed += val * (cell == O ? 1 : cell == X ? -1 : 0);
		}
	srand(seed);
	int skip = rand() % 9;
	while (true)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (skip-- <= 0 && game_state.getCell(i, j) == NONE)
				{
					game_state.setCell(j, i, role);
					return;
				}
	}
}

GameState GameAutomate::makeBestMove(GameState game_state)
{
	if (!makeAutomateWinningMove(game_state) && !blockOpponentWinningMove(game_state))
	{
		makeRandomMove(game_state);
	}
	return game_state;
}

Cell GameAutomate::getRole()
{
	return role;
}
