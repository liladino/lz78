#include "tests.h"

void tests(){
	#ifdef DEBUG
	//~ TEST(vector10, pop){
		//~ Vector<int> v;
		//~ for (int i = 1; i < 10; i++) v.push_back(i);
		//~ EXPECT_EQ((size_t)9, v.size());
		//~ v.pop_item(2);
		//~ EXPECT_EQ((size_t)8, v.size());
		//~ v.pop_item(2); //nincs valtozas
		//~ EXPECT_EQ((size_t)8, v.size());
		//~ EXPECT_EQ(1, v[0]);
		//~ for (size_t i = 1; i < v.size(); i++){
			//~ EXPECT_EQ((int) i+2, v[i]);
		//~ }
	//~ } ENDM
	TEST(io1, iotest){
		std::string s = "the quick brown fox jumps over the lazy dog";	
		bits message;
		for (size_t i = 0; i < s.size(); i++){
			message.push_ui8(s[i]);
		}
		std::vector<uint8_t> transformed;
		io::flush_bits_to_vector(message, transformed);
		bits back;
		for (const uint8_t& x : transformed){
			back.push_ui8(x);
		}
		
		EXPECT_TRUE(back == message);
	} ENDM	
	
	TEST(c1, compresssiontest){
		std::string s = "01000010100101010000110010", expectedstring = "000000010010011100111000010011101001110";	
		bits message, compressed, expected;
		for (size_t i = 0; i < s.size(); i++){
			message.push_bool(s[i] == '1' ? 1 : 0);
		}
		for (size_t i = 0; i < expectedstring.size(); i++){
			expected.push_bool(expectedstring[i] == '1' ? 1 : 0);
		}
		std::vector<codeword> code;
		
		compressor::break_down(message, code);
		compressor::set_compressed_message(code, compressed);
		
		//~ std::cout << compressed << std::endl << expected << std::endl;
		EXPECT_TRUE(compressed == expected);
	} ENDM	
	
	TEST(c2, compresssiontest){
		std::string s = "01000101001010001101011", expectedstring = "000000010010001110001011011001011101000";	
		bits message, compressed, expected;
		for (size_t i = 0; i < s.size(); i++){
			message.push_bool(s[i] == '1' ? 1 : 0);
		}
		for (size_t i = 0; i < expectedstring.size(); i++){
			expected.push_bool(expectedstring[i] == '1' ? 1 : 0);
		}
		std::vector<codeword> code;
		
		compressor::break_down(message, code);
		compressor::set_compressed_message(code, compressed);
		
		bits temp(compressed);
		uint8_t padding = compressor::add_padding(temp);
		
		//~ std::cout << compressed << std::endl << expected << std::endl;
		EXPECT_TRUE(compressed == expected);
		EXPECT_EQ(1, (int)padding);
	} ENDM	
	
	
	#endif
}