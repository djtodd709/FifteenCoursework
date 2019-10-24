// FifteenCoursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Code by Dave Todd - b9052651

#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <thread>
#include <mutex>
#include "Puzzle.h"

using namespace std;

void getInputNum(int& x, int min, int max) {
	bool passed = false;
	while (!passed) {
		cin >> x;
		
		if (!cin) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "That's not a valid number: ";
			continue;
		}

		if (!(cin.peek() == '\n')) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "That's not a valid number: ";
			continue;
		}

		if ((x > max) | (x < min)) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << x << " is not within the allowed number range: ";
			continue;
		}
		
		passed = true;
	}
}

void fillPuzzle(Puzzle*& p) {
	system("CLS");
	cout << "Manually Create Configuration" << endl;
	cout << *p;

	int x;
	cout << "Please enter a number for the next tile: ";
	getInputNum(x, 1, p->getNumLimit());
	while (p->isTilePresent(x)) {
		cout << x << " is already present in the square: ";
		getInputNum(x, 1, p->getNumLimit());
	}
	p->setNextTile(x);
}

void extractRow(int* const & row, string line) {
	stringstream ss(line);
	int index = 0;
	for (int i = 0; ss >> i;) {
		row[index] = i;
		index++;
	}
}

Puzzle** openFile(int& numPuzzles) {
	string line;
	ifstream infile("15-File.txt");
	if (!infile) {
		cout << "Could not open the 15-file. Make sure a 15-file already exists." << endl;
		return NULL;
	}

	getline(infile, line);
	numPuzzles = stoi(line);

	Puzzle** puzzleList = new Puzzle * [numPuzzles];

	for (int i = 0; i < numPuzzles; i++) {
		getline(infile, line);
		int rowSize = count(line.begin(), line.end(), '\t');
		int squareSize = rowSize * rowSize - 1;
		if (squareSize < 8) {
			return NULL;
		}
		int* square = new int[squareSize];
		
		extractRow(square, line);

		for (int j = 1; j < rowSize; j++) {
			getline(infile, line);
			extractRow(square+(j*rowSize), line);
		}

		Puzzle* p = new Puzzle(square, rowSize);
		puzzleList[i] = p;
		getline(infile, line);
	}

	infile.close();
	return puzzleList;
}

void outputAnalysis(ostream& dest, Puzzle** pList, int numPuzzles, int partialSize, bool freeSquare) {
	dest << numPuzzles << endl;

	for (int i = 0; i < numPuzzles; i++) {
		Puzzle* p = pList[i];
		dest << *p;

		int prefix;
		int suffix;
		p->getAnswerFacForm(prefix, suffix, partialSize, freeSquare);
		
		dest << "Number of " << partialSize << "-partial continuous rows/columns, for all valid turns" << endl;
		if (prefix > 0) {
			if (prefix % 2 == 1) {
				dest << "row = " << prefix / 2 << ".5 x " << suffix << "!" << endl;
				dest << "column = " << prefix / 2 << ".5 x " << suffix << "!" << endl;
				dest << "reverse row = " << prefix / 2 << ".5 x " << suffix << "!" << endl;
				dest << "reverse column = " << prefix / 2 << ".5 x " << suffix << "!" << endl;
			}
			else {
				dest << "row = " << prefix / 2 << " x " << suffix << "!" << endl;
				dest << "column = " << prefix / 2 << " x " << suffix << "!" << endl;
				dest << "reverse row = " << prefix / 2 << " x " << suffix << "!" << endl;
				dest << "reverse column = " << prefix / 2 << " x " << suffix << "!" << endl;
			}
		}
		else {
			dest << "row = 0" << endl;
			dest << "column = 0" << endl;
			dest << "reverse row = 0" << endl;
			dest << "reverse column = 0" << endl;
		}

		dest << "Total for row & column, including reverse, in this configuration" << endl;
		for (int j = 2; j <= p->getRowSize(); j++) {
			dest << j << " = " << p->rowConsecs(false, j) + p->colConsecs(false, j) + p->rowConsecs(true, j) + p->colConsecs(true, j) << endl;
		}

		dest << "Total for row & column, including reverse, for all valid turns" << endl;
		for (int j = 2; j <= p->getRowSize(); j++) {
			p->getAnswerFacForm(prefix, suffix, j, false);
			dest << j << " = " << prefix * 2 << " x " << suffix << "!" << endl;
		}
		p->getAnswerFacForm(prefix, suffix, p->getRowSize(), true);
		dest << p->getRowSize() << "(free square included) = " << prefix * 2 << " x " << suffix << "!" << endl;

		dest << endl;

		delete p;
		p = NULL;
	}
}

void saveSolution(int partialSize, bool freeSquare) {
	ofstream outfile("Solution-File.txt");
	if (!outfile) {
		cout << "Could not open the solution-file." << endl;
		return;
	}

	int numPuzzles;
	Puzzle** pList = openFile(numPuzzles);

	outputAnalysis(outfile, pList, numPuzzles, partialSize, freeSquare);

	delete[] pList;
	pList = NULL;

	outfile.close();
}

void overwriteFile(Puzzle** puzzleList, int numPuzzles) {
	ofstream outfile("15-File.txt");
	if (!outfile) {
		cout << "Could not open the 15-file." << endl;
		return;
	}

	outfile << numPuzzles << endl;

	for (int i = 0; i < numPuzzles; i++) {
		outfile << *(puzzleList[i]) << endl;
	}

	outfile.close();
}

void appendFile(Puzzle** puzzleList, int numPuzzles) {
	int oldNumPuzzles;
	Puzzle** oldPuzzleList = openFile(oldNumPuzzles);

	if (oldPuzzleList == NULL) {
		cout << "Could not find the 15-file. Creating new 15-File..." << endl;
		overwriteFile(puzzleList, numPuzzles);
		return;
	}

	ofstream outfile("15-File.txt");
	if (!outfile) {
		cout << "Could not open the 15-file." << endl;
		return;
	}

	int newNumPuzzles = numPuzzles + oldNumPuzzles;

	outfile << newNumPuzzles << endl;

	for (int i = 0; i < oldNumPuzzles; i++) {
		Puzzle* p = oldPuzzleList[i];
		outfile << *p << endl;
		delete p;
		p = NULL;
	}

	delete[] oldPuzzleList;
	oldPuzzleList = NULL;

	for (int i = 0; i < numPuzzles; i++) {
		outfile << *(puzzleList[i]) << endl;
	}

	outfile.close();

}

void fileChoice(Puzzle** puzzleList, int numPuzzles) {
	int choice;
	getInputNum(choice, 1, 3);
	switch (choice) {
	case 1:
		overwriteFile(puzzleList, numPuzzles);
		system("CLS");
		cout << "15-File successfully overwritten" << endl << endl;
		break;
	case 2:
		appendFile(puzzleList, numPuzzles);
		system("CLS");
		cout << "15-File successfully appended" << endl << endl;
		break;
	case 3:
		system("CLS");
		cout << "Nothing has been added to the 15-File" << endl << endl;
		break;
	}
}

void makeManualPuzzle() {
	cout << "How many rows/columns will your puzzle have?: ";
	int psize = 4;
	getInputNum(psize, 3, INT_MAX);
	Puzzle* p = new Puzzle(psize);

	while (!p->isFull()) {
		fillPuzzle(p);
	}

	system("CLS");
	cout << "Manually Create Configuration" << endl;
	cout << *p;

	cout << endl << "(1) Overite 15-File" << endl;
	cout << "(2) Append to 15-File" << endl;
	cout << "(3) Do not add to 15-File" << endl;
	cout << "What would you like to do with this configuration?: ";
	fileChoice(&p,1);

	delete p;
	p = NULL;

}

struct threadPack {
	int rows;
	int index;
	Puzzle** randPuzzles;
	mutex* mm;
};

void addRandPuzzle(threadPack tp) {
	Puzzle* p = new RandomPuzzle(tp.rows);
	lock_guard<mutex>guard(*(tp.mm));
	tp.randPuzzles[tp.index] = p;
}

void makeRandomPuzzle() {
	system("CLS");
	cout << "Generate Random Configurations" << endl;
	cout << "How many rows/columns will your puzzles have?: ";
	int psize = 4;
	getInputNum(psize, 3, INT_MAX);
	cout << "How many configurations would you like to generate?: " << endl;
	int numPuzzles;
	getInputNum(numPuzzles, 1, INT_MAX);
	cout << "(1) Yes" << endl;
	cout << "(2) No" << endl;
	cout << "Would you like to see the puzzles after generation?: " << endl;
	int showChoice;
	getInputNum(showChoice, 1, INT_MAX);

	clock_t timer = clock();

	Puzzle** randPuzzles = new Puzzle*[numPuzzles];

	if (psize <= 500) {
		for (int i = 0; i < numPuzzles; i++) {
			Puzzle* p = new RandomPuzzle(psize);

			if (showChoice == 1)
				cout << *p << endl;

			randPuzzles[i] = p;
		}
	}
	else {

		thread* t = new thread[numPuzzles];
		mutex mMutex;

		for (int i = 0; i < numPuzzles; i++) {
			threadPack tp;
			tp.index = i;
			tp.mm = &mMutex;
			tp.randPuzzles = randPuzzles;
			tp.rows = psize;
			t[i] = thread(addRandPuzzle, tp);
		}

		for (int i = 0; i < numPuzzles; i++) {
			t[i].join();
			if (showChoice == 1)
				cout << *randPuzzles[i] << endl;
		}
		
		delete[] t;
		t = NULL;
	}

	double diff = double(clock() - timer) / double(CLOCKS_PER_SEC);
	cout << "Generation finished in " << diff << " seconds" << endl;

	cout << endl << "(1) Overite 15-File" << endl;
	cout << "(2) Append to 15-File" << endl;
	cout << "(3) Do not add to 15-File" << endl;
	cout << "What would you like to do with this configuration set?: ";
	fileChoice(randPuzzles,numPuzzles);

	for (int i = 0; i < numPuzzles; i++) {
		delete randPuzzles[i];
		randPuzzles[i] = NULL;
	}
	delete [] randPuzzles;
	randPuzzles = NULL;
}

void readPuzzles() {
	system("CLS");

	int numPuzzles;
	Puzzle** pList = openFile(numPuzzles);

	if (pList == NULL) {
		cout << "You do not currently have a 15-File, or the file is invalid." << endl;
		cout << "Please create a valid file using the other menu options first." << endl << endl;
		return;
	}

	bool freeSquare = true;

	cout << "What partial size would you like to use?: ";
	int partialSize;
	getInputNum(partialSize, 2, INT_MAX);

	cout << "(1)\tInclude rows/columns with the free square" << endl;
	cout << "(2)\tIgnore rows/columns with the free square" << endl;
	cout << "What would you like to do in the case of " << partialSize << "x" << partialSize <<" puzzles?: ";
	int x;
	getInputNum(x, 1, 2);

	switch (x) {
	case 1:
		freeSquare = true;
		break;
	case 2:
		freeSquare = false;
		break;
	}

	cout << "Your current 15-File:" << endl;

	outputAnalysis(cout, pList, numPuzzles, partialSize, freeSquare);

	delete[] pList;
	pList = NULL;

	cout << "(1)\tSave these calculations in the solution file" << endl;
	cout << "(2)\tIgnore these calculations and return to the main menu" << endl;
	cout << "What would you like to do?: ";
	int y;
	getInputNum(y, 1, 2);

	switch (y) {
	case 1:
		saveSolution(partialSize,freeSquare);
		system("CLS");
		cout << "Solution file saved succesfully" << endl;
		break;
	case 2:
		system("CLS");
		break;
	}
}

int main()
{

	bool exit = false;

	while (true) {
		cout << "(1)\tManually create configuration" << endl;
		cout << "(2)\tGenerate random configurations" << endl;
		cout << "(3)\tCalculate continuous rows/columns from a 15-file" << endl;
		cout << "(4)\tExit program" << endl;
		cout << "What would you like to do?: ";
		int x;
		getInputNum(x, 1, 4);

		switch (x) {
		case 1:
			makeManualPuzzle();
			break;
		case 2: 
			makeRandomPuzzle();
			break;
		case 3:
			readPuzzles();
			break;
		case 4:
			exit = true;
			break;
		}
		
		if (exit)
			break;
	}

}
