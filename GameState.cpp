#include "GameState.h"
#include <algorithm>

Cell GameState::getCell(const int row, const int column)
{
	return mCells[row][column];
}

void GameState::setCell(const int row, const int column, const Cell cell)
{
	mCells[row][column] = cell;
}

GameState::GameState()
{
	for (int r = 0; r < kRowCount; ++r)
		for (int c = 0; c < kColumnsCount; ++c)
			mCells[r][c] = NONE;
}

GameState::GameState(const Cell cells[3][3])
{
	for (int r = 0; r < kRowCount; ++r)
		for (int c = 0; c < kColumnsCount; ++c)
			mCells[r][c] = cells[r][c];
}

bool GameState::isDraw() const
{
	for (int r = 0; r < kRowCount; ++r)
		for (int c = 0; c < kColumnsCount; ++c)
			if (mCells[r][c] == NONE)
				return false;
	return true;
}


bool GameState::isOsWin() const
{
	return checkWin(O);
}

bool GameState::isXsWin() const
{
	return checkWin(X);
}

int GameState::getCellCountOfType(Cell type) const
{
	int result = 0;
	for (int r = 0; r < kRowCount; ++r)
		for (int c = 0; c < kColumnsCount; ++c)
			if (mCells[r][c] == type)
				result++;
	return result;
}

bool GameState::checkWin(Cell type) const
{
	for (int r = 0; r < kRowCount; ++r)
		if (checkLine(0, r, 1, 0, type))
			return true;
	for (int c = 0; c < kColumnsCount; ++c)
		if (checkLine(c, 0, 0, 1, type))
			return true;
	return checkLine(0, 0, 1, 1, type) || checkLine(2, 0, -1, 1, type);
}

bool GameState::checkLine(const int x, const int y, const int dx, const int dy, const Cell type) const
{
	for (int i = 0; i < 3; ++i)
		if (mCells[y + dy * i][x + dx * i] != type)
			return false;
	return true;
}
