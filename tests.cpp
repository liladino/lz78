#include "tests.h"

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
}

void compressiontest(){
	stringst
	
	TEST(io1, iotest){
		
		
		std::string s = "the quick brown fox jumps over the lazy dog";	
		bits message;
		for (size_t i = 0; i < s.size(); i++){
			message.push_ui8(s[i]);
		}
		vector<uint8_t> transformed;
		io::out::flush_bits_to_vector(message, transformed);
		bits back;
		for (const uint8_t& x : transformed){
			back.push_ui8(x);
		}
		
		EXPECT_TRUE(back == message);
	} ENDM		
}

void tests(){
	#ifdef DEBUG
	
	vectortest();
	compressiontest();
	
	#endif
}