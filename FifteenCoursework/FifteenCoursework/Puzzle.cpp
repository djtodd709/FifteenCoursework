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

//WILL NOT BE NEEDED IN FINAL
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

void Puzzle::printPuzzle() const{
	int col = 0;
	for (int i = 0; i < setTiles; i++) {
		cout << layout[i] << "\t";
		col++;
		if (col == rowSize) {
			col = 0;
			cout << endl;
		}
	}

	for (int i = setTiles; i < numTiles; i++) {
		cout << "__\t";
		col++;
		if (col == rowSize) {
			col = 0;
			cout << endl;
		}
	}

	cout << endl;
	return;
}