#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

void ConwaySequenceNthGeneration(vector<int> sequence, int currentGeneration, int targetGeneration, vector<int> &dest);

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	int R;
	cin >> R; cin.ignore();
	int L;
	cin >> L; cin.ignore();

	vector<int> startSequence = { R };
	vector<int> finalSequence;
	ConwaySequenceNthGeneration(startSequence, 0, L - 1, finalSequence);

	stringstream ss;
	for (int i = 0; i < finalSequence.size(); i++)
	{
		ss << finalSequence[i];
		if (i != finalSequence.size() - 1)
		{
			ss << " ";
		}
	}

	cout << ss.str() << endl;
}

void ConwaySequenceNthGeneration(vector<int> sequence, int currentGeneration, int targetGeneration, vector<int> &dest)
{
	if (targetGeneration <= currentGeneration)
	{
		dest.insert(dest.end(), sequence.begin(), sequence.end());
		return;
	}

	vector<int> newSequence;

	int currentNum = -1;
	int comboLength = 0;

	for (int num : sequence)
	{
		if (currentNum == -1)
		{
			currentNum = num;
			comboLength = 1;
		}
		else if (currentNum != num)
		{
			newSequence.push_back(comboLength);
			newSequence.push_back(currentNum);
			currentNum = num;
			comboLength = 1;
		}
		else
		{
			comboLength++;
		}
	}

	if (currentNum != -1)
	{
		newSequence.push_back(comboLength);
		newSequence.push_back(currentNum);
	}

	if (currentGeneration < targetGeneration)
	{
		ConwaySequenceNthGeneration(newSequence, currentGeneration + 1, targetGeneration, dest);
	}
}