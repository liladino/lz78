#ifndef IO_H
#define IO_H

#include "bits.h"
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>

namespace io{
	namespace out{
		void flush_bits_to_vector(bits& in, std::vector<uint8_t>& transformed);
		void flush_vector_uint8_to_stream(std::vector<uint8_t>& transformed, std::ostream& os);
	}
	namespace in{
		
	}
}

#endif