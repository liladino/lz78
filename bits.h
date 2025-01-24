#ifndef BITS_H
#define BITS_H 

#include <vector>
#include <iostream>
#include <string>


class bits {
private:
	std::vector<bool> v;
	
public:
	bits(){};
	bits(std::vector<bool>& v) : v(v){}
	bits(const bits& b){
		v = b.v;
	}
	//~ bits(bool b) {
		//~ append(b);
	//~ }
	
	bits& operator=(const bits& other){
		v = other.v;
		return *this;
	}

	void clear(){
		v.clear();
	}

	void push_bool(bool b){
		v.push_back(b);
	}
	
	void push_bits(const bits& other){
		for (size_t i = 0; i < other.v.size(); i++){
			push_bool(other.at(i));
		}
	}
	
	void push_ui64(uint64_t n){
		for (uint64_t mask = 1LLU << 63; mask != 0; mask >>= 1){
			push_bool((mask & n) == 0 ? 0 : 1);
		}
	}
	
	void push_ui8(uint8_t n){
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
	
	bool at(size_t i) const {
		return v.at(i);
	}
	
	bool operator[](size_t i){
		return v.at(i);
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