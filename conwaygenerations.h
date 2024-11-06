/*
ConwayGenerations is a library for running John Conway's Game of Life
ConwayGenerations keeps track of how long a cell has been alive
ConwayGenerations is optimized for memory usage targetting microcontrollers.
Copyright Ali Raheem 2024 - https://github.com/ali-raheem/ConwayGenerations
MIT Licensed
File version: 2024-11-06 10:13 GMT
*/

template<int rows, int cols>
class ConwayGenerations {
public:
  ConwayGenerations(uint8_t (&state)[rows][cols], uint8_t bits)
    : state(state), generation(0), bits(bits), colCells(8 / bits), mask((1 << bits) - 1) {}
  uint8_t (&state)[rows][cols];
  uint16_t generation;
  uint8_t bits;
  uint8_t colCells;
  uint8_t mask;
  uint8_t getCell(uint8_t* stateRow, uint8_t cellIndex) {
    uint8_t i = cellIndex / colCells;
    uint8_t pos = cellIndex % colCells;
    uint8_t shift = bits * pos;
    return (stateRow[i] >> shift) & mask;
  }
  
  void setCell(uint8_t* stateRow, uint8_t cellIndex, uint8_t value) {
    uint8_t i = cellIndex / colCells;
    uint8_t pos = cellIndex % colCells;
    uint8_t shift = bits * pos;
    uint8_t mask_shifted = mask << shift;
    value &= mask;
    stateRow[i] = (stateRow[i] & ~mask_shifted) | (value << shift);
  }
  
  uint8_t getTotalCellsInRow() const {
    return cols * colCells;
  }
  void next() {
    uint8_t prevRow[cols];
    uint8_t firstRow[cols];
    uint8_t curRow[cols];
    
    memcpy(firstRow, state[0], cols);
    memcpy(prevRow, state[rows - 1], cols);
    
    for(int i = 0; i < rows; i++) {
      memcpy(curRow, state[i], cols);
      uint8_t* nextRow = (i == rows - 1) ? firstRow : state[i + 1];
      
      uint8_t sum_l = 0, sum_c = 0, sum_r = 0;
      
      uint8_t totalCells = colCells * cols;
      sum_l = !!getNibble(prevRow, totalCells - 1)
	+ !!getNibble(curRow, totalCells - 1)
	+ !!getNibble(nextRow, totalCells - 1);
      sum_c = !!getNibble(prevRow, 0)
	+ !!getNibble(curRow, 0)
	+ !!getNibble(nextRow, 0);
      
      for(int j = 0; j < totalCells; j++) {
	size_t nextJ = (j + 1) % totalCells;
	sum_r = !!getNibble(prevRow, nextJ)
	  + !!getNibble(curRow, nextJ)
	  + !!getNibble(nextRow, nextJ);
	
	uint8_t currentState = getNibble(curRow, j);
	uint8_t s = cgmin(getNextState(currentState, sum_l + sum_c + sum_r), mask);
	setNibble(state[i], j, s);
	
	sum_l = sum_c;
	sum_c = sum_r;
      }
      memcpy(prevRow, curRow, cols);
    }
    generation++;
  }
  
private:
  template <typename T, typename U>
  inline uint8_t cgmin(T x, U cap) {
    T tcap = static_cast<T>(cap);
    return (x < tcap) ? x : tcap;
  }
  
  uint8_t getNibble(uint8_t* state, uint8_t nibble) {
    uint8_t i = nibble / colCells;
    uint8_t pos = nibble % colCells;
    uint8_t shift = bits * pos;
    return (state[i] >> shift) & mask;
  }
  
  void setNibble(uint8_t* state, uint8_t nibble, uint8_t value) {
    uint8_t i = nibble / colCells;
    uint8_t pos = nibble % colCells;
    uint8_t shift = bits * pos;
    uint8_t mask_shifted = mask << shift;
    value &= mask;
    state[i] = (state[i] & ~mask_shifted) | (value << shift);
  }
  
  uint8_t getNextState(uint8_t s, uint8_t sum) {
    switch(sum) {
    case 3:
      return s + 1;
    case 4:
      return (s == 0) ? 0 : s + 1;
    default:
      return 0;
    }
  }
};
