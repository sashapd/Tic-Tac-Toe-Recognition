#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/core/mat.hpp>
#include "GridExtractor.h"
#include "GameState.h"
#include "GameAutomate.h"

void drawState(GameState state)
{
	std::cout << std::endl;
	for (int r = 0; r < GameState::kRowCount; ++r)
	{
		for (int c = 0; c < GameState::kColumnsCount; ++c)
		{
			if (state.getCell(r, c) == X)
			{
				std::cout << 'x';
			} else if (state.getCell(r, c) == O)
			{
				std::cout << 'o';
			} else
			{
				std::cout << ' ';
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main() {

	GameState game_state;

	drawState(game_state);

	GameAutomate automate(O);

	while (!game_state.isOsWin() && !game_state.isXsWin() && !game_state.isDraw())
	{
		std::cout << "Enter x position (row col): ";
		int row;
		int col;
		std::cin >> row;
		std::cin >> col;
		if (game_state.getCell(row, col) != NONE)
		{
			continue;
		}
		game_state.setCell(row, col, X);
		game_state = automate.makeTurn(game_state);
		drawState(game_state);
	}

	drawState(game_state);
	std::cout << "is draw: " << game_state.isDraw() << std::endl;
	std::cout << "is x's win " << game_state.isXsWin() << std::endl;
	std::cout << "is o's win" << game_state.isOsWin() << std::endl;

	system("Pause");
    return 0;
}
