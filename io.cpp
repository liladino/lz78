#include "io.h"

namespace io{
	void flush_bits_to_vector(bits& in, std::vector<uint8_t>& transformed){
		for (size_t i = 0; i < in.size()/8; i++){
			uint8_t current = in.at(i * 8);
			for (int j = 1; j < 8; j++){
				current <<= 1;
				current |= (uint8_t)(in.at(i * 8 + j));
			}
			transformed.push_back(current);
		}
	}
	
	void flush_vector_uint8_to_stream(std::vector<uint8_t>& transformed, std::ostream& os){		
		for (size_t i = 0; i < transformed.size(); i++){
			os << transformed[i];
		}
	}
	
	
	//~ void read_input_to_bits(std::istream& is, bits& out){
		
	//~ }	
}