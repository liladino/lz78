#include "compressor.h"

namespace compressor{	
	void compress(const bits& message, std::vector<codeword>& result){
		std::map<bits, uint64_t> known;
		uint64_t index = 1;
		bits b;
		
		for (size_t i = 0; i < message.size(); i++){
			b.push(message.at(i));
			if (0 == known.count(b)){
				int newbit = b.at(b.size()-1);
				known[b] = index++;
				
				if (b.size() > 1){
					b.pop();				
					result.push_back(codeword(known[b], newbit));
				}
				else{				
					result.push_back(codeword(0, newbit));
				}
				b.clear();
			}
		}
		if (b.size() > 0) {
			/* Save the last couple of bits as a known sequence.
			 * It is known, because otherwise the algorithm would have 
			 * compressed it. */
			result.push_back(codeword(known[b], 0, true));
		}
	}
	
	
	uint64_t maxCodeWordLength(std::vector<codeword>& result){	
		uint64_t mask = 1, max = 0, res;
		for (const codeword& c : result){
			if (c.address > max){
				max = c.address;
			}
		}
		for (uint64_t i = 1; i < 63; i++) {
			res = i;
			if ((mask << i) > max){
				break;
			}
		}
		
		return res + 1;
	}
}


namespace decoder{
	
	
}
