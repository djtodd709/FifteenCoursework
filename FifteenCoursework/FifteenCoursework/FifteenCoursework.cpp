// FifteenCoursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

void saveSolution() {
	ofstream outfile("Solution-File.txt");
	if (!outfile) {
		cout << "Could not open the solution-file." << endl;
		return;
	}

	int numPuzzles;
	Puzzle** pList = openFile(numPuzzles, 4);

	outfile << numPuzzles << endl;

	for (int i = 0; i < numPuzzles; i++) {
		Puzzle* p = pList[i];
		outfile << *p;
		int continuousAns = p->getAnswer(true);
		outfile << "row = " << continuousAns << endl;
		outfile << "column = " << continuousAns << endl;
		outfile << "reverse row = " << continuousAns << endl;
		outfile << "reverse column = " << continuousAns << endl << endl;
		delete p;
		p = NULL;
	}

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
	int numPuzzles;
	Puzzle** pList = openFile(numPuzzles, 4);

	cout << numPuzzles << endl;

	for (int i = 0; i < numPuzzles; i++) {
		Puzzle* p = pList[i];
		cout << *p;
		int continuousAns = p->getAnswer(freeSquare);
		cout << "row = " << continuousAns << endl;
		cout << "column = " << continuousAns << endl;
		cout << "reverse row = " << continuousAns << endl;
		cout << "reverse column = " << continuousAns << endl << endl;
		delete p;
		p = NULL;
	}

	delete[] pList;
	pList = NULL;

	cout << "(1)\tSave these calculations in the solution file" << endl;
	cout << "(2)\tIgnore these calculations and return to the main menu" << endl;
	cout << "What would you like to do?: ";
	int y;
	getInputNum(y, 1, 2);

	switch (y) {
	case 1:
		saveSolution();
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

	

	

	//int start[15] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	




	/*int start3[8] = { 1,2,3,4,5,6,7,8 };
	int start5[24] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24 };
	printPuzzle(start,4);
	int consecs = consecutivePerms(start, 4);
	cout << consecs << endl;
	int ans = answer(consecs, 4);
	
	printPuzzle(start5, 5);
	int consecs = consecutivePerms(start5, 5);
	cout << consecs << endl;
	unsigned long long ans = answerL(consecs, 5);

	cout << "row\t\t = " << ans << endl;
	cout << "column\t\t = " << ans << endl;
	cout << "reverse row\t = " << ans << endl;
	cout << "reverse column\t = " << ans << endl;*/
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
