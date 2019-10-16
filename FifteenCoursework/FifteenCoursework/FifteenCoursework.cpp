// FifteenCoursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma once
#include <iostream>
#include <algorithm>
#include <time.h>
#include "Puzzle.h"

using namespace std;

int fac(int f) {
	if (f == 1) {
		return 1;
	}
	return f * fac(f - 1);
}

unsigned long long fac(unsigned long long f) {
	if (f == 1) {
		return 1;
	}
	return f * fac(f - 1);
}

int answer(int consecutives, int rowSize) {
	return (rowSize - 1) * consecutives * fac((rowSize * (rowSize - 1)) - 1) / 2;
}

unsigned long long answerL(int consecutives, int rowSize) {
	return (rowSize - 1) * consecutives * fac((unsigned long long)(rowSize * (rowSize - 1)) - 1) / 2;
}

int compare(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}

int consecutivePerms(int* layout, int rowSize) {
	int squareSize = rowSize * rowSize - 1;
	int* layoutCopy = new int[squareSize];
	copy(layout,layout+squareSize,layoutCopy);
	qsort(layoutCopy, squareSize, sizeof(int), compare);
	int consecutives = 0;
	int conStreak = 0;
	for (int i = 1; i < squareSize; i++) {
		if (layoutCopy[i] - 1 == layoutCopy[i-1]) {
			conStreak++;
			if (conStreak >= rowSize-1) {
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


/// ^^^^ TO BE MOVED

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
		if ((x > max) | (x < min)) {
			cout << x << " is not within the allowed number range: ";
			continue;
		}
		passed = true;
	}
}

void fillPuzzle(Puzzle*& p) {
	int x;
	cout << "Please enter a number for the next tile: ";
	getInputNum(x, 1, p->getNumLimit());
	while (p->isTilePresent(x)) {
		cout << x << " is already present in the square: ";
		getInputNum(x, 1, p->getNumLimit());
	}
	p->setNextTile(x);
	system("CLS");
	cout << "Manually Create Configuration" << endl;
	cout << *p;
}

void makeManualPuzzle() {
	system("CLS");
	cout << "Manually Create Configuration" << endl;

	Puzzle* p = new Puzzle(4);

	cout << *p;

	while (!p->isFull()) {
		fillPuzzle(p);
	}

	cout << endl << "(1) Overite 15-File" << endl;
	cout << "(2) Append to 15-File" << endl;
	cout << "(3) Do not add to 15-File" << endl;
	cout << "What would you like to do with this configuration?: ";
	int choice;
	getInputNum(choice, 1, 3);

	delete p;
	p = NULL;

}

void makeRandomPuzzle() {
	system("CLS");
	cout << "Generate Random Configurations" << endl;
	cout << "How many configurations would you like to generate?: " << endl;
	int numPuzzles;
	getInputNum(numPuzzles, 1, INT_MAX);
	for (int i = 0; i < numPuzzles; i++) {
		Puzzle* p = new RandomPuzzle(4);

		cout << *p;
		cout << endl;

		delete p; ///<------------HMM
		p = NULL;
	}

	cout << endl << "(1) Overite 15-File" << endl;
	cout << "(2) Append to 15-File" << endl;
	cout << "(3) Do not add to 15-File" << endl;
	cout << "What would you like to do with this configuration set?: ";
	int choice;
	getInputNum(choice, 1, 3);
}

int main()
{
	srand(time(NULL));

	bool exit = false;

	while (true) {
		cout << "(1)\tManually create configuration" << endl;
		cout << "(2)\tGenerate random configurations" << endl;
		cout << "(3)\tRead in configurations from a file" << endl;
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
