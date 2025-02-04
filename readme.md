# LZ78

This is an implementation of the LZ78 lossless compression algorithm described by A. Lempel and J. Ziv.
This program is purely for educational purposes, the code is not guaranteed to be flawless, and should not be used in critical places. 

## Compression

The bit stream is broken down into bit sequences of increasing length, where each sequence is 1 bit longer then the last known sequence. Each sequence get a number (incrementing from 1), and a codeword. Each sequence is encoded into a codeword, where the codeword consists of the address of the sequence 1 bit shorter and the new bit. An example is given below.

### Note

This format currently supports individual files only. The algorithm is suited for files with clear patterns of repetition (HTML, XML), but even then it's compression ratio is terrible compared to other algorithms. 

## Example

### Compression

Input:
`01000010100101010000110010`

Breaking down:

```
0 1 00 001 01 0010 10 100 0011 0010
addr  code  uncompressed bit sequence
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

At the end, `0010` is already known, so we represent it with it's address only. As seen in this example, the addresses can be longer than what we actually need - if the longest addresses ( `1000` and `1001` here ) are not used in any codeword, leading zeroes can be omitted. In this case this means that unambigous encoding can be achieved with only 3 bit long addresses, despite there being more than `2^3-1` bit sequences. 

Output:
`0000 0001 0010 0111 0011 1000 0100 1110 1001 110`

### Decompression

Input:
`
0001000000100110100110101101
`

Breaking down using 3 bit long addresses:

```
0001 0000 0010 0110 1001 1010 1101
addr  code  uncompressed bit sequence
001   0001  1
010   0000  0
011   0010  10
100   0110  100
101   1001  1001
110   1010  10010
111   1101  100101 
```

Original code:
`
1010100100110010100101
`

## File format

This program uses its own file format, described here. The compressed files have the extension `.lz78`. The format expects little endian, and so does this program.
 
| Field                | Size       | Description                                                                 |
|----------------------|------------|-----------------------------------------------------------------------------|
| Format Information   | 32 bits    | Fixed string: the characters 'l' 'z' '7' and '8'                            |
| Length Info          | 64 bits    | Number of codewords in the file ( `n` ).                                    |
| Coding Info          | 64 bits    | Length of each codeword (`k = len(i) + 1`, where `i` is the address).       |
| Padding Info         |  8 bits    | Number of padding bits at the end of the file.                              |
| Codewords            | `n * k`    | Compressed data stored as codewords ( `n*(address + new bit)` ).            |
| Remaining Bits' Info | `k-1`      | An already known address, a 'not new codeword'.                             |
| Padding              | Variable   | Zeroes added to ensure the file size is a multiple of 8 bits (1 byte).      |
 
The remaining bits' info should be `k-1` zeroes, if the bit stream was a whole number of codewords. Otherwise, the remaining bits at the end of the stream are a known sequence of bits, and as such could be represented with an address of a known sequence. See the example.

## Build and Run 

### Requirements for the program

 * A C++17 (or later) compiler.
 * Little-endian machine (most modern systems).

The codebase includes a makefile, you can compile the program with:

`make all`

## Usage

Run the program: 
`./lz78.out example.txt`

By default the program is in compression mode, use `-d` to change that. If no output name was specified, 

 * in compression mode the output is `[original filename].lz78`, `example.txt.lz78` in the example
 * in decompression mode
   * if the input name ends with `.lz78`, that will be cut off, `example.txt.lz78` becomes `example.txt`
   * otherwise the output will be `out_[original filename]`, `example.txt` becomes `out_example.txt`

List of flags:

| Flag             | Description                                 |
|------------------|---------------------------------------------|
| `-o [filename]`  | Set output filename                         |
| `-d`             | Decompress mode                             |
| `-k`             | Remove the input file after (de)compression |
| `-h` or `--help` | List of commands and how to                 |
| `-t`             | Run tests. Only works in debug compilation  |