#ifndef TICTACTOERECOGNITION_GAMESTATE_H
#define TICTACTOERECOGNITION_GAMESTATE_H
#include "Cell.h"
#include <vector>

class GameState
{
public:
	const static int kRowCount = 3;
	const static int kColumnsCount = 3;
	Cell getCell(const int row, const int column);
	void setCell(const int row, const int column, const Cell cell);
	GameState();
	GameState(const Cell [kRowCount][kColumnsCount]);
	bool isOsWin() const;
	bool isXsWin() const;
	bool isDraw() const;
	int getCellCountOfType(Cell type) const;
	std::vector<int> getCompleteLineFor(Cell type);
private:
	Cell mCells[kRowCount][kColumnsCount];
	bool checkWin(const Cell type) const;
	bool checkLine(const int x, const int y, const int dx, const int dy, const Cell type) const;
	std::vector<int> getLine(int xStart, int yStart, int dx, int dy);
};

#endif // TICTACTOERECOGNITION_GAMESTATE_H
