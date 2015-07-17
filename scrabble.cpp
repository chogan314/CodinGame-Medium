#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;

typedef unordered_map<string, int> Dictionary;
typedef unordered_map<char, int> TileDict;

void AddTilesToDict(string tiles, int value, TileDict &tileDict);
vector<string> GetAllCombinations(string str);
int CalcWordValue(string &word, TileDict &tileDict);

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	Dictionary dict;

	int N;
	cin >> N; cin.ignore();
	for (int i = 0; i < N; i++) {
		string W;
		getline(cin, W);
		dict[W] = i;
	}

	TileDict tileDict;
	AddTilesToDict("eaionrtlsu", 1, tileDict);
	AddTilesToDict("dg", 2, tileDict);
	AddTilesToDict("bcmp", 3, tileDict);
	AddTilesToDict("fhvwy", 4, tileDict);
	AddTilesToDict("k", 5, tileDict);
	AddTilesToDict("jx", 8, tileDict);
	AddTilesToDict("qz", 10, tileDict);

	string LETTERS;
	getline(cin, LETTERS);

	int maxScore = -1;
	string word = "";

	vector<string> combinations = GetAllCombinations(LETTERS);

	for (string combination : combinations)
	{
		if (dict.count(combination))
		{
			int score = CalcWordValue(combination, tileDict);
			if (score > maxScore)
			{
				maxScore = score;
				word = combination;
			}
			if (score == maxScore)
			{
				if (dict[combination] < dict[word])
				{
					maxScore = score;
					word = combination;
				}
			}
		}
	}


	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	 cout << word << endl;
}

void AddTilesToDict(string tiles, int value, TileDict &tileDict)
{
	for (char tile : tiles)
	{
		tileDict[tile] = value;
	}
}

vector<string> GetAllCombinations(string str)
{
	vector<string> strings;

	char c = str[0];
	strings.push_back(string(1, c));

	if (str.size() == 1)
	{
		return strings;
	}

	string remainder = string(str);
	remainder.erase(remainder.begin());
	vector<string> combinationsFromRemainder = GetAllCombinations(remainder);

	for (string combination : combinationsFromRemainder)
	{
		strings.push_back(combination);
		for (int i = 0; i <= combination.size(); i++)
		{
			string copy = string(combination);
			copy.insert(copy.begin() + i, c);
			strings.push_back(copy);
		}
	}

	return strings;
}

int CalcWordValue(string &word, TileDict &tileDict)
{
	int score = 0;

	for (char c : word)
	{
		score += tileDict[c];
	}

	return score;
}