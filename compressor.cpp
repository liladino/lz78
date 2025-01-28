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
		out_file.write((const char*)(&number_of_codewords),       1 * sizeof(uint64_t));
		out_file.write((const char*)(&codeword_len),              1 * sizeof(uint64_t));
		out_file.write((const char*)(&padding_info),              1 * sizeof(uint8_t));
		out_file.write((const char*)(compressed_part.get_data()), compressed_part.size_padded());
	}
}


namespace decoder{
	
	
}
