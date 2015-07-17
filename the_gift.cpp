#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	int numContributors;
	cin >> numContributors; cin.ignore();
	int cost;
	cin >> cost; cin.ignore();

	int totalBudget = 0;
	vector<int> budgets;
	for (int i = 0; i < numContributors; i++) {
		int B;
		cin >> B; cin.ignore();
		budgets.push_back(B);
		totalBudget += B;
	}

	if (totalBudget < cost)
	{
		cout << "IMPOSSIBLE" << endl;
		return 0;
	}

	sort(budgets.begin(), budgets.end());

	vector<int> contributions;

	for (int budget : budgets)
	{
		float averageContribution = cost / numContributors;
		float contribution = min(budget, (int) ceilf(averageContribution));
		contributions.push_back(contribution);
		cost -= contribution;
		numContributors--;
	}

	sort(contributions.begin(), contributions.end());

	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	for (int contribution : contributions)
	{
		cout << contribution << endl;
	}
}