#include "compressor.h"

namespace compressor{	
	using my::vector;
	
	void break_down(std::istream& file, vector<codeword>& result){
		std::map<bits, uint64_t> known;
		uint64_t index = 1;
		bits buffer, current;
		
		io::in::read_bytes_from_file(file, buffer, 1024);

		for (int64_t i = 0; i < (int64_t)buffer.size(); i++){
			bool newbit = buffer.at(i);
			if (i == (int64_t)buffer.size() - 1){
				io::in::handle_empty_buffer(file, buffer, i);
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
	void compress(std::istream& in_file, std::ostream& out_file){	
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
	using my::vector;
	bool is_input_lz78(std::istream& file){
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
			
	int decompress(std::istream& in_file, std::ostream& out_file) {
		int warning_level = 0;
		in_file.read((char *)(&number_of_codewords), 8);
		in_file.read((char *)(&codeword_len), 8);
		in_file.read((char *)(&padding_info), 1);		
		
		warning_level = break_down(in_file, out_file);
		
		return warning_level;
	}
	
	void find_original_word(uint64_t address, vector<codeword>& known, bits& result){
		bits reversed;
		while (address != 0){
			reversed.push_bool(known[address].newbit);
			address = known[address].address;
		}
		
		while (reversed.size()){
			result.push_bool(reversed.last());
			reversed.pop();
		}
	}
	
	int break_down(std::istream& in_file, std::ostream& out_file){
		int warning_level = 0;		
		
		/* Vector of codewords found in order
		 * starts with a dummy element so we can use 1 based indexing */
		vector<codeword> known(1);
		bits input_buffer, current, output_buffer;
		
		io::in::read_bytes_from_file(in_file, input_buffer, 1024);
		
		const uint64_t codeword_bits = number_of_codewords * codeword_len;
		uint64_t read_bits_absolute = 0;

		int64_t i;
		for (i = 0; i < (int64_t)input_buffer.size() && read_bits_absolute < codeword_bits; i++, read_bits_absolute++){
			/* read every codeword, except the padding */
			
			bool newbit = input_buffer.at(i);
			
			if (i == (int64_t)input_buffer.size() - 1){
				io::in::handle_empty_buffer(in_file, input_buffer, i);
			}
			
			if (current.size() == codeword_len - 1 && read_bits_absolute < codeword_bits){
				/* strore the codeword found */ 
				codeword found(current.to_ui64(), newbit);
				known.push_back(found);
				
				bits temp;
				find_original_word(current.to_ui64(), known, temp);

				output_buffer.push_bits(temp);
				output_buffer.push_bool(newbit);
				current.clear();
			}
			else {
				current.push_bool(newbit);
			}
			
			if (output_buffer.size() % 8 == 0){
				out_file.write((const char*)output_buffer.get_data(), output_buffer.size_padded());
				output_buffer.clear();
			}
		}
		
		if (known.size() != number_of_codewords + 1){
			throw std::runtime_error("Unexpected end of file found!");
		}
		
		if (padding_info > 0){
			/* read in the remaining bits (if present), which correspond to a known sequence */
			io::in::read_bytes_from_file(in_file, input_buffer, INT_MAX);
			
			bits current, temp;
			
			if (input_buffer.size() - padding_info != codeword_len - 1){
				if (input_buffer.size() - padding_info < codeword_len - 1){
					std::cout << "Unexpected end of file found!" << std::endl;
					warning_level = 1;
				}
				else {
					std::cout << "Longer file than expected!" << std::endl;
					warning_level = 2;
				}
			}
			
			for (; i < (int64_t)std::min(input_buffer.size() - padding_info, codeword_len - 1); i++){
				current.push_bool(input_buffer.at(i));
			}
			
			if (current.to_ui64() != 0){
				/* there are k-1 zeroes if there was a whole number of codewords */
				find_original_word(current.to_ui64(), known, temp);
				output_buffer.push_bits(temp);
			}
		}
		
		if (output_buffer.size() > 0){
			output_buffer.add_padding();
		
			out_file.write((const char*)output_buffer.get_data(), output_buffer.size_padded());
			output_buffer.clear();
		}
		
		return warning_level;
	}
}
