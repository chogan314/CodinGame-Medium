#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Node
{
public:
	Node(int id) : id(id) { }

	void AddAdjacentNode(Node *node)
	{
		adjNodes.push_back(node);
	}

	int GetId()
	{
		return id;
	}

	int NumAdjNodes()
	{
		return adjNodes.size();
	}

	Node *GetAdjNode(int index)
	{
		return adjNodes[index];
	}

private:
	int id;
	vector<Node*> adjNodes;
};

class Graph
{
public:
	~Graph()
	{
		for (auto elem : nodes)
		{
			delete elem.second;
		}
	}

	void AddEdge(int nodeAId, int nodeBId)
	{
		Node *nodeA = GetNode(nodeAId);
		Node *nodeB = GetNode(nodeBId);

		nodeA->AddAdjacentNode(nodeB);
		nodeB->AddAdjacentNode(nodeA);
	}

	Node *GetRandomNode()
	{
		return nodes.begin()->second;
	}

private:
	unordered_map<int, Node*> nodes;

	Node *GetNode(int id)
	{
		Node *node;
		if (nodes.count(id))
		{
			node = nodes[id];
		}
		else
		{
			node = new Node(id);
			nodes[id] = node;
		}

		return node;
	}
};

typedef vector<Node*> Path;

Path *PathToFarthestNode(Node *start);

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	Graph graph;

	int n; // the number of adjacency relations
	cin >> n; cin.ignore();
	for (int i = 0; i < n; i++) {
		int xi; // the ID of a person which is adjacent to yi
		int yi; // the ID of a person which is adjacent to xi
		cin >> xi >> yi; cin.ignore();

		graph.AddEdge(xi, yi);
	}

	Path *pathA = PathToFarthestNode(graph.GetRandomNode());
	Path a = *pathA;
	Path *diameterPath = PathToFarthestNode(a[0]);
	Path diameter = *diameterPath;

	int midPoint = (int) ((float) diameter.size() / 2.0f);

	int result = max(midPoint, (int) diameter.size() - midPoint - 1);

	cout << result << endl;

	delete pathA;
	delete diameterPath;

	// Write an action using cout. DON'T FORGET THE "<< endl"
	// To debug: cerr << "Debug messages..." << endl;

	//cout << "1" << endl; // The minimal amount of steps required to completely propagate the advertisement
}

Path *FarthestNodeSearch(Node *from, Node *node)
{
	if (node->NumAdjNodes() == 0)
	{
		Path *path = new Path();
		path->push_back(node);
		return path;
	}

	if (from && node->NumAdjNodes() == 1 && node->GetAdjNode(0)->GetId() == from->GetId())
	{
		Path *path = new Path();
		path->push_back(node);
		return path;
	}

	Path *result = nullptr;

	for (int i = 0; i < node->NumAdjNodes(); i++)
	{
		Node *adjNode = node->GetAdjNode(i);

		if (from && adjNode->GetId() == from->GetId())
		{
			continue;
		}

		Path *path = FarthestNodeSearch(node, adjNode);
		if (!result || path->size() > result->size())
		{
			if (result)
			{
				delete result;
			}

			result = path;
		}
	}

	result->push_back(node);

	return result;
}

Path *PathToFarthestNode(Node *start)
{
	return FarthestNodeSearch(nullptr, start);
}