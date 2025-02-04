#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "bits.h"
#include "codeword.h"
#include "io.h"
#include "vector.h"
#include <map>
#include <iostream>
#include <iomanip>
#include <climits>

namespace compressor{
	extern uint64_t codeword_len;
	void break_down(std::istream& file, my::vector<codeword>& result);
	uint64_t set_max_codeword_len(const my::vector<codeword>& result);
	void print_bit(const codeword& c);
	void write_codeword_to_bits(const codeword& c, bits& b);
	void set_compressed_message(const my::vector<codeword>& codes, bits& result);
	uint8_t add_padding(bits& result);
	void compress(std::istream& in_file, std::ostream& out_file);
}

namespace decoder{
	extern uint64_t number_of_codewords;
	extern uint64_t codeword_len;
	extern uint8_t padding_info;
	bool is_input_lz78(std::istream& file);
	int decompress(std::istream& in_file, std::ostream& out_file) ;
	void find_original_word(uint64_t address, my::vector<codeword>& known, bits& result);
	int break_down(std::istream& in_file, std::ostream& out_file);
}

#endif