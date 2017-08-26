#include "GameState.h"
#include <algorithm>
#include <vector>

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

std::vector<int> GameState::getCompleteLineFor(Cell type)
{
	for (int r = 0; r < kRowCount; ++r)
		if (checkLine(0, r, 1, 0, type))
			return getLine(0, r, 1, 0);
	for (int c = 0; c < kColumnsCount; ++c)
		if (checkLine(c, 0, 0, 1, type))
			return getLine(c, 0, 0, 1);
	if (checkLine(0, 0, 1, 1, type))
		return getLine(0, 0, 1, 1);
	if (checkLine(2, 0, -1, 1, type))
		return getLine(2, 0, -1, 1);
	std::vector<int> temp;
	return temp;
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

std::vector<int> GameState::getLine(int xStart, int yStart, int dx, int dy)
{
	std::vector<int> result;
	result.push_back(yStart);
	result.push_back(xStart);
	result.push_back(yStart + dy * 2);
	result.push_back(xStart + dx * 2);
	return result;
}
