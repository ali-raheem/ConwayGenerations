const int rows = 32, cols = 32;

uint8_t getNextState(uint8_t curState, uint8_t sum) {
  switch(sum) {
  case 3:
    return curState + 1;
    break;
  case 4:
    return (curState == 0)? 0 : curState + 1;
    break;
  default:
    return 0;
  }
}


  uint8_t state[rows][cols];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Conway Generations!");
  randomSeed(1);

  uint8_t i, j;
  for(i = 0; i < rows; i++) {
    for(j = 0; j < cols; j++) {
      state[i][j] = random(0, 2);
      Serial.print((state[i][j] > 0)?" - " : "   ");
    }
    Serial.println();
  }
  Serial.println();
}

void loop() {
   // cout << "\033[2J\033[H";
   uint8_t i, j;
   uint8_t prevRow[cols];
   uint8_t firstRow[cols];
   uint8_t curRow[cols];
   memcpy(firstRow, state[0], cols);
   memcpy(prevRow, state[rows - 1], cols);
   uint8_t sum_r, sum_c, sum_l, s;
   for(i = 0; i < rows; i++) {
     memcpy(curRow, state[i], cols);
     for(j = 0; j < cols; j++) {
      uint8_t s = state[i][j];

      if(j == 0) {
        sum_l = !!prevRow[cols - 1]
              + !!state[i][cols - 1]
              + !!((i == rows - 1)?firstRow:state[(i + 1) % rows])[cols - 1];
        sum_c = !!prevRow[0]
              + !!s
              + !!((i == rows - 1)?firstRow:state[(i + 1) % rows])[0];
      } else {
        sum_l = sum_c;
        sum_c = sum_r;
      }
      sum_r = !!prevRow[(j + 1) % cols]
            + !!state[i][(j + 1) % cols]
            + !!((i == rows - 1)?firstRow:state[(i + 1) % rows])[(j + 1) % cols];
      uint8_t sum = sum_r + sum_c + sum_l;
      state[i][j] = getNextState(s, sum);
      switch (state[i][j]) {
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
      Serial.println();
      memcpy(prevRow, curRow, cols);
  }
  Serial.println();
  delay(500);
}
