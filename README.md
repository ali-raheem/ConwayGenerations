# Conway Generations

Simple game of life implementation in which each generation that the cell remains a live it intensifies.

This library allows you to specify how many bits are used per cell, right down to a single bit (which is only alive/dead tracking).

Bits can be set to 1, 2, 4, 8 depending on what you want allowing for more complex production of display.

## Representative SRAM usage by bits (32x32 board)

| Bits | SRAM usage |
+------+------------+
| 8    | 61%        |
| 4    | 36%        |
| 2    | 23%        |
| 1    | 17%        |

The library uses 3 line buffers to save on memory usage, so using 2 bits per cell the Arduino code will only use about 23bytes SRAM for a 32x32 display.


## Display

See [Conway](https://github.com/ali-raheem/conway/) for a low footprint basic game of life library that does not track generation count. This is equivalent to bits = 1 for conwaygenerations, but is slightly more optimized.



Watch a run on [Youtube](https://youtu.be/VNp8FQVHZYA)

![An example running in Serial on Arduino](./screenshot-arduino.png)