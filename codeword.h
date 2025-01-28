#ifndef CODEWORD_H
#define CODEWORD_H

#include <iostream>

struct codeword {	
	uint64_t address = 0;
	bool newbit = 0;
	bool remaining = false;
	
	codeword(){}
	codeword(uint64_t address, bool newbit) : address(address), newbit(newbit), remaining(false){}
	codeword(uint64_t address, bool newbit, bool remaining) : address(address), newbit(newbit), remaining(remaining){}
	
	friend std::ostream& operator<<(std::ostream& os, const codeword& c);
};

inline std::ostream& operator<<(std::ostream& os, const codeword& c){
	if (!(c.remaining)){
		os << "(" << c.address << ", " << (int)(c.newbit) << ")";
	}
	else {
		os << "(" << c.address << ")";
	}
	return os;
}

#endif
