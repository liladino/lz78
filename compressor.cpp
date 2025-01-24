#include "compressor.h"

namespace compressor{	
	void break_down(const bits& message, std::vector<codeword>& result){
		std::map<bits, uint64_t> known;
		uint64_t index = 1;
		bits b;
		
		for (size_t i = 0; i < message.size(); i++){
			b.push_bool(message.at(i));
			if (0 == known.count(b)){
				int newbit = b.at(b.size()-1);
				known[b] = index++;
				
				if (b.size() > 1){
					b.pop();				
					result.push_back(codeword(known[b], newbit));
				}
				else{				
					result.push_back(codeword(0, newbit));
				}
				b.clear();
			}
		}
		if (b.size() > 0) {
			/* Save the last couple of bits as a known sequence.
			 * It is known, because otherwise the algorithm would have 
			 * compressed it. */
			result.push_back(codeword(known[b], 0, true));
		}
	}
	
	uint64_t set_max_codeword_len(const std::vector<codeword>& result){	
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
	
	//~ void set_max_codeword_len(const std::vector<codeword>& result){	
		//~ codeword_len = max_codeword_len(result);
	//~ }
	
	size_t codeword_len;
	
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
		b.push_bool(c.newbit);
	}
	
	void set_compressed_message(const std::vector<codeword>& codes, bits& result){
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
	
	void compress(const bits& message, bits& result){	
		bits compressed_part;
		std::vector<codeword> code;
		
		break_down(message, code);
		set_compressed_message(code, compressed_part);
		
		uint64_t number_of_codewords = code.size();
		uint64_t codeword_len = set_max_codeword_len(code); 
		uint8_t padding_info = add_padding(compressed_part);
		
		result.push_ui8('l');
		result.push_ui8('z');
		result.push_ui64(number_of_codewords);
		result.push_ui64(codeword_len);
		result.push_ui8(padding_info);
		
		#ifdef DEBUG
		std::cout << "Meta info:   " << result << "\n                  l       z";
		std::cout << std::setw(64) << number_of_codewords;
		std::cout << std::setw(64) << codeword_len;
		std::cout << std::setw(8) << (int)padding_info << std::endl;
		#endif
		
		result.push_bits(compressed_part);
		
		#ifdef DEBUG
		std::cout << "Compression: " << compressed_part << std::endl;
		#endif
	}
}


namespace decoder{
	
	
}
