#include "compressor.h"

namespace compressor{	
	using my::vector;
	
	void break_down(std::ifstream& file, vector<codeword>& result){
		std::map<bits, uint64_t> known;
		uint64_t index = 1;
		bits buffer, current;
		
		io::in::read_bytes_from_file(file, buffer, 1024);

		for (size_t i = 0; i < buffer.size(); i++){
			bool newbit = buffer.at(i);
			if (i == buffer.size() - 1){
				//if the buffer is about to be empty, try read in a kB new data
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
			
			current.push_bool(newbit);
			if (0 == known.count(current)){
				//int newbit = current.last();
				known[current] = index++;
				
				if (current.size() > 1){
					current.pop();				
					result.push_back(codeword(known[current], newbit));
				}
				else{				
					result.push_back(codeword(0, newbit));
				}
				current.clear();
			}
		}
		
		if (current.size() > 0) {
			/* Save the last couple of bits as a known sequence.
			 * It is known, because otherwise the algorithm would have 
			 * compressed it. */
			result.push_back(codeword(known[current], 0, true));
		}
		else {
			/* If everything has been compressed, save a zero address, so that
			 * the file format will be consistent*/
			result.push_back(codeword(0, 0, true));
		}
	}
	
	uint64_t codeword_len;
	
	uint64_t set_max_codeword_len(const vector<codeword>& result){	
		uint64_t mask = 1, max = 0, res;
		for (const codeword& c : result){
			if (c.address > max){
				max = c.address;
			}
		}
		for (uint64_t i = 1; i < 63; i++) {
			res = i;
			if ((mask << i) > max){
				break;
			}
		}
		
		codeword_len = res+1;
		
		return res+1;
	}
	
	void print_bit(const codeword& c) {
		std::cout << "("; //<< codeword_len << "  ";
		for (uint64_t mask = 1LLU << (codeword_len-2); mask != 0; mask >>= 1){
			std::cout << ((mask & c.address) == 0 ? 0 : 1);
		}
		std::cout << " " << (int)(c.newbit) << ")";
	}
	
	void write_codeword_to_bits(const codeword& c, bits& b) {
		for (uint64_t mask = 1LLU << (codeword_len-2); mask != 0; mask >>= 1){
			b.push_bool((mask & c.address) == 0 ? 0 : 1);
		}
		if (!c.remaining) b.push_bool(c.newbit);
	}
	
	void set_compressed_data(const vector<codeword>& codes, bits& result){
		set_max_codeword_len(codes);
		for (const codeword& c : codes){
			bits b;
			write_codeword_to_bits(c, b);
			result.push_bits(b);
		}
	}
	
	uint8_t add_padding(bits& result){
		if (result.size() % 8 == 0) return 0;
		size_t x = 8 - (result.size() % 8);
		for (size_t i = 0; i < x; i++){
			result.push_bool(0);
		}
		
		return x;
	}
	
	//expects two in binary mode open files 
	void compress(std::ifstream& in_file, std::ofstream& out_file){	
		bits compressed_part;
		vector<codeword> code;
		
		break_down(in_file, code);
		set_compressed_data(code, compressed_part);
		
		uint64_t number_of_codewords = code.size();
		set_max_codeword_len(code); 
		/* Padding: 0 bits, if divisible by 8, otherwise 8 - remainder. */
		uint8_t padding_info = (8 - (((codeword_len % 8) * (number_of_codewords % 8)) % 8)) % 8;
		add_padding(compressed_part);
		
		static const char* format_info = "lz78"; 
		
		out_file.write(              format_info,                 4 * sizeof(char));		
		out_file.write((const char*)(&number_of_codewords),       sizeof(uint64_t));
		out_file.write((const char*)(&codeword_len),              sizeof(uint64_t));
		out_file.write((const char*)(&padding_info),              sizeof(uint8_t));
		out_file.write((const char*)(compressed_part.get_data()), compressed_part.size_padded());
	}
}


namespace decoder{
	bool is_input_lz78(std::ifstream& file){
		char buffer[5] = {0};
		file.read (buffer, 4);
		if (file){
			if (strncmp(buffer, "lz78", 4) == 0){
				return true;
			}
		}
		return false;
	}
	uint64_t number_of_codewords = 0;
	uint64_t codeword_len = 0;
	uint8_t padding_info = 0;
			
	void decompress(std::ifstream& in_file, std::ifstream& out_file) {
		in_file.read((char *)(&number_of_codewords), 8);
		in_file.read((char *)(&codeword_len), 8);
		in_file.read((char *)(&padding_info), 1);		
	}
	
	void break_down(std::ifstream& file, std::ifstream& out_file){
		/* Map: address and new bits */
		std::map<uint64_t, bool> known;
		
		uint64_t index = 1;
		bits input_buffer, current, output_buffer;
		
		io::in::read_bytes_from_file(file, buffer, 1024);
		
		const uint64_t codeword_bits = number_of_codewords * codeword_len;
		uint64_t read_bits_absolute = 0;

		for (size_t i = 0; i < buffer.size() && read_bits_absolute < codeword_bits; i++, read_bits_absolute++){
			bool newbit = buffer.at(i);
			if (i == buffer.size() - 1){
				//if the buffer is about to be empty, try read in a kB new data
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
			
			if (current.size() == codeword_len - 1 && read_bits_absolute < codeword_bits){
				bits temp;
				/* find_the_original_word(temp)*/
				output_buffer.push_bits(temp);
				output_buffer.push_bool(newbit);
			}
			else {
				current.push_bool(newbit);
			}
			
			if (/* output buffer big enough */){
				/* write the buffer into file *
				 * empty buffer */
			}
		}
		
		if (/* output buffer not empty */){
			/* write out remainder */
		}
	}
}
