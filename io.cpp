#include "io.h"

namespace io{
	namespace out{
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
	}
	
	namespace in{
		void read_file_bytes(const std::string& filename, std::vector<uint8_t>& file_bytes) {
			std::ifstream file(filename, std::ios::binary);
			if (!file) {
				throw std::runtime_error("Failed to open file: " + filename);
			}

			file >> std::noskipws;

			file_bytes = std::vector<uint8_t>(
				(std::istream_iterator<uint8_t>(file)), (std::istream_iterator<uint8_t>())
			);
			
			file.close();
		}
		
		void read_vector_to_bits(const std::vector<uint8_t>& file_bytes, bits& b){
			for (const uint8_t x : file_bytes){
				b.push_ui8(x);
			}
		}
	}
		
	//~ void read_input_to_bits(std::istream& is, bits& out){
		
	//~ }	
}