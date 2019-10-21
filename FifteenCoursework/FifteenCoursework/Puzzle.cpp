#include <iostream>
#include <stdlib.h>
#include "Puzzle.h"

using namespace std;

Puzzle::Puzzle(int rows) :
rowSize(rows),setTiles(0),numLimit(20){
	numTiles = (rows * rows) - 1;
	layout = new int[numTiles];
	if (numTiles > 20)
		numLimit = numTiles;
}

Puzzle::Puzzle(int* setup, int rows) :
Puzzle(rows) {
	for (int i = 0; i < numTiles; i++) {
		layout[i] = setup[i];
	}
	setTiles = numTiles;
}

Puzzle::~Puzzle() {
	delete[] layout;
}

RandomPuzzle::RandomPuzzle(int rows):
Puzzle(rows){
	for (int i = 0; i < numTiles; i++) {
		int x = rand() % numLimit + 1;
		while (isTilePresent(x)) {
			x = rand() % numLimit + 1;
		}
		setNextTile(x);
	}
}

bool Puzzle::isTilePresent(int value) const{
	bool present = false;
	for (int i = 0; i < numTiles; i++) {
		present = present | (layout[i] == value);
	}
	return present;
}

int Puzzle::getNumLimit() const {
	return numLimit;
}

void Puzzle::setNextTile(int value) {
	if (setTiles < numTiles) {
		layout[setTiles] = value;
		setTiles++;
	}
}

bool Puzzle::isFull() const{
	return setTiles == numTiles;
}

void Puzzle::stream_to(std::ostream& ostr) const {
	int col = 0;
	for (int i = 0; i < setTiles; i++) {
		ostr << layout[i] << "\t";
		col++;
		if (col == rowSize) {
			col = 0;
			ostr << endl;
		}
	}

	for (int i = setTiles; i < numTiles; i++) {
		ostr << "__\t";
		col++;
		if (col == rowSize) {
			col = 0;
			ostr << endl;
		}
	}

	ostr << endl;
}

int compare(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}

int Puzzle::getConsCombs(int consecLength) const{
	int* layoutCopy = new int[numTiles];
	copy(layout, layout + numTiles, layoutCopy);
	qsort(layoutCopy, numTiles, sizeof(int), compare);
	int consecutives = 0;
	int conStreak = 0;
	for (int i = 1; i < numTiles; i++) {
		if (layoutCopy[i] - 1 == layoutCopy[i - 1]) {
			conStreak++;
			if (conStreak >= consecLength - 1) {
				consecutives++;
			}
		}
		else {
			conStreak = 0;
		}
	}
	delete[] layoutCopy;
	return consecutives;
}

unsigned long long fac(int f) {
	if (f == 1) {
		return 1;
	}
	return f * fac(f - 1);
}

unsigned long long Puzzle::getAnswer(bool includeLastRow) const{
	unsigned long long answer = ((unsigned long long)rowSize - 1) * getConsCombs(rowSize) * fac(numTiles-rowSize) / 2;
	if (includeLastRow) {
		answer += getConsCombs(rowSize-1) * fac(numTiles + 1 - rowSize) / 2;
	}
	return answer;
}
