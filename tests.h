#ifndef TESTS_H
#define TESTS_H

#include "bits.h"
#include "compressor.h"
#include "io.h"
#include "vector.h"
#include <iostream>
#include <string>
#include <sstream>

#ifdef DEBUG
	#include "gtest_lite.h"
#endif

void tests();

#endif