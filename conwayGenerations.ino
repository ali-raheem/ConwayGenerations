/*
ConwayGenerations is a library for running John Conway's Game of Life
ConwayGenerations keeps track of how long a cell has been alive
ConwayGenerations is optimized for memory usage targetting microcontrollers.
Copyright Ali Raheem 2024 - https://github.com/ali-raheem/ConwayGenerations
MIT Licensed
File version: 2024-11-06 00:15 GMT
*/

#include "conwaygenerations.h"

const int rows = 32;
const int cols = 4;
uint8_t bits = 1;

uint8_t state[rows][cols];
ConwayGenerations<rows, cols> gol(state, bits);

void setup() {
    Serial.begin(115200);
    Serial.println("Conway Generations!");
    randomSeed(1);

    uint8_t totalCells = gol.getTotalCellsInRow();
    uint8_t maxCellValue = (1 << bits) - 1;

    for(int i = 0; i < rows; i++) {
        for(uint8_t j = 0; j < totalCells; j++) {
            uint8_t randomValue = random(0, 2);
            gol.setCell(state[i], j, randomValue);
            Serial.print((randomValue > 0) ? " ' " : "   ");
        }
        Serial.println();
    }
    Serial.println();
}

void printFrame() {
    Serial.println(F("+------------------------------------------------------------------------------------------------+"));
}

void loop() {
    printFrame();
    Serial.println(gol.generation);
    printFrame();

    uint8_t totalCells = gol.getTotalCellsInRow();

    for(int i = 0; i < rows; i++) {
        Serial.print("|");
        for(uint8_t j = 0; j < totalCells; j++) {
            uint8_t s = gol.getCell(state[i], j);

            switch(s) {
                case 0:
                    Serial.print(F("   "));
                    break;
                case 1:
                    Serial.print(F(" ' "));
                    break;
                case 2:
                    Serial.print(F(" - "));
                    break;
                case 3:
                    Serial.print(F(" + "));
                    break;
                case 4:
                    Serial.print(F(" = "));
                    break;
                case 5:
                    Serial.print(F(" # "));
                    break;
                default:
                    Serial.print(F(" @ "));
            }
        }
        Serial.println("|");
    }
    gol.next();
    delay(100);
}
