/*
ConwayGenerations is a library for running John Conway's Game of Life
ConwayGenerations keeps track of how long a cell has been alive
ConwayGenerations is optimized for memory usage targetting microcontrollers.
Copyright Ali Raheem 2024 - https://github.com/ali-raheem/ConwayGenerations
MIT Licensed
File version: 2024-10-27 09:33 GMT
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

        memcpy(firstRow, state[0], rowSize);
        memcpy(prevRow, state[rows - 1], rowSize);

        uint8_t sum_r = 0, sum_c = 0, sum_l = 0, s = 0;
        for(int i = 0; i < rows; i++) {
            memcpy(curRow, state[i], rowSize);
            for(int j = 0; j < cols; j++) {
                s = curRow[j];

                if(j == 0) {
                    sum_l = !!prevRow[cols - 1]
                          + !!curRow[cols - 1]
                          + !!((i == rows - 1) ? firstRow[(cols - 1) % cols] : state[(i + 1) % rows][cols - 1]);
                    sum_c = !!prevRow[0]
                          + !!s
                          + !!((i == rows - 1) ? firstRow[0] : state[(i + 1) % rows][0]);
                } else {
                    sum_l = sum_c;
                    sum_c = sum_r;
                }

                sum_r = !!prevRow[(j + 1) % cols]
                      + !!curRow[(j + 1) % cols]
                      + !!((i == rows - 1) ? firstRow[(j + 1) % cols] : state[i + 1][(j + 1) % cols]);

                uint8_t sum = sum_l + sum_c + sum_r;
                state[i][j] = getNextState(s, sum);
            }
            memcpy(prevRow, curRow, rowSize);
        }
        generation++;
    }

private:
    uint8_t (&state)[rows][cols];
    const size_t rowSize = cols * sizeof(uint8_t);
    uint8_t getNextState(uint8_t curState, uint8_t sum) {
        switch(sum) {
            case 3:
                return curState + 1;
            case 4:
                return (curState == 0) ? 0 : curState + 1;
            default:
                return 0;
        }
    }
};
