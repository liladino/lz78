#include "bits.h"
#include "codeword.h"
#include "compressor.h"
#ifdef DEBUG
	#include "tests.h"
#endif
#include <map>

int main(){
	{
		int n = 1;
		if(*(char *)&n != 1) {
			std::cout << "Currently big endian systems are not supported" << std::endl;
			return 0;
		}
	}
	#ifdef DEBUG
	tests();
	#endif

	std::string s = "01000010100101010000110010";
	
	bits message, result;
	for (size_t i = 0; i < s.size(); i++){
		message.push_bool(s[i] == '1' ? 1 : 0);
	}
	
	compressor::compress(message, result);
	
	std::cout << "\nResult: " << result << std::endl;
	return 0;
}