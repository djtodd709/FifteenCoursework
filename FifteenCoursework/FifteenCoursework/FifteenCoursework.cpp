// FifteenCoursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

bool evenInversions(int* layout) {
	int inversions = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = i + 1; j < 8; j++) {
			inversions += layout[i] > layout[j];
		}
	}
	return (inversions%2 == 0);
}

bool isValidLayout() {
	return true;
}

int fac(int f) {
	if (f == 1) {
		return 1;
	}
	return f * fac(f - 1);
}

int answer(int consecutives, int rowSize) {
	return consecutives * fac((rowSize * (rowSize - 1)) - 1) / 2;
}


int main()
{
	int odds = 0;
	int evens = 0;
	int start[8] = { 1,2,3,4,5,6,7,8 };
	for (int i = 1; i < 9; i++) {
		start[0] = i;
		for (int j = 1; j < 9; j++) {
			if (j == i) {
				continue;
			}
			start[1] = j;
			for (int k = 1; k < 9; k++) {
				if (k == i || k==j) {
					continue;
				}
				start[2] = k;
				for (int l = 1; l < 9; l++) {
					if (l == i || l == j || l == k) {
						continue;
					}
					start[3] = l;
					for (int m = 1; m < 9; m++) {
						if (m == i || m == j || m == k || m == l) {
							continue;
						}
						start[4] = m;
						for (int n = 1; n < 9; n++) {
							if (n == i || n == j || n== k || n == l || n == m) {
								continue;
							}
							start[5] = n;
							for (int o = 1; o < 9; o++) {
								if (o == i || o == j || o == k || o == l || o == m || o == n) {
									continue;
								}
								start[6] = o;
								for (int p = 1; p < 9; p++) {
									if (p == i || p == j || p == k || p == l || p == m || p == n || p == o) {
										continue;
									}
									start[7] = p;
									if (evenInversions(start)) {
										evens++;
									}
									else {
										odds++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	cout << evenInversions(start);
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
