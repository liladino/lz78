#ifndef IO_H
#define IO_H

#include "bits.h"
#include <iostream>
#include <vector>

namespace io{
	void flush_bits_to_vector(bits& in, std::vector<uint8_t>& transformed);
	void flush_vector_uint8_to_stream(std::vector<uint8_t>& transformed, std::ostream& os);
}

#endif