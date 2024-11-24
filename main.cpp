/*
ConwayGenerations is a library for running John Conway's Game of Life
ConwayGenerations keeps track of how long a cell has been alive
ConwayGenerations is optimized for memory usage targetting microcontrollers.
Copyright Ali Raheem 2024 - https://github.com/ali-raheem/ConwayGenerations
MIT Licensed
File version: 2024-11-24 14:28 GMT
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
const int cols = 8; // 2 bits per cell, 4 cells per uint8_t 8 bytes gives 32 cells.
const uint8_t bits = 2; // 8 (bits per byte) / bits = number of cells per byte, * cols = number of cells per row.

int main(int argc, char* argv[]) {
    uint8_t state[rows][cols];
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

        auto start_time = chrono::high_resolution_clock::now();
        gol.next();
        auto end_time = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duration = end_time - start_time;

        for (int i = 0; i < rows; i++) {
            for (uint8_t j = 0; j < totalCells; j++) {
                uint8_t s = gol.getCell(state[i], j);

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

        cout << "Frame calculation time: " << duration.count() << " ms" << endl;

        bool is_stale = (memcmp(prev_state, state, sizeof(state)) == 0);
        if (is_stale) {
            staleness++;
        } else {
            staleness = 0;
        }

        if (max_staleness >= 0 && staleness > max_staleness)
            break;

        generations++;
        if (max_generations >= 0 && generations >= max_generations)
            break;

        this_thread::sleep_for(chrono::milliseconds(pause_duration));
    }

    return 0;
}
