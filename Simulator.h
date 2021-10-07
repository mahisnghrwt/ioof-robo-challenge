#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <vector>
#include "./Robo.h"
#include "./Navigator.h"
#include <string.h>

// I tried with _WIN_32 flag, that works on msvc but g++ throws error
// msvc does not have strtok_r
#ifdef _MSC_VER
# define strtok_r strtok_s
#endif

#define MAX_INPUT_LENGTH 20

using namespace std;

class Simulator {
private:
	vector<Robo> robos;
	int activeRobot; // -1 means no active robot

	Navigator navigator;

	const string noRoboErr = "!Error:\tNo Active Robot";

	void initRobo(pair<int, int> pos, direction face) {
		int numRobos = (int)robos.size();
		Robo robo(numRobos, pos, face);
		robos.push_back(robo);
		cout << "!New robot added: Robot " << to_string(numRobos) << endl;

		// set the first robo active by default
		if (numRobos == 0) {
			activeRobot = 0;
			cout << "!Robot set as active" << endl;
		}
	}
	
	void activateRobot(int roboId) {
		if (roboId > 0 && roboId < (int)robos.size()) {
			activeRobot = roboId;
			cout << "!Active robot changed!" << endl;
		}
		else {
			cout << "!Error:\tInvalid robotId" << endl;
		}
	}

	void report() {
		if (activeRobot == -1) {
			cout << "!Error:\tNo active robot" << endl;
		}
		else {
			stringstream ss;
			ss << "Active robot: Robot " << activeRobot << endl;
			cout << ss.str();
			robos[activeRobot].report();
		}
	}

	/*
		* place command strictly follows this format - <PLACE x,y,faceDirection>
		* e.g. - "PLACE 1,2,EAST"
		*/
	bool tryParsePlaceCommand(string command, pair<pair<int, int>, string>& parsedCommand) {
		// delimeters newline character, ,(comma), and (space)
		const char* delims = " ,\n";
		char* nextToken = NULL;
		char* intermediate = strtok_r(&command[0], delims, &nextToken);
		int i = 0;
		bool isValidCommand = true;

		while (intermediate != NULL) {
			if (i > 3) {
				isValidCommand = false;
				break;
			}

			if (i == 0) {
				if (strcmp(intermediate, "PLACE") != 0) {
					isValidCommand = false;
					break;
				}
			}
			else if (i == 1 || i == 2) {
				int num = atoi(intermediate);
				if (i == 1)
					parsedCommand.first.first = num;
				else
					parsedCommand.first.second = num;
			}
			else {
				parsedCommand.second = intermediate;
			}

			intermediate = strtok_r(NULL, delims, &nextToken);
			i++;
		}

		return isValidCommand;
	}

	/*
		* place command strictly follows this format - <PLACE x,y,faceDirection>
		* e.g. - "PLACE 1,2,EAST"
		*/
	bool tryParseRobotCommand(string command, int& robotId) {
		// delimeters newline character, ,(comma), and (space)
		const char* delims = " \n";
		char* nextToken = NULL;
		char* intermediate = strtok_r(&command[0], delims, &nextToken);
		int i = 0;
		bool isValidCommand = true;

		while (intermediate != NULL) {
			if (i > 1) {
				isValidCommand = false;
				break;
			}

			if (i == 0) {
				if (strcmp(intermediate, "ROBOT") != 0) {
					isValidCommand = false;
					break;
				}
			}
			else {
				robotId = atoi(intermediate);
			}

			intermediate = strtok_r(NULL, delims, &nextToken);
			i++;
		}

		return isValidCommand;
	}

	void executeCommand(string command) {
		// execute command on active robot
		if (command == "MOVE") {
			// if there is no active robot, then ignore command
			if (activeRobot == -1) {
				cout << noRoboErr << endl;
				return;
			}
			robos[activeRobot].move();
		}
		else if (command == "LEFT") {
			if (activeRobot == -1) {
				cout << noRoboErr << endl;
				return;
			}
			robos[activeRobot].turn(side::LEFT);
		}
		else if (command == "RIGHT") {
			if (activeRobot == -1) {
				cout << noRoboErr << endl;
				return;
			}
			robos[activeRobot].turn(side::RIGHT);
		}
		else if (command == "REPORT") {
			report();
		}
		else {
			int roboId;
			if (tryParseRobotCommand(command, roboId)) {
				activateRobot(roboId);
			}
			else {
				// represents place command
				// <<x,y>, face>
				pair<pair<int, int>, string> placeCommand;
				if (tryParsePlaceCommand(command, placeCommand)) {
					direction face_;
					if (Navigator::isValidPos(placeCommand.first) && navigator.tryGetDirectionId(placeCommand.second, face_))
						initRobo(placeCommand.first, face_);
				}
			}
		}
	}

public:
	Simulator() {
		activeRobot = -1;
	}

	void run() {
		cout << "WELCOME" << endl << endl
			<< "INSTRUCTIONS:" << endl
			<< "\tAll the commands are case senstitive." << endl
			<< "\tCommands:" << endl
			<< "\t\tPLACE <x>,<y>,<face>" << endl
			<< "\t\t\tx - x position on grid (0 >= x < 5)" << endl
			<< "\t\t\ty - y position on grid (0 >= x < 5)" << endl
			<< "\t\t\tface - NORTH | EAST | SOUTH | WEST" << endl
			<< "\t\tROBOT <id>" << endl
			<< "\t\t\tid - Robot to set as active" << endl
			<< "\t\tREPORT" << endl
			<< "\t\t\treport active robot's position" << endl
			<< "\t\tLEFT" << endl
			<< "\t\t\t(executed on active robot only)" << endl
			<< "\t\tRIGHT" << endl
			<< "\t\t\t(executed on active robot only)" << endl
			<< "\t\tMOVE" << endl
			<< "\t\t\t(executed on active robot only)" << endl << endl;


		string input;
		while (input != "REPORT") {
			getline(cin, input);
			try {
				executeCommand(input);
			}
			catch(...) {
				cout << "Exception triggered, invalid command!";
			}
		}
	}

};

#endif