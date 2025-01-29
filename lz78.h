#ifndef LZ78_H
#define LZ78_H

#include "bits.h"
#include "codeword.h"
#include "compressor.h"
#ifdef DEBUG
	#include "tests.h"
#endif
#include <iostream>
#include <filesystem>
#include <string>
#include <cstring>
#include <fstream>

typedef enum compression_dir {
	compress,
	decompress
} compression_dir;


#endif