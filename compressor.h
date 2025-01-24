#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "bits.h"
#include "codeword.h"
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>

namespace compressor{
	extern size_t codeword_len;
	void break_down(const bits& message, std::vector<codeword>& result);
	uint64_t set_max_codeword_len(const std::vector<codeword>& result);
	//~ void set_max_codeword_len(const std::vector<codeword>& result);
	void print_bit(const codeword& c);
	void write_codeword_to_bits(const codeword& c, bits& b);
	void set_compressed_message(const std::vector<codeword>& codes, bits& result);
	uint8_t add_padding(bits& result);
	void compress(const bits& message, bits& result);
}

namespace decoder{
	
	
}

#endif