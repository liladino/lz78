#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "bits.h"
#include "codeword.h"
#include <map>
#include <vector>

namespace compressor{
	void compress(const bits& message, std::vector<codeword>& result);
	uint64_t maxCodeWordLength(std::vector<codeword>& result);
}

namespace decoder{
	
	
}

#endif