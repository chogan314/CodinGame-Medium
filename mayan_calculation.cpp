#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_map>

#define ulong unsigned long long int

using namespace std;

//typedef unsigned long long int ulong;

ulong ConvertMayanToDecimal(vector<string> &mayanNumber, unordered_map<string, ulong> &dict);
string ConvertDecimalToMayan(ulong decimal, unordered_map<ulong, string> &dict, int numeralWidth);
string RecomposeNumeral(string &source, int numeralWidth);

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	int L;
	int H;
	cin >> L >> H; cin.ignore();

	stringstream streams[20];

	for (int i = 0; i < H; i++) {
		string numeral;
		cin >> numeral; cin.ignore();
		for (int j = 0; j < 20; j++)
		{
			streams[j] << numeral.substr(j * L, L);
		}
	}

	unordered_map<string, ulong> MayanToDecimalDict;
	unordered_map<ulong, string> DecimalToMayanDict;

	for (int i = 0; i < 20; i++)
	{
		string mayan = streams[i].str();
		MayanToDecimalDict[mayan] = i;
		DecimalToMayanDict[i] = mayan;
	}

	stringstream ss;
	vector<string> mayanNumberOne;
	vector<string> mayanNumberTwo;

	int S1;
	cin >> S1; cin.ignore();
	for (int i = 0; i < S1; i++) {
		string num1Line;
		cin >> num1Line; cin.ignore();
		ss << num1Line;
		if ((i + 1) % H == 0)
		{
			mayanNumberOne.push_back(ss.str());
			ss.str("");
			ss.clear();
		}
	}
	int S2;
	cin >> S2; cin.ignore();
	for (int i = 0; i < S2; i++) {
		string num2Line;
		cin >> num2Line; cin.ignore();
		ss << num2Line;
		if ((i + 1) % H == 0)
		{
			mayanNumberTwo.push_back(ss.str());
			ss.str("");
			ss.clear();
		}
	}


	string operation;
	cin >> operation; cin.ignore();

	ulong numOne = ConvertMayanToDecimal(mayanNumberOne, MayanToDecimalDict);
	ulong numTwo = ConvertMayanToDecimal(mayanNumberTwo, MayanToDecimalDict);

	ulong result = 0;

	switch (operation[0])
	{
	case '+':
		result = numOne + numTwo;
		break;
	case '-':
		result = numOne - numTwo;
		break;
	case '*':
		result = numOne * numTwo;
		break;
	case '/':
		result = numOne / numTwo;
		break;
	}

	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	cout << ConvertDecimalToMayan(result, DecimalToMayanDict, L) << endl;
}

ulong ConvertMayanToDecimal(vector<string> &mayanNumber, unordered_map<string, ulong> &dict)
{
	int n = mayanNumber.size() - 1;
	ulong accumulator = 0;

	for (string numeral : mayanNumber)
	{
		accumulator += dict[numeral] * (ulong) pow(20, n--);
	}

	return accumulator;
}

string ConvertDecimalToMayan(ulong decimal, unordered_map<ulong, string> &dict, int numeralWidth)
{
	int n = 0;
	while (pow(20, n + 1) < decimal)
	{
		n++;
	}

	stringstream ss;

	while (n >= 0)
	{
		for (int i = 19; i >= 0; i--)
		{
			ulong val = i * (ulong) pow(20, n);
			if (val <= decimal)
			{
				ss << RecomposeNumeral(dict[i], numeralWidth);
				decimal -= val;
				n--;
				break;
			}
		}
	}

	return ss.str();
}

string RecomposeNumeral(string &source, int numeralWidth)
{
	stringstream ss;
	for (unsigned int i = 0; i < source.size(); i += numeralWidth)
	{
		ss << source.substr(i, numeralWidth) << "\n";
	}
	return ss.str();
}