#include "bits.h"
#include "codeword.h"
#include "compressor.h"
#include <map>

int main(){
	{
		int n = 1;
		if(*(char *)&n != 1) {
			std::cout << "Currently big endian systems are not supported" << std::endl;
			return 0;
		}
		//~ else {
			//~ std::cout << "We are on a little endian system!" << std::endl;
		//~ }
	}

	std::string s = "01000101001010001101011";
	
	std::vector<codeword> compressed;
	bits message;
	for (size_t i = 0; i < s.size(); i++){
		message.push(s[i] == '1' ? 1 : 0);
	}
	
	compressor::compress(message, compressed);
	compressor::set_max_codeword_len(compressed);
	
	for (const codeword& c : compressed){
		compressor::print_bit(c);
		std::cout << " ";
	}
	std::cout << std::endl;
	
	bits result;
	compressor::get_compressed_message(result, compressed);
	std::cout << result << std::endl;
	return 0;
}