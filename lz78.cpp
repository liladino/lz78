#include "bits.h"
#include "codeword.h"
#include "compressor.h"
#include <map>

/*
 * LZ78
 * 
 * example:
 * 0 1 00 001 01 0010 10 100 0011 0
 * 
 * num   code  uncompressed
 * 0001  0000  0
 * 0010  0001  1
 * 0011  0010  00
 * 0100  0111  001 
 * 0101  0011  01
 * 0110  1000  0010
 * 0111  0100  10
 * 1000  1110  100
 * 1001  1001  0011
 * 
 * output:
 * 0000 0001 0010 0111 0011 1000 0100 1110 1001 0
 * 
 * FILE FORMAT
 * first 64 bits: number of codewords in file (n)
 * coding info 64 bits: length of each codeword (k = len(i)+1, as i is the address, c is the new bit, (i, c))
 * padding info 8 bit: number of padding bits at the end of the file
 * next k*n bits: codewords
 * remaining: the remaining bits are a known sequence of bits, so the file ends with that address.
 */

int main(){
	std::string s = "01000010100101010000110";
	
	std::vector<codeword> compressed;
	bits message;
	for (size_t i = 0; i < s.size(); i++){
		message.push(s[i] == '1' ? 1 : 0);
	}
	
	compressor::compress(message, compressed);
		
	for (const codeword& c : compressed){
		std::cout << c << " ";
	}
	std::cout << std::endl << compressor::maxCodeWordLength(compressed) << std::endl;
	
	return 0;
}