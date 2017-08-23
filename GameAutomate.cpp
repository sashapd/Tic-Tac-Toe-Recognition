#include "GameAutomate.h"
#include <iostream>

GameAutomate::GameAutomate(Cell role) : role(role)
{
}

GameState GameAutomate::makeTurn(GameState current_state)
{
	return current_state;
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

int GameAutomate::negamax(GameState state, Cell role, Cell opponent_role) {

	int best_move_score = -9999;
	int score_for_this_move = 0;

	if (state.isOsWin())
		return role == O ? 1000 : -1000;
	if (state.isXsWin())
		return role == X ? 1000 : -1000;

	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			if (state.getCell(r, c) == NONE) {
				state.setCell(r, c, role);
				score_for_this_move = -negamax(state, opponent_role, role);
				state.setCell(r, c, NONE);
				if (score_for_this_move >= best_move_score) {
					best_move_score = score_for_this_move;
				}
			}
		}
	}

	if (best_move_score == -9999 || best_move_score == 0)
		return 0;

	if (best_move_score < 0)
		return best_move_score + 1;
	if (best_move_score > 0)
		return best_move_score - 1;

	return 0;
}

GameState GameAutomate::makeBestMove(GameState game_state)
{
	int best_move_score = -9999;
	int best_move_row = -9999;
	int best_move_col = -9999;
	int score_for_this_move = 0;

	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			if (game_state.getCell(r, c) == NONE) {
				game_state.setCell(r, c, role);
				score_for_this_move = -negamax(game_state, getOpponentRole(), getRole());
				game_state.setCell(r, c, NONE);
				if (score_for_this_move >= best_move_score) {
					best_move_score = score_for_this_move;
					best_move_row = r;
					best_move_col = c;
				}
			}
		}
	}
	game_state.setCell(best_move_row, best_move_col, role);
	return game_state;
}

Cell GameAutomate::getRole()
{
	return role;
}

Cell GameAutomate::getOpponentRole()
{
	return role == O ? X : O;
}

