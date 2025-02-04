#include "io.h"

namespace io{
	using my::vector;
	
	namespace out{
		void flush_bits_to_vector(bits& in, vector<uint8_t>& transformed){
			for (size_t i = 0; i < in.size()/8; i++){
				uint8_t current = in.at(i * 8);
				for (int j = 1; j < 8; j++){
					current <<= 1;
					current |= (uint8_t)(in.at(i * 8 + j));
				}
				transformed.push_back(current);
			}
		}
		
		void flush_vector_uint8_to_stream(vector<uint8_t>& transformed, std::ostream& os){		
			for (size_t i = 0; i < transformed.size(); i++){
				os << transformed[i];
			}
		}
	}
	
	namespace in{
		size_t read_bytes_from_file(std::istream& file, bits& buffer, size_t number_of_bytes){
			size_t i = 0;

			while (i < number_of_bytes) {
				uint8_t temp;
				if (file.get(reinterpret_cast<char&>(temp))) {
					buffer.push_ui8(temp);
				} 
				else {
					break;
				}
			}
			
			return i;
		}
		
		void handle_empty_buffer(std::istream& file, bits& buffer, int64_t& i){
			/* if the buffer is about to be empty, try to read in a kB new
			 * data. The content in this case is already in the current /
			 * output_buffer bit streams, so we can empty it. */
			buffer.clear();
			if (0 < io::in::read_bytes_from_file(file, buffer, 1024)){
				/* read new bits, and after this loop ends, start again
				 * at i == 0 */
				i = -1;
			}
			else {
				//couldn't read anything, the loop should exit next time
				i = 1;
			}
		}
		
		//~ void read_file_bytes(const std::string& filename, vector<uint8_t>& file_bytes) {
			//~ std::ifstream file(filename, std::ios::binary);
			//~ if (!file) {
				//~ throw std::runtime_error("Failed to open file: " + filename);
			//~ }

			//~ file >> std::noskipws;

			//~ file_bytes = vector<uint8_t>(
				//~ (std::istream_iterator<uint8_t>(file)), (std::istream_iterator<uint8_t>())
			//~ );
			
			//~ file.close();
		//~ }
		
		//~ void read_vector_to_bits(const vector<uint8_t>& file_bytes, bits& b){
			//~ for (const uint8_t x : file_bytes){
				//~ b.push_ui8(x);
			//~ }
		//~ }
	}
}