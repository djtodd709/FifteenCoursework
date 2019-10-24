//Code by Dave Todd - b9052651

#include <stdlib.h>
#include "Puzzle.h"
#include <time.h>

using namespace std;

Puzzle::Puzzle(int rows) :
rowSize(rows),setTiles(0),numLimit(20){
	numTiles = (rows * rows) - 1;
	layout = new int[numTiles];
	if (numTiles > 20)
		numLimit = numTiles+10;
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

random_device r;
mt19937 gen(r());

RandomPuzzle::RandomPuzzle(int rows):
Puzzle(rows){
	uniform_int_distribution<int> distribution(1, numLimit);
	time_t timer = time(NULL);
	bool* present = new bool[numLimit] {false};
	for (int i = 0; i < numTiles; i++) {
		
		int x = distribution(gen);
		while (present[x-1]) {
			x = distribution(gen);
		}
		setNextTile(x);
		present[x - 1] = true;
	}

	delete[] present;
	present = NULL;
}

bool Puzzle::isTilePresent(int value) const{
	bool present = false;
	for (int i = 0; i < numTiles; i++) {
		present = present | (layout[i] == value);
	}
	return present;
}

int Puzzle::getRowSize() const {
	return rowSize;
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

int Puzzle::numPossibilities(int partSize) const{
	if (partSize > rowSize)
		return 0;
	int possibilities = rowSize - 1;
	while (partSize < rowSize) {
		possibilities += rowSize;
		partSize++;
	}
	return possibilities;
}

void Puzzle::getAnswerFacForm(int& prefix, int& factorial, int partSize, bool includeLast) const {
	int spaces = numTiles - partSize;

	prefix = numPossibilities(partSize) * getConsCombs(partSize);
	factorial = spaces;

	if (includeLast && partSize == rowSize) {
		int combinations = getConsCombs(partSize-1) * (spaces + 1);
		prefix += combinations;
	}
}

int Puzzle::rowConsecs(bool reverse, int partSize) const {
	int result = 0;
	int conStreak = 0;

	int searchFor = 1;
	if (reverse)
		searchFor = -1;

	for (int i = 0; i < numTiles - 1; i++) {
		if ((i + 1) % rowSize == 0) {
			conStreak = 0;
			continue;
		}
		if (layout[i + 1] - layout[i] == searchFor) {
			conStreak++;
			if (conStreak >= partSize - 1) {
				result++;
			}
		}
		else {
			conStreak = 0;
		}
	}

	return result;
}

int Puzzle::colConsecs(bool reverse, int partSize) const {
	int result = 0;
	int conStreak = 0;

	int searchFor = 1;
	if (reverse)
		searchFor = -1;

	for (int i = 0; i < numTiles - 1; i++) {
		int j = rowSize * (i % rowSize) + (i / rowSize);
		if (j + rowSize > numTiles-1) {
			conStreak = 0;
			continue;
		}
		if (layout[j + rowSize] - layout[j] == searchFor) {
			conStreak++;
			if (conStreak >= partSize - 1) {
				result++;
			}
		}
		else {
			conStreak = 0;
		}
	}

	return result;
}