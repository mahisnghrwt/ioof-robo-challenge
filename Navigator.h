#ifndef _HELPER_H
#define _HELPER_H

#include <iostream>
#include <utility>

using namespace std;

// grid size is 5x5
#define GRID_SIZE 5

enum class side {
	LEFT = -1,
	RIGHT = 1
};

// directions are intentionally arranged in clock-wise order, to turn right add +1 and for left add -1.
enum class direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class Navigator {
	private:
		const string directionNames[4] = { "NORTH", "EAST", "SOUTH", "WEST" };
	public:
		string getDirectionName(direction d) {
			return directionNames[(int)d];
		}

		// return respective direction enum if a valid directionName
		bool tryGetDirectionId(string directionName, direction& d) {
			for (int i = 0; i < 4; i++) {
				if (directionName == directionNames[i]) {
					d = (direction)i;
					return true;
				}
			}
			return false;
		}

		direction turnTo(direction currentDir, side s) {
			int result = (int)currentDir + (int)s;
			// below two, are wrap around cases
			if (result == 4) return direction::NORTH;
			if (result == -1) return direction::WEST;
			return (direction)result;
		}

		static bool isValidPos(pair<int, int> pos) {
			return ((pos.first >= 0 && pos.first < GRID_SIZE) && (pos.second >= 0 && pos.second < GRID_SIZE));
		}
};
#endif