#include "bits.h"
#include "codeword.h"
#include "compressor.h"
#include <map>

int main(){
	{
		int n = 1;
		if(*(char *)&n == 1) {
			std::cout << "We are on a little endian system!" << std::endl;
		}
	}

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