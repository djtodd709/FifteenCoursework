#pragma once
class Puzzle
{
public:
	static const int defaultSize = 4;
	Puzzle(int rows = defaultSize);
	Puzzle(int* setup, int rows);
	~Puzzle();
	bool isTilePresent(int value) const;
	void setNextTile(int value);
	void printPuzzle() const;
	bool isFull() const;
	int getConsCombs() const;
protected:
	int* layout;
	int rowSize;
	int numTiles;
	int setTiles;
};

