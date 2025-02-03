#ifndef BITS_H
#define BITS_H 

#include "vector.h"
#include <iostream>
#include <string>


class bits {
private:
	my::vector<bool> v;
	
public:
	bits(){};
	bits(my::vector<bool>& v) : v(v){}
	bits(const bits& b){
		v = b.v;
	}
	
	bits& operator=(const bits& other){
		v = other.v;
		return *this;
	}

	void clear(){
		v.clear();
	}
	
	void push_bool(const bool b){
		v.push_back(b);
	}
	
	void push_bits(const bits& other){
		for (size_t i = 0; i < other.v.size(); i++){
			push_bool(other.at(i));
		}
	}
	
	void push_ui64(const uint64_t n){
		for (uint64_t mask = 1LLU << 63; mask != 0; mask >>= 1){
			push_bool((mask & n) == 0 ? 0 : 1);
		}
	}  
	
	void push_ui8(const uint8_t n){
		for (uint8_t mask = 1 << 7; mask != 0; mask >>= 1){
			push_bool((mask & n) == 0 ? 0 : 1);
		}
	}
	
	void pop(){
		v.pop_back();
	}
		
	size_t size() const{
		return v.size();
	}
	size_t size_padded() const{
		return v.size_in_ui8();
	}
	
	bool last() const {
		return v.at(v.size()-1);
	}
	
	bool at(size_t i) const {
		return v.at(i);
	}
	
	uint64_t to_ui64() const{
		const uint8_t* const  data = v.get_data();
		size_t size = v.size_in_ui8();
		
		uint64_t result = 0;
		for (size_t i = (size-1 > 7 ? 7 : size-1); i != 0; i--){
			result <<= 8;
			result += data[i];
		}		
		
		return result;
	}
	
	void add_padding(){
		v.add_padding();
	}
	
	const uint8_t* get_data() const {
		return v.get_data();
	}
	
	bool operator<(const bits& other) const{
		if (v.size() != other.v.size()) return v.size() < other.v.size();
		for (size_t i = 0; i < std::min(v.size(), other.v.size()); i++){
			if ((int)v[i] < (int)other.v[i]){
				return false;
			}
			else if ((int)v[i] > (int)other.v[i]){
				return true;
			}
		}
		return false;
	}
	
	bool operator==(const bits& oths) const {
		if (oths < *this) return false;
		if (*this < oths) return false;
		return true;
	}
	
	friend std::ostream& operator<<(std::ostream& os, const bits& b);
};

inline std::ostream& operator<<(std::ostream& os, const bits& b){
	for (size_t i = 0; i < b.v.size(); i++){
		os << (int)(b.v[i]);
	}
	return os;	
}

#endif