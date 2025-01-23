# LZ78

This is an implementation of the LZ78 lossless compression algorithm described by A. Lempel and J. Ziv.
This program is purely for educational pruposes, the code is not guaranteed to be flawless, and should not be used in critical places. 

## Example

The code is broken down into bit sequences of increasing length, where each sequence is 1 bit longer then the last known sequence. Each sequence get a number (incrementing from 1), and a codeword. The codeword consists of the number (address) of the sequence 1 bit shorter, and the new bit. An example is given below.

Input:
`0100001010010101000011`

Compression:

```
0 1 00 001 01 0010 10 100 0011
num   code  uncompressed
0001  0000  0
0010  0001  1
0011  0010  00
0100  0111  001 
0101  0011  01
0110  1000  0010
0111  0100  10
1000  1110  100
1001  1001  0011
```

Output:
`0000 0001 0010 0111 0011 1000 0100 1110 1001`

## File format
This program uses it's own file format, descibed here. The compressed files have the extension `.lz78`. The format expects little endian, and so does this program.
 
 * first 64 bits: number of codewords in file (n)
 * coding info 64 bits: length of each codeword (k = len(i)+1, as i is the address, c is the new bit, (i, c))
 * padding info 8 bit: number of padding bits at the end of the file
 * next k*n bits: codewords
 * remaining: the remaining bits are a known sequence of bits, so the file ends with that address.