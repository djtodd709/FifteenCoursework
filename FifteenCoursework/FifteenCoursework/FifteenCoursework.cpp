// FifteenCoursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Code by Dave Todd - b9052651

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h>
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

Puzzle** openFile(int& numPuzzles, int rowSize) {
	string line;
	ifstream infile("15-File.txt");
	if (!infile) {
		cout << "Could not open the 15-file. Make sure a 15-file already exists." << endl;
		return NULL;
	}

	infile >> line;
	numPuzzles = stoi(line);

	Puzzle** puzzleList = new Puzzle * [numPuzzles];

	int squareSize = rowSize * rowSize - 1;
	int i = 0;
	int listIndex = 0;
	int* square = new int[squareSize];
	while (infile >> line) {
		square[i] = stoi(line);
		i++;
		if (i == squareSize) {
			i = 0;
			Puzzle* p = new Puzzle(square, rowSize);
			puzzleList[listIndex] = p;
			listIndex++;
		}
	}

	if (listIndex != numPuzzles) {
		infile.close();
		cout << "Unexpected number of puzzles. 15-file is corrupt. Try creating a new 15-file." << endl;
		return NULL;
	}

	infile.close();
	return puzzleList;
}

void outputAnalysis(ostream& dest, Puzzle** pList, int numPuzzles, bool freeSquare) {
	dest << numPuzzles << endl;

	for (int i = 0; i < numPuzzles; i++) {
		Puzzle* p = pList[i];
		dest << *p;
		unsigned long long continuousAns = p->getAnswer(freeSquare);
		dest << "row = " << continuousAns << endl;
		dest << "column = " << continuousAns << endl;
		dest << "reverse row = " << continuousAns << endl;
		dest << "reverse column = " << continuousAns << endl << endl;
		delete p;
		p = NULL;
	}
}

void saveSolution(bool freeSquare) {
	ofstream outfile("Solution-File.txt");
	if (!outfile) {
		cout << "Could not open the solution-file." << endl;
		return;
	}

	int numPuzzles;
	Puzzle** pList = openFile(numPuzzles, 4);

	outputAnalysis(outfile, pList, numPuzzles, freeSquare);

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
	Puzzle** oldPuzzleList = openFile(oldNumPuzzles, 4);

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
	Puzzle* p = new Puzzle(4);

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

void makeRandomPuzzle() {
	system("CLS");
	cout << "Generate Random Configurations" << endl;
	cout << "How many configurations would you like to generate?: " << endl;
	int numPuzzles;
	getInputNum(numPuzzles, 1, INT_MAX);
	Puzzle** randPuzzles = new Puzzle*[numPuzzles];
	for (int i = 0; i < numPuzzles; i++) {
		Puzzle* p = new RandomPuzzle(4);

		cout << *p;
		cout << endl;

		randPuzzles[i] = p;
	}

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
	Puzzle** pList = openFile(numPuzzles, 4);

	if (pList == NULL) {
		cout << "You do not currently have a 15-File." << endl;
		cout << "Please create one using the other menu options first." << endl << endl;
		return;
	}

	bool freeSquare = true;

	cout << "(1)\tInclude rows/columns with the free square" << endl;
	cout << "(2)\tIgnore rows/columns with the free square" << endl;
	cout << "What would you like to do?: ";
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

	outputAnalysis(cout, pList, numPuzzles, freeSquare);

	delete[] pList;
	pList = NULL;

	cout << "(1)\tSave these calculations in the solution file" << endl;
	cout << "(2)\tIgnore these calculations and return to the main menu" << endl;
	cout << "What would you like to do?: ";
	int y;
	getInputNum(y, 1, 2);

	switch (y) {
	case 1:
		saveSolution(freeSquare);
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
	srand(time(NULL));

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
