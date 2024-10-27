/*
ConwayGenerations is a library for running John Conway's Game of Life
ConwayGenerations keeps track of how long a cell has been alive
ConwayGenerations is optimized for memory usage targetting microcontrollers.
Copyright Ali Raheem 2024 - https://github.com/ali-raheem/ConwayGenerations
MIT Licensed
File version: 2024-10-27 21:48 GMT
*/

#pragma once

template<int rows, int cols>
class ConwayGenerations {
public:
    ConwayGenerations(uint8_t (&state)[rows][cols])
        : state(state) {
          generation = 0;
          
    }
    uint16_t generation;
    
    void next() {
        uint8_t prevRow[cols];
        uint8_t firstRow[cols];
        uint8_t curRow[cols];

        memcpy(firstRow, state[0], cols);
        memcpy(prevRow, state[rows - 1], cols);
        uint8_t sum_r = 0, sum_c = 0, sum_l = 0;
        for(int i = 0; i < rows; i++) {
            memcpy(curRow, state[i], cols);
            uint8_t *nextRow = (i == rows - 1)? firstRow : state[i + 1];
            sum_l = !!prevRow[cols - 1]
                  + !!curRow[cols - 1]
                  + !!nextRow[cols - 1];
            sum_c = !!prevRow[0]
                  + !!curRow[0]
                  + !!nextRow[0];
            for(int j = 0; j < cols; j++) {
                sum_r = !!prevRow[(j + 1) % cols]
                      + !!curRow[(j + 1) % cols]
                      + !!nextRow[(j + 1) % cols];
                state[i][j] = getNextState(curRow[j], sum_l + sum_c + sum_r);
                sum_l = sum_c;
                sum_c = sum_r;
            }
            memcpy(prevRow, curRow, cols);
        }
        generation++;
    }

private:
    uint8_t (&state)[rows][cols];
    uint8_t getNextState(uint8_t s, uint8_t sum) {
        switch(sum) {
            case 3:
                return s + 1;
            case 4:
                return (s == 0)? 0 : s + 1;
            default:
                return 0;
        }
    }
};
