#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

#define ullong unsigned long long

using namespace std;

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	int N;
	cin >> N; cin.ignore();

	long xMin = LONG_MAX;
	long xMax = LONG_MIN;

	vector<long> yPositions;

	for (int i = 0; i < N; i++) {
		long X;
		long Y;
		cin >> X >> Y; cin.ignore();

		if (X < xMin)
		{
			xMin = X;
		}

		if (X > xMax)
		{
			xMax = X;
		}

		yPositions.push_back(Y);
	}

	sort(yPositions.begin(), yPositions.end());

	long yMean = yPositions[(long) (yPositions.size() / 2.0f)];

	ullong sum = xMax - xMin;

	for (long yPos : yPositions)
	{
		sum += (long) abs(yMean - yPos);
	}

	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	cout << sum << endl;
}