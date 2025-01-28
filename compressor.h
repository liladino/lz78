#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "bits.h"
#include "codeword.h"
#include "io.h"
#include "vector.h"
#include <map>
#include <iostream>
#include <iomanip>

namespace compressor{
	extern size_t codeword_len;
	void break_down(std::ifstream& file, my::vector<codeword>& result);
	uint64_t set_max_codeword_len(const my::vector<codeword>& result);
	void print_bit(const codeword& c);
	void write_codeword_to_bits(const codeword& c, bits& b);
	void set_compressed_message(const my::vector<codeword>& codes, bits& result);
	uint8_t add_padding(bits& result);
	void compress(std::ifstream& in_file, std::ofstream& out_file);
}

namespace decoder{
	
	
}

#endif