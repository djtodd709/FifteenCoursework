#pragma once

#include <iostream>
using namespace std;

class Puzzle
{
public:
	static const int defaultSize = 4;
	Puzzle(int rows = defaultSize);
	Puzzle(int* setup, int rows);
	~Puzzle();
	bool isTilePresent(int value) const;
	void setNextTile(int value);
	void stream_to(ostream& ostr) const;
	bool isFull() const;
	int getRowSize() const;
	int getNumLimit() const;
	int getConsCombs(int consecLength) const;
	unsigned long long getAnswer(bool includeLastRow) const;
	void getAnswerFacForm(int& prefix, int& factorial, int partSize, bool includeLast) const;
	int numPossibilities(int partSize) const;
protected:
	int* layout;
	int rowSize;
	int numTiles;
	int setTiles;
	int numLimit;
};

class RandomPuzzle : public Puzzle {
public:
	RandomPuzzle(int rows = defaultSize);
};

inline ostream& operator<<(ostream& ostr, const Puzzle& p) {
	p.stream_to(ostr);
	return ostr;
}