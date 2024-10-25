#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <string>
#include <cstdlib>

using namespace std;

const int rows = 32, cols = 32;

uint8_t getNextState(uint8_t curState, uint sum) {
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

int main() {
  
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  mt19937 generator(seed);
  uniform_int_distribution<uint8_t> distribution(0, 1);
  uint8_t state[rows][cols][2];
  uint flip = 0;

  uint i, j;

  for(i = 0; i < rows; i++) {
    for(j = 0; j < cols; j++) {
      state[i][j][0] = distribution(generator);
      state[i][j][1] = distribution(generator);
      cout << ((state[i][j][flip^1] > 0)?" - " : "   ");
    }
    cout << endl;
  }
  cout << endl;

  while (1) {
    cout << "\033[2J\033[H";
    for(i = 0; i < rows; i++) {
      for(j = 0; j < cols; j++) {
	uint sum = !!state[(i-1) % rows][(j-1) %cols][flip]
	  + !!state[(i-1) % rows][j][flip]
	  + !!state[(i-1) % rows][(j+1)%cols][flip]
	  + !!state[i][(j-1) % cols][flip]
	  + !!state[i][j][flip]
	  + !!state[i][(j+1) % cols][flip]
	  + !!state[(i+1) % rows][(j-1) % cols][flip]
	  + !!state[(i+1) % rows][j][flip]
	  + !!state[(i+1) % rows][(j+1) % cols][flip];
	
	state[i][j][flip^1] = getNextState(state[i][j][flip], sum);
	switch (state[i][j][flip^1]) {
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
    flip ^= 1;
    cout << endl;
    this_thread::sleep_for(chrono::milliseconds(200));
  }
}
