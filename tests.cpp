#include "tests.h"

#ifdef DEBUG
void vectortest(){
	using my::vector;
	
	TEST(vector1, constr){
		vector<int> v;
		EXPECT_EQ((size_t)0, v.size()) << "Size not 0." << std::endl;
	} ENDM
	TEST(vector2, constr){
		vector<int> v(10);
		EXPECT_EQ((size_t)10, v.size()) << "Size not 10." << std::endl;
		for (size_t i = 0; i < 10; i++) {
			EXPECT_EQ((int)0, v[i]) << "Not initialized to 0." << std::endl;
			v[i] = i;
		}
	} ENDM
	TEST(vector3, index) {
		vector<int> a(10);
		EXPECT_NO_THROW(a[0]);
		EXPECT_NO_THROW(a[7]);
		EXPECT_NO_THROW(a[8]);
		EXPECT_THROW(a[10], std::out_of_range&);
		EXPECT_THROW(a[11], std::out_of_range&);
		EXPECT_THROW(a[-1], std::out_of_range&);
    } ENDM
	TEST(vector4, constr){
		vector<size_t> v(10, 1);
		for (size_t i = 0; i < 10; i++) {
			EXPECT_EQ((size_t)1, v[i]);
			v[i] = i;
		}
		vector<size_t> w = v;
		for (size_t i = 0; i < 10; i++) EXPECT_EQ(i, w[i]);
	} ENDM
	TEST(vector6, eq){
		vector<int> v(10, 1);
		vector<int> w = v;
		EXPECT_NE(&v, &w) << "Not a deep copy" << std::endl;
	} ENDM
	TEST(vector7, pushb){
		vector<int> v;
		v.push_back(2);
		EXPECT_EQ((size_t)1, v.size());
		v.push_back(4);
		EXPECT_EQ((size_t)2, v.size());
		v.push_back(6);
		EXPECT_EQ((size_t)3, v.size());
		v.push_back(8);
		EXPECT_EQ((size_t)4, v.size());
		for (size_t i = 0; i < v.size(); i++){
			EXPECT_EQ((int)(i+1)*2, v[i]);
		}
	} ENDM
	TEST(vector8, resize){
		vector<int> v(10, 1); //1 1 1 1 1...
		v.resize(2); // 1 1 
		EXPECT_EQ((size_t)2, v.size());
		v.resize(4); // 1 1 0 0
		EXPECT_EQ((size_t)4, v.size());
		v.push_back(2); // 1 1 0 0 2
		EXPECT_EQ((size_t)5, v.size());
		for (size_t i = 0; i < v.size()-1; i++){
			EXPECT_EQ(1 - (int)i/2, v[i]); //1 1 0 0
		}
		EXPECT_EQ(2, v[4]);
	} ENDM
	TEST(vector9, clear){
		vector<int> v(10, 1);
		v.clear();
		EXPECT_EQ((size_t)0, v.size());
		v.resize(4);
		EXPECT_EQ((size_t)4, v.size());
		v.clear();
		EXPECT_EQ((size_t)0, v.size());
		v.push_back(2); 
		EXPECT_EQ((size_t)1, v.size());
	} ENDM
	TEST(vector10, bool1){
		vector<bool> v(8);
		EXPECT_EQ((size_t)8, v.size());
		v.clear();
		EXPECT_EQ((size_t)0, v.size());
		v.push_back(0);
		EXPECT_EQ((size_t)1, v.size());
		v.push_back(1); 
		EXPECT_EQ((size_t)2, v.size());
	} ENDM
	TEST(vector10, bool2){
		vector<bool> v;
		v.push_back(1);
		EXPECT_EQ(true,  v[0]);
		v.push_back(0);
		EXPECT_EQ(false, v[1]);
		v.push_back(1);
		EXPECT_EQ(true,  v[2]);
		v.push_back(0);
		EXPECT_EQ(false, v[3]);
		v.push_back(1);
		EXPECT_EQ(true,  v.at(4));
		v.push_back(0);
		EXPECT_EQ(false, v.at(5));
		v.push_back(1);
		EXPECT_EQ(true,  v.at(6));
		v.push_back(1);
		EXPECT_EQ(true,  v.at(7));
		
		EXPECT_EQ(0b10101011, (int)v.get_data()[0]);
	} ENDM
	TEST(bits1, push1){
		bits b;
		b.push_ui64(0b00010010'10010001'00100100'10001010'01101011'00001111'11110000'01010101);
		const uint8_t* v = b.get_data(); 
		EXPECT_EQ(0b00010010, (int)v[0]);
		EXPECT_EQ(0b10010001, (int)v[1]);
		EXPECT_EQ(0b00100100, (int)v[2]);
		EXPECT_EQ(0b10001010, (int)v[3]);
		EXPECT_EQ(0b01101011, (int)v[4]);
		EXPECT_EQ(0b00001111, (int)v[5]);
		EXPECT_EQ(0b11110000, (int)v[6]);
		EXPECT_EQ(0b01010101, (int)v[7]);
	} ENDM
	TEST(bits1, convert){
		bits b;
		b.push_bool(1);
		EXPECT_EQ(1llu, b.to_ui64());
		b.push_bool(1);
		EXPECT_EQ(3llu, b.to_ui64());
		b.push_bool(0);
		EXPECT_EQ(6llu, b.to_ui64());
		b.push_bool(0);
		EXPECT_EQ(12llu, b.to_ui64());
		b.push_bool(1);
		EXPECT_EQ(25llu, b.to_ui64());
		b.push_bool(1);
		EXPECT_EQ(51llu, b.to_ui64());
		b.push_bool(0);
		EXPECT_EQ(102llu, b.to_ui64());
		b.push_bool(0);
		EXPECT_EQ(204llu, b.to_ui64());
		b.push_bool(0);
		EXPECT_EQ(408llu, b.to_ui64());
		b.push_bool(1);
		EXPECT_EQ(817llu, b.to_ui64());
		b.push_bool(1);
		EXPECT_EQ(1635llu, b.to_ui64());
		b.push_bool(1);
		EXPECT_EQ(3271llu, b.to_ui64());
		b.push_bool(1);
		EXPECT_EQ(6543llu, b.to_ui64());
		b.push_ui8(1);
		EXPECT_EQ(1675009llu, b.to_ui64());
		b.push_ui8(1);
		EXPECT_EQ(428802305llu, b.to_ui64());
		b.push_ui8(0);
		EXPECT_EQ(109773390080llu, b.to_ui64());
	} ENDM
}

void compressiontest(){  
	TEST(comp, c1){
		/* Compress the example in example in readme padded to be 32 bits and whole
		 * codewords.
		 * ........ ........ ........ ..******
		 * 01000010 10010101 00001100 10000101
		 * 
		 * 0 1 00 001 01 0010 10 100 0011 00100 00101
		 * addr  code  uncompressed bit sequence
		 * 0001  0000  0
		 * 0010  0001  1
		 * 0011  0010  00
		 * 0100  0111  001 
		 * 0101  0011  01
		 * 0110  1000  0010
		 * 0111  0100  10
		 * 1000  1110  100
		 * 1001  1001  0011
		 * 1010  1100  00100
		 * 1011  1101  00101
		 * 
		 * number_of_codewords = 11
		 * codeword_len = 4
		 * padding_info = 1
		 * 
		 * (length of section in bits, conent)
		 * output: (64, lz78) (64, 11) (64, 4) (8, 1) (44, 0000 0001 0010 0111 0011 1000 0100 1110 1001 1100 1101) (3, 000) (1, 0)
		 * 
		 * */
		std::ostringstream f(std::ios::binary);
		const uint8_t data1[4] = {0b01000010, 0b10010101, 0b00001100, 0b10000101};
		f.write(reinterpret_cast<const char*>(data1), 4);

		std::istringstream in_file(f.str(), std::ios::binary);
		std::ostringstream temp(std::ios::binary);
		
		EXPECT_NO_THROW(compressor::compress(in_file, temp));
		
		std::istringstream out_file(temp.str(), std::ios::binary);
		EXPECT_TRUE(io::in::is_input_lz78(out_file));
		
		EXPECT_EQ((size_t)27, out_file.str().size());
		
		uint64_t number_of_codewords;
		uint64_t codeword_len;
		uint8_t padding_info;
		
		out_file.read((char *)(&number_of_codewords), 8);
		EXPECT_EQ(11llu, number_of_codewords);
		
		out_file.read((char *)(&codeword_len), 8);
		EXPECT_EQ(4llu, codeword_len);
		
		out_file.read((char *)(&padding_info), 1);
		EXPECT_EQ(1, (int)padding_info);
		
		const uint8_t expect_res[] = {0b00000001, 0b00100111, 0b00111000, 0b01001110, 0b10011100, 0b11010000};
		char actual_res[7] = {0};
		
		out_file.read(actual_res, 6);
		EXPECT_TRUE(strncmp((const char *)expect_res, actual_res, 6) == 0);
	} ENDM
}


void decompressiontest(){
	TEST(decomp, d1){
		/* Decompress the data from the first compression test 
		 * 
		 * */
				
		uint8_t bit_stream[27] = "lz78"; //27 WITHOUT TERMINATING 0
		uint64_t number_of_codewords = 11;
		uint64_t codeword_len = 4;
		uint8_t padding_info = 1;
		const uint8_t compressed_part[] = {0b00000001, 0b00100111, 0b00111000, 0b01001110, 0b10011100, 0b11010000};
		memcpy(bit_stream +  4, (&number_of_codewords), 8);
		memcpy(bit_stream + 12, (&codeword_len), 8);
		memcpy(bit_stream + 20, (&padding_info), 1);
		memcpy(bit_stream + 21, compressed_part, 6);
		
		std::ostringstream f(std::ios::binary);
		f.write(reinterpret_cast<const char*>(bit_stream), 27);

		std::istringstream in_file(f.str(), std::ios::binary);
		std::ostringstream temp(std::ios::binary);
		
		//~ std::cout << bit_stream << std::endl;
		EXPECT_TRUE(io::in::is_input_lz78(in_file));
		
		int returnval = 0;
		EXPECT_NO_THROW({returnval = decoder::decompress(in_file, temp);});
		EXPECT_EQ(0, returnval);
		
		std::istringstream out_file(temp.str(), std::ios::binary);
		
		EXPECT_EQ((size_t)4, out_file.str().size());
		
		const uint8_t expect_res[4] = {0b01000010, 0b10010101, 0b00001100, 0b10000101};
		char actual_res[5] = {0};
		
		out_file.read(actual_res, 4);
		EXPECT_TRUE(strncmp((const char *)expect_res, actual_res, 4) == 0);
	} ENDM
}

#endif

void tests(){
	#ifdef DEBUG
	
	vectortest();
	compressiontest();
	decompressiontest();
	
	#endif
}