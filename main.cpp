/*
ConwayGenerations is a library for running John Conway's Game of Life
ConwayGenerations keeps track of how long a cell has been alive
ConwayGenerations is optimized for memory usage targetting microcontrollers.
Copyright Ali Raheem 2024 - https://github.com/ali-raheem/ConwayGenerations
MIT Licensed
File version: 2024-11-06 10:12 GMT
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
const int cols = 8;

int main(int argc, char* argv[]) {
    uint8_t state[rows][cols];
    uint8_t bits = 2; // Or any other valid value (1, 2, 4)
    ConwayGenerations<rows, cols> gol(state, bits);
    unsigned generations = 0;
    int max_generations = -1;
    int max_staleness = -1;
    int pause_duration = 200;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

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

    uint8_t totalCells = gol.getTotalCellsInRow();

    // Initialize each cell individually
    for (int i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < totalCells; j++) {
            uint8_t randomValue = distribution(generator);
            gol.setCell(state[i], j, randomValue);
        }
    }

    uint8_t prev_state[rows][cols];
    int staleness = 0;

    while (true) {
        cout << "\033[2J\033[H";

        memcpy(prev_state, state, sizeof(state));

        gol.next();

        for (int i = 0; i < rows; i++) {
            for (uint8_t j = 0; j < totalCells; j++) {
                uint8_t s = gol.getCell(state[i], j);

                // Adjust the display based on the cell value
                switch (s) {
                    case 0:
                        cout << "   ";
                        break;
                    case 1:
                        cout << " - ";
                        break;
                    case 2:
                        cout << " = ";
                        break;
                    default:
                        cout << " # ";
                }
            }
            cout << endl;
        }
        cout << endl;

        bool is_stale = (memcmp(prev_state, state, sizeof(state)) == 0);
        if (is_stale) {
            staleness++;
        } else {
            staleness = 0;
        }

        if (max_staleness >= 0 && staleness > max_staleness)
            break;

        generations++;
        this_thread::sleep_for(chrono::milliseconds(pause_duration));
    }

    return 0;
}
