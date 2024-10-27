/*
ConwayGenerations is a library for running John Conway's Game of Life
ConwayGenerations keeps track of how long a cell has been alive
ConwayGenerations is optimized for memory usage targetting microcontrollers.
Copyright Ali Raheem 2024 - https://github.com/ali-raheem/ConwayGenerations
MIT Licensed
File version: 2024-10-27 09:33 GMT
*/
#include "conwaygenerations.h"

const int rows = 32;
const int cols = 32;

uint8_t state[rows][cols];
ConwayGenerations<rows, cols> gol(state);

void setup() {
    Serial.begin(115200);
    Serial.println("Conway Generations!");
    randomSeed(1);

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            state[i][j] = random(0, 2);
            Serial.print((state[i][j] > 0) ? " - " : "   ");
        }
        Serial.println();
    }
    state[10][10] = 1;
    state[11][10] = 1;
    state[12][10] = 1;
    state[12][9] = 1;
    state[11][8] = 1;

    state[20][0] = 1;
    state[21][0] = 1;
    state[22][0] = 1;

    state[0][10] = 1;
    state[0][11] = 1;
    state[0][12] = 1;
    
    Serial.println();
}

void printFrame(){
      Serial.println(F("+------------------------------------------------------------------------------------------------+"));
}
void loop() {
    printFrame();
    Serial.println(gol.generation);
    printFrame();
    for(int i = 0; i < rows; i++) {
      Serial.print("|");
        for(int j = 0; j < cols; j++) {
            uint8_t s = state[i][j];
            switch(s) {
                case 0:
                    Serial.print("   ");
                    break;
                case 1:
                    Serial.print(" - ");
                    break;
                case 2:
                    Serial.print(" + ");
                    break;
                case 3:
                    Serial.print(" = ");
                    break;
                case 4:
                    Serial.print(" # ");
                    break;
                default:
                    Serial.print(" @ ");
            }
        }
       Serial.println("|");
    }
    gol.next();
    delay(200);
}
