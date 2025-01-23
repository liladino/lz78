#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "bits.h"
#include "codeword.h"
#include <map>
#include <vector>

namespace compressor{
	extern size_t codeword_len;
	void compress(const bits& message, std::vector<codeword>& result);
	uint64_t max_codeword_len(const std::vector<codeword>& result);
	void set_max_codeword_len(const std::vector<codeword>& result);
	void print_bit(const codeword& c);
	void print_bit(bits& b, const codeword& c);
	void get_compressed_message(bits& result, const std::vector<codeword>& codes);
}

namespace decoder{
	
	
}

#endif