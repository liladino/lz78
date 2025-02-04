#ifndef IO_H
#define IO_H

#include "bits.h"
#include "vector.h"
#include <iostream>
#include <iterator>
#include <iostream>
#include <string>

namespace io{
	namespace out{
		void flush_bits_to_vector(bits& in, my::vector<uint8_t>& transformed);
		void flush_vector_uint8_to_stream(my::vector<uint8_t>& transformed, std::ostream& os);
	}
	namespace in{
		size_t read_bytes_from_file(std::istream& file, bits& buffer, size_t number_of_bytes);
		void handle_empty_buffer(std::istream& file, bits& buffer, int64_t& i);
	}
}

#endif