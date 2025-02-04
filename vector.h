#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#ifdef DEBUG
	#include "memtrace.h"
#endif
#include "string.h"

namespace my{
	template<typename T>
	class vector{
		T* data;
		size_t length;
		size_t capacity;
		public:
		
		vector(size_t n = 0) : length(n), capacity(n + 2) {
			data = new T[capacity]();
		}
		vector(size_t n, T defElement) : length(n), capacity(n + 2) {
			data = new T[capacity]();
			for (size_t i = 0; i < length; i++){
				data[i] = defElement;
			}
		}
		vector& operator=(const vector<T>& v){
			if (v.data == data) return *this;
			deep_clear();
			
			length = v.size();
			if (v.capacity != 0) capacity = v.capacity; //cleared vector
			else capacity = 2;
			data = new T[capacity]();
			for (size_t i = 0; i < v.size(); i++){
				data[i] = v[i];
			}
			return *this;
		}
		vector(const vector<T>& v){
			*this = v;
		}
		
		~vector() {
			if (capacity > 0) {
				delete[] data;
				data = nullptr;
			}
		}

		void push_back(const T& element) {
			if (length == capacity){
				size_t new_capac = capacity > 0 ? 2 * capacity : 2;
				T* newData = new T[new_capac]();
				for (size_t i = 0; i < length; ++i) {
					newData[i] = data[i];
				}
				if (capacity > 0) delete[] data;
				capacity = new_capac;
				data = newData;
			}
			data[length++] = element;
		}
		
		void pop_back(){
			length--;
		}
		
		size_t size() const {
			return length;
		}
		
		T& at(size_t index) {
			if (index >= length) throw std::out_of_range("Index out of range");
			return data[index];
		}
		const T& at(size_t index) const {
			if (index >= length) throw std::out_of_range("Index out of range");
			return data[index];
		}
		
		T& operator[](size_t index) {
			return at(index);
		}
		const T& operator[](size_t index) const {
			return at(index);
		}
		
		void deep_clear(){
			if (capacity > 0) {
				delete[] data;
				data = nullptr;
			}
			length = 0;
			capacity = 0;
		}
		
		void clear(){
			//doesn't actually delete the contents, but marks them as reusable
			length = 0;
		}

		void resize(const size_t newLen){
			if (newLen <= length) {
				length = newLen;
				return;
			}
			T* newData = new T[newLen + 2]();
			for (size_t i = 0; i < newLen && i < length; i++){
				newData[i] = data[i];
			}
			if (capacity > 0) delete[] data;
			data = newData;
			
			length = newLen;
			capacity = newLen + 2;
		}
		
		T* get_data() {
			return data;
		}
		
		const T* get_data() const  {
			return data;
		}
		
		/*
		 * Iterators
		 */
		class iterator {
			T* ptr;
		public:
			explicit iterator(T* p = nullptr) : ptr(p) { }

			T& operator*() { return *ptr; }
			T* operator->() { return ptr; }

			iterator& operator++() {
				++ptr;
				return *this;
			}

			iterator operator++(int) {
				iterator temp = *this;
				++ptr;
				return temp;
			}

			bool operator==(const iterator& other) const { return ptr == other.ptr; }
			bool operator!=(const iterator& other) const { return ptr != other.ptr; }
		};
		
		class const_iterator {
			const T* ptr;
		public:
			explicit const_iterator(const T* p = nullptr) : ptr(p) { }
			const T& operator*() const { return *ptr; }
			const T* operator->() const { return ptr; }
			
			const_iterator& operator++() {
				++ptr;
				return *this;
			}

			const_iterator operator++(int) {
				const_iterator temp = *this;
				++ptr;
				return temp;
			}

			bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
			bool operator!=(const const_iterator& other) const { return ptr != other.ptr; }
		};

		iterator begin() { return iterator(data); }
		iterator end() { return iterator(data + length); }
		const_iterator begin() const { 	return const_iterator(data); }
		const_iterator end() const { return const_iterator(data + length); }
	};
	
	/*
	 * Bool specification
	 */
	template <>
	class vector<bool> {
		vector<uint8_t> data;
		size_t length;
	public:
		vector<bool>(size_t n = 0) : data((n + 7) / 8, 0), length(n) { }
		
		vector<bool>(const vector<bool>& v) : data(v.data), length(v.length) { }
    
		~vector<bool>() = default;

		void push_back(const bool b) {
			if (data.size() <= length / 8){
				data.push_back(0);
			}
			/* delete the bit to make sure theres no garbage there */
			data.at(length / 8) &= ~(1 << (7 - length % 8));
			if (b){
				/* add new bit */
				data.at(length / 8) |= 1 << (7 - length % 8); 
			}
			length++;
		}
		
		void pop_back(){
			length--;
		}
		
		void add_padding(){
			if (length % 8){
				length += 8 - (length % 8);
			}
		}
		
		size_t size() const {
			return length;
		}
		
		size_t size_in_ui8() const {
			return data.size();
		}
		
		bool at(size_t index) const {
			if (index < length) {
				bool temp = (data.at(index / 8) & (1 << (7 - index % 8))) != 0;
				return temp;
			} else {
				throw std::out_of_range("Index out of range");
			}
		}
		
		bool operator[](size_t index) const {
			return at(index);
		}
		
		void clear(){
			length = 0;
		}
		
		vector<bool>& operator=(const vector<bool>& v){
			data = v.data;
			length = v.length;
			return *this;
		}
				
		uint8_t* get_data() {
			return data.get_data();
		}
		
		const uint8_t* get_data() const {
			return data.get_data();
		}
	};
}

#endif

