#ifndef HUFSTR_H
#define HUFSTR_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

struct elem {
	uint8_t _sym;
	uint8_t _len = 0;
	uint32_t _code = 0;
	elem() = default; 
	elem(const uint8_t& _sym, const uint8_t& _len, const uint32_t& _code)
		: _sym(_sym), _len(_len), _code(_code) {}
};

class hufstr {
	elem find(uint8_t sym) const;
	uint8_t reverse_find(uint32_t code, uint8_t len) const;


public:
	std::vector<uint8_t> compress(const std::string& s) const;
	std::string decompress(const std::vector<uint8_t>& v) const;
	vector<elem> _table;
	hufstr();
};
#endif  
