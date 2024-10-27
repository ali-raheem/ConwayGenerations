/*
ConwayGenerations is a library for running John Conway's Game of Life
ConwayGenerations keeps track of how long a cell has been alive
ConwayGenerations is optimized for memory usage targetting microcontrollers.
Copyright Ali Raheem 2024 - https://github.com/ali-raheem/ConwayGenerations
MIT Licensed
File version: 2024-10-26 12:36 GMT
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <string>
#include <cstdlib>
#include <cstring>

#include "conwaygenerations.h"

using namespace std;

const int rows = 32;
const int cols = 32;

int main(int argc, char* argv[]) {
    array<std::array<uint8_t, cols>, rows> state{};
    ConwayGenerations<rows, cols> gol(state);
    int max_generations = -1;
    int max_staleness = -1;
    int pause_duration = 200;

    for (int i = 1; i < argc; ++i) {
      std::string_view arg = argv[i];

        if ((arg == "-s" || arg == "--staleness") && i + 1 < argc) {
            max_staleness = stoi(argv[++i]);
        }
        else if ((arg == "-p" || arg == "--pause") && i + 1 < argc) {
            pause_duration = stoi(argv[++i]);
        }
        else if ((arg == "-g" || arg == "--generations") && i + 1 < argc) {
            max_generations = stoi(argv[++i]);
        }
        else {
            cerr << "Unknown argument: " << arg << endl;
            return 1;
        }
    }

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 generator(seed);
    uniform_int_distribution<int> distribution(0, 1);

    for (auto& row: state) {
      for (auto& cell : row) {
            cell = distribution(generator);
        }
    }

    std::array<std::array<uint8_t, cols>, rows> prev_state;
    int staleness = 0;

    for (unsigned generations = 0; max_generations < 0
	   || generations < max_generations; ++generations) {
        cout << "\033[2J\033[H";

	std::copy(prev_state.begin(), prev_state.end(), state.begin());
        gol.next();

        for (auto& row: state) {
	  for (auto& cell: row) {
		switch (cell) {
		case 0:
		  cout << "   ";
		  break;
		case 1:
		  cout << " - ";
		  break;
		case 2:
		  cout << " + ";
		  break;
		case 3:
		  cout << " = ";
		  break;
		case 4:
		  cout << " # ";
		  break;
		default:
		  cout << " @ ";
		}
	    }
            cout << endl;
        }
        cout << endl;

        bool is_stale = std::equal(prev_state.begin(), prev_state.end(), state.begin());
        if (is_stale) {
            staleness++;
        } else {
            staleness = 0;
        }

        if (max_staleness >= 0 && staleness > max_staleness)
            break;

	this_thread::sleep_for(chrono::milliseconds(pause_duration));
    }

    return 0;
}
