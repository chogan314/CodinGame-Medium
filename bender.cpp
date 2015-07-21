#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sstream>

using namespace std;

enum Direction
{
	S = 0,
	E,
	N,
	W
};

enum PlayerState
{
	PLAYER_STUCK,
	PLAYER_MOVING,
	PLAYER_ARRIVED
};

struct Position
{
	int x = 0;
	int y = 0;

	Position() : x(0), y(0) { }
	Position(int x, int y) : x(x), y(y) { }
	Position(const Position &other) : x(other.x), y(other.y) { }

	void operator=(const Position &other)
	{
		x = other.x;
		y = other.y;
	}

	Position GetNextPosition(Direction dir) const
	{
		int newX = x;
		int newY = y;

		switch (dir)
		{
		case S:
			newY += 1;
			break;
		case E:
			newX += 1;
			break;
		case N:
			newY -= 1;
			break;
		case W:
			newX -= 1;
			break;
		}

		return Position(newX, newY);
	}

	bool operator==(const Position &other) const
	{
		return x == other.x && y == other.y;
	}
};

struct PositionHash
{
	size_t operator()(const Position& position) const
	{
		return hash<int>()(position.x) ^ (hash<int>()(position.y) << 1);
	}
};

class Player;
class TileBase;
template <typename T>
class Tile;
class Board;

typedef unordered_map<Position, TileBase*, PositionHash> TileDict;

class TileBase
{
public:
	TileBase(Position position) : pos(position) { }
	virtual ~TileBase() { }

	virtual bool PreMove(Player &player) = 0;
	virtual void OnMove(Player &player) = 0;

	Position GetPosition() const
	{
		return pos;
	}

protected:
	Position pos;
};

class Player
{
public:
	Player(Position pos) : pos(pos) { }

	Direction Advance(TileDict &tileDict)
	{
		int tries = 0;
		Direction direction = heading;

		while (tries < 4)
		{
			Position nextPosition = pos.GetNextPosition(heading);
			TileBase *tile = tileDict[nextPosition];

			if (tile->PreMove(*this))
			{
				direction = heading;
				tile->OnMove(*this);
				break;
			}
			else
			{
				tries++;
				Turn();
			}
		}

		if (tries >= 4)
		{
			state = PLAYER_STUCK;
		}

		return direction;
	}

	void Turn()
	{
		int headingVal = 0;

		if (inverted)
		{
			headingVal = (int) heading - 1;
		}
		else
		{
			headingVal = (int) heading + 1;
		}

		if (headingVal < 0)
		{
			headingVal = 3;
		}
		else if (headingVal > 3)
		{
			headingVal = 0;
		}

		heading = (Direction) headingVal;
	}

	void InvertTurningSequence()
	{
		inverted = !inverted;
	}

	void ToggleBreaker()
	{
		breaker = !breaker;
	}

	bool IsBreaker() const
	{
		return breaker;
	}

	Position GetPosition() const
	{
		return pos;
	}

	void SetPosition(Position position)
	{
		pos = position;
	}

	void SetHeading(Direction direction)
	{
		heading = direction;
	}

	PlayerState GetState() const
	{
		return state;
	}

	void SetState(PlayerState state)
	{
		this->state = state;
	}

private:
	Position pos;
	Direction heading = S;
	bool breaker = false;
	bool inverted = false;
	PlayerState state = PLAYER_MOVING;
};

template <typename T>
class Tile : public TileBase
{
public:
	Tile(Position pos,
		T *tileData,
		bool(*preMoveFunc)(T* tileData, Player &player),
		void(*onMoveFunc)(T* tileData, Player &player)) :
		TileBase(pos),
		tileData(tileData),
		preMoveFunc(preMoveFunc),
		onMoveFunc(onMoveFunc)
	{ }

	Tile(Position pos,
		bool(*preMoveFunc)(T* tileData, Player &player),
		void(*onMoveFunc)(T* tileData, Player &player)) :
		TileBase(pos),
		preMoveFunc(preMoveFunc),
		onMoveFunc(onMoveFunc)
	{ }

	Tile() :
		TileBase(Position(0, 0))
	{ }

	~Tile()
	{
		if (tileData)
		{
			delete tileData;
		}
	}

	bool PreMove(Player &player)
	{
		if (preMoveFunc)
		{
			return preMoveFunc(tileData, player);
		}

		return true;
	}

	void OnMove(Player &player)
	{
		player.SetPosition(pos);
		if (onMoveFunc)
		{
			onMoveFunc(tileData, player);
		}
	}

	void SetTileData(T *tileData)
	{
		this->tileData = tileData;
	}

private:
	bool (*preMoveFunc)(T* tileData, Player &player) = nullptr;
	void(*onMoveFunc)(T* tileData, Player &player) = nullptr;
	T *tileData = nullptr;
};

class Board
{
public:
	Board(int width, int height) :
		width(width),
		height(height)
	{ }

	~Board()
	{
		if (player)
		{
			delete player;
		}

		for (auto element : tileDict)
		{
			delete element.second;
		}
	}

	void Run()
	{
		while (moves < 1000000 && player->GetState() == PLAYER_MOVING)
		{
			Direction direction = player->Advance(tileDict);
			switch (direction)
			{
			case S:
				moveHistory.push_back("SOUTH");
				break;
			case E:
				moveHistory.push_back("EAST");
				break;
			case N:
				moveHistory.push_back("NORTH");
				break;
			case W:
				moveHistory.push_back("WEST");
				break;
			default:
				break;
			}
			moves++;
		}
	}

	bool ContainsTile(Position position) const
	{
		if (position.x < 0 || position.y < 0)
		{
			return false;
		}

		if (position.x >= width || position.y >= height)
		{
			return false;
		}

		return true;
	}

	void AddTile(Position position, char type)
	{
		switch (type)
		{
		case '@':
			player = new Player(position);
			tileDict[position] = CreateEmptyTile(position);
			break;
		case ' ':
			tileDict[position] = CreateEmptyTile(position);
			break;
		case '$':
			tileDict[position] = CreateEndTile(position);
			break;
		case '#':
			tileDict[position] = CreateUnbreakableWallTile(position);
			break;
		case 'X':
			tileDict[position] = CreateBreakableWallTile(position);
			break;
		case 'S':
		case 'E':
		case 'N':
		case 'W':
			tileDict[position] = CreateDirectionTile(position, type);
			break;
		case 'I':
			tileDict[position] = CreateInversionTile(position);
			break;
		case 'B':
			tileDict[position] = CreateBreakerTile(position);
			break;
		case 'T':
			tileDict[position] = CreateTeleportTile(position);
			break;
		default:
			cout << "ERROR: UNRECOGNIZED SYMBOL " << type << endl;
			tileDict[position] = CreateEmptyTile(position);
			break;
		}
	}

	vector<string> &GetMoveHistory()
	{
		return moveHistory;
	}

	bool IsLooping()
	{
		return moves >= loopThreshold || player->GetState() == PLAYER_STUCK;
	}

private:
	int width = 0;
	int height = 0;
	TileDict tileDict;
	Player *player = nullptr;
	Tile<Position> *teleporterA = nullptr;
	const int loopThreshold = 1000000;
	unsigned long moves = 0;
	vector<string> moveHistory;

	TileBase *CreateEmptyTile(Position position)
	{
		return new Tile<bool>(position, nullptr, nullptr);
	}

	TileBase *CreateEndTile(Position position)
	{
		auto onMoveFunc = [](bool *tileData, Player &player)
		{
			player.SetState(PLAYER_ARRIVED);
		};

		return new Tile<bool>(position, nullptr, onMoveFunc);
	}

	TileBase *CreateUnbreakableWallTile(Position position)
	{
		auto preMoveFunc = [](bool *tileData, Player &player)
		{
			return false;
		};

		return new Tile<bool>(position, preMoveFunc, nullptr);
	}

	TileBase *CreateBreakableWallTile(Position position)
	{
		auto preMoveFunc = [](bool *tileData, Player &player)
		{
			return *tileData || player.IsBreaker();
		};

		auto onMoveFunc = [](bool *tileData, Player &player)
		{
			if (player.IsBreaker())
			{
				tileData = new bool(true);
			}
		};

		return new Tile<bool>(position, new bool(false), preMoveFunc, onMoveFunc);
	}

	TileBase *CreateDirectionTile(Position position, char symbol)
	{
		auto onMoveFunc = [](Direction *tileData, Player &player)
		{
			player.SetHeading(*tileData);
		};

		Direction direction = S;

		switch (symbol)
		{
		case 'E':
			direction = E;
			break;
		case 'N':
			direction = N;
			break;
		case 'W':
			direction = W;
			break;
		default:
			break;
		}

		return new Tile<Direction>(position, new Direction(direction), nullptr, onMoveFunc);
	}

	TileBase *CreateInversionTile(Position position)
	{
		auto onMoveFunc = [](bool *tileData, Player &player)
		{
			player.InvertTurningSequence();
		};

		return new Tile<bool>(position, nullptr, onMoveFunc);
	}

	TileBase *CreateBreakerTile(Position position)
	{
		auto onMoveFunc = [](bool *tileData, Player &player)
		{
			player.ToggleBreaker();
		};

		return new Tile<bool>(position, nullptr, onMoveFunc);
	}

	TileBase *CreateTeleportTile(Position position)
	{
		auto onMoveFunc = [](Position *tileData, Player &player)
		{
			player.SetPosition(*tileData);
		};

		Tile<Position> *tile = new Tile<Position>(position, nullptr, onMoveFunc);

		if (teleporterA)
		{
			teleporterA->SetTileData(new Position(position));
			tile->SetTileData(new Position(teleporterA->GetPosition()));
		}

		return tile;
	}
};

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	int L;
	int C;
	cin >> L >> C; cin.ignore();

	Board board(C, L);

	for (int y = 0; y < L; y++) {
		string row;
		getline(cin, row);
		for (int x = 0; x < row.size(); x++)
		{
			board.AddTile(Position(x, y), row[x]);
		}
	}

	board.Run();

	if (board.IsLooping())
	{
		cout << "LOOP" << endl;
	}
	else
	{
		vector<string> moves = board.GetMoveHistory();
		stringstream ss;
		for (string move : moves)
		{
			ss << move << endl;
		}

		cout << ss.str();
	}
}