# bitwise ops on embedded register cheat sheet
**General NOTES:**
- the individual bits in the register are 0 based
- bitshift left is really useful for setting values, bitshift right is really useful for reading values
- bitshift left can be though of as how many trailing zeros
    - Ex: N << 12 means we have 12 trailing zeros

### reset/clear specific bits in the register
- `reg &= ~(value << bitshift_amount)`
    - Ex: 3 << 12 == 0b11 << 12 == 0b0000 0000 0000 0000 0011 0000 0000 0000
    - now negate the binary == 0b1111 1111 1111 1111 1100 1111 1111 1111
    - now when you go to perform the bitwise AND op, everything but the one "bin" in the reg has been preserved

### setting bits in the register
- `reg |= (value << bitshift_amount)`
    - Ex: 3 << 12 == 0b11 << 12 == 0b0000 0000 0000 0000 0011 0000 0000 0000
    - now when you perform the bitwise OR, only the one "bin" in the reg has been updated
    - **NOTE** before doing this, it's smart to reset the bits you intend to change.
    For example, what if after setting the 12th bin above to 0b11, we instead needed to have it be 0b10
    If we were to just try reg |= (0b10 << 12) the "bin" would still show 0b11 since a bitwise OR 
    would preserve the existing binary since 1 | 0 is still 1. 

### reading bits in the register
- `(reg >> bitshift_amount) & value`
    - Ex: We have an 8 bit register that looks like: 0bXX10 XXXX
      we only care about reading the bits 4 and 5
      (reg >> 4) == 0000 XX10
      now bitwise AND with the desired value to check the bits
      pins = (reg >> 4) & 0x3; // 0x3 == 0b11
      pins == 0b10 // TRUE

### toggling bits in the register
- `reg ^= (value << bitshift_amount)`
    - Ex: we have a byte that looks like this XXXX 1XXX
      For the sake of this example, all of the bits besides the 4th (3rd on proper indexing)
      are reserved for other functions
      If we wanted to toggle this 4th bit, all we need to do is reg ^= (1 << 3)
      1 << 3 == 1000, and then bitwise XOR toggles the correct bit
      the existing values don't get messed up since 0 as in input to XOR ops just results in the 
      existing value whether that be 0 or 1

      **truth table**:
      | existing | input | existing ^ input |
      | -------- | ----- | :--------------: |
      |    0     |   0   |       0          |  // output is same as existing value
      |    0     |   1   |       1          |  // output is opposite of input
      |    1     |   0   |       1          |  // output is same as existing value
      |    1     |   1   |       0          |  // output is opposite of input

00000000 00000000 00000000 00000110  << 1
00000000 00000000 00000000 00001100  << 2
00000000 00000000 00000000 00011000  << 3
00000000 00000000 00000000 00110000  << 4
00000000 00000000 00000000 01100000  << 5
00000000 00000000 00000000 11000000  << 6
00000000 00000000 00000001 10000000  << 7
00000000 00000000 00000011 00000000  << 8
00000000 00000000 00000110 00000000  << 9
00000000 00000000 00001100 00000000  << 10
00000000 00000000 00011000 00000000  << 11
00000000 00000000 00110000 00000000  << 12
