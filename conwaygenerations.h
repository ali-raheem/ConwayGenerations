template<int rows, int cols>
class ConwayGenerations {
public:
    ConwayGenerations(uint8_t (&state)[rows][cols])
        : state(state), generation(0) {}

    uint16_t generation;

    void next() {
        uint8_t prevRow[cols];
        uint8_t firstRow[cols];
        uint8_t curRow[cols];

	memcpy(firstRow, state[0], cols);
        memcpy(prevRow, state[rows - 1], cols);
	// No fancy C++ for you arduino...
        // std::copy(std::begin(state[0]), std::end(state[0]), firstRow);
        // std::copy(std::begin(state[rows - 1]), std::end(state[rows - 1]), prevRow);

        for(int i = 0; i < rows; i++) {
	  memcpy(curRow, state[i], cols);
          //  std::copy(std::begin(state[i]), std::end(state[i]), curRow);
            uint8_t* nextRow = (i == rows - 1) ? firstRow : state[i + 1];

            uint8_t sum_l = 0, sum_c = 0, sum_r = 0;

            sum_l = !!getNibble(prevRow, 2 * cols - 1)
                  + !!getNibble(curRow, 2 * cols - 1)
                  + !!getNibble(nextRow, 2 * cols - 1);
            sum_c = !!getNibble(prevRow, 0)
                  + !!getNibble(curRow, 0)
                  + !!getNibble(nextRow, 0);

            for(int j = 0; j < 2 * cols; j++) {
                size_t nextJ = (j + 1) % (2 * cols);
                sum_r = !!getNibble(prevRow, nextJ)
                      + !!getNibble(curRow, nextJ)
                      + !!getNibble(nextRow, nextJ);

                uint8_t currentState = getNibble(curRow, j);
                uint8_t s = cgmin(getNextState(currentState, sum_l + sum_c + sum_r), 0b1111);
                setNibble(state[i], j, s);

                sum_l = sum_c;
                sum_c = sum_r;
            }
	    memcpy(prevRow, curRow, cols);
	    //            std::copy(std::begin(curRow), std::end(curRow), prevRow);
        }
        generation++;
    }

private:
  uint8_t (&state)[rows][cols];
  
  template <typename T, typename U>
  inline uint8_t cgmin(T x, U cap) {
    T tcap = static_cast<T>(cap);
    return (x < tcap)? x : tcap;
  }
  uint8_t getNibble(uint8_t* state, uint8_t nibble) {
    uint8_t i = nibble / 2;
    if (nibble % 2 == 0) {
      return (state[i] >> 4) & 0x0F;
    } else {
      return state[i] & 0x0F;
    }
  }
  
  void setNibble(uint8_t* state, uint8_t nibble, uint8_t value) {
    uint8_t i = nibble / 2;
    value &= 0x0F;
        if (nibble % 2 == 0) {
	  state[i] = (state[i] & 0x0F) | (value << 4);
        } else {
	  state[i] = (state[i] & 0xF0) | value;
        }
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
