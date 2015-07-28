#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Vec2
{
public:
	Vec2(float x, float y) : x(x), y(y) { }
	Vec2() : x(0.0f), y(0.0f) { }

	Vec2 operator+(const Vec2 &other) const
	{
		return Vec2(x + other.x, y + other.y);
	}

	Vec2 operator-(const Vec2 &other) const
	{
		return Vec2(x - other.x, y - other.y);
	}

	void operator=(const Vec2 &other)
	{
		x = other.x;
		y = other.y;
	}

	float GetX() const
	{
		return x;
	}

	float GetY() const
	{
		return y;
	}

private:
	float x;
	float y;
};

class Terrain
{
public:
	void AddPoint(const Vec2 &point)
	{
		points.push_back(point);
	}

	vector<Vec2> GetFlatGroundBounds() const
	{
		vector<Vec2> bounds;

		for (int i = 1; i < points.size(); i++)
		{
			if (points[i].GetX() == points[i - 1].GetX())
			{
				bounds.push_back(points[i]);
				bounds.push_back(points[i - 1]);
			}
		}

		return bounds;
	}

	bool InBounds(float x)
	{
		return x >= points[0].GetX() && x <= points[points.size() - 1].GetX();
	}

	float GetHeightAt(float x)
	{
		for (int i = 1; i < points.size(); i++)
		{
			if (points[i].GetX() > x)
			{
				Vec2 left = points[i - 1];
				Vec2 right = points[i];
				float slope = (right.GetY() - left.GetY()) / (right.GetX() - left.GetX());
				return left.GetY() + slope * (x - left.GetX());
			}
		}

		return 0;
	}

private:
	vector<Vec2> points;
};

class Lander
{
public:
	void InitTo(const Vec2 &position, float rotation, int thrust)
	{
		this->position = position;
		this->rotation = rotation;
		this->thrust = thrust;
	}

private:
	Vec2 position;
	float rotation;
	int thrust;
};

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	Terrain terrain;

	int surfaceN; // the number of points used to draw the surface of Mars.
	cin >> surfaceN; cin.ignore();
	for (int i = 0; i < surfaceN; i++) {
		int landX; // X coordinate of a surface point. (0 to 6999)
		int landY; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
		cin >> landX >> landY; cin.ignore();
		terrain.AddPoint(Vec2(landX, landY));
	}

	// game loop
	while (1) {
		int X;
		int Y;
		int hSpeed; // the horizontal speed (in m/s), can be negative.
		int vSpeed; // the vertical speed (in m/s), can be negative.
		int fuel; // the quantity of remaining fuel in liters.
		int rotate; // the rotation angle in degrees (-90 to 90).
		int power; // the thrust power (0 to 4).
		cin >> X >> Y >> hSpeed >> vSpeed >> fuel >> rotate >> power; cin.ignore();

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;

		cout << "-20 3" << endl; // rotate power. rotate is the desired rotation angle. power is the desired thrust power.
	}
}