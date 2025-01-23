#ifndef BITS_H
#define BITS_H 

#include <vector>
#include <iostream>
#include <string>


class bits {
private:
	std::vector<bool> v;
	bool operator==(const bits& oths) const {
		if (oths < *this) return false;
		if (*this < oths) return false;
		return true;
	}
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

	void push(bool b){
		v.push_back(b);
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
	
	friend std::ostream& operator<<(std::ostream& os, const bits& b);
};

inline std::ostream& operator<<(std::ostream& os, const bits& b){
	for (size_t i = 0; i < b.v.size(); i++){
		os << (int)(b.v[i]);
	}
	return os;	
}

#endif