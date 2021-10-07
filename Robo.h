#include <iostream>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include "./Navigator.h"

using namespace std;

class Robo {
private:
	// Represents north, east, south, west respectively
	// add a respective pair with robo position to move in that direction
	const pair<int, int> movementVectors[4] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

	int id;
	pair<int, int> pos;
	direction face;
	Navigator navigator;

public:
	Robo(int id, pair<int, int> pos, direction face) {
		this->id = id;
		this->pos = pos;
		this->face = face;
	}

	void turn(side turnTo) {
		face = navigator.turnTo(face, turnTo);
		cout << "!Robot " << to_string(id) << " now faces " << navigator.getDirectionName(face) << endl;
	}

	void move() {
		pair<int, int> resultPos = { pos.first + movementVectors[(int)face].first, pos.second + movementVectors[(int)face].second };
		if (Navigator::isValidPos(resultPos)) {
			stringstream ss;
			ss << "!Robot " << id << " moved from <" << pos.first << ", " << pos.second << "> to <" << resultPos.first << ", " << resultPos.second << ">" << endl;
			cout << ss.str();
			pos = resultPos;
		}
		else {
			cout << "!Error:\tInvalid position." << endl;
		}
	}

	void report() {
		// using stringstream for formatted string
		stringstream ss;
		ss << "Output: " << pos.first << "," << pos.second << "," + navigator.getDirectionName(face);
		cout << ss.str();
	}
};