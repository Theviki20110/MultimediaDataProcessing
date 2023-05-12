#if!defined PGM16_H
#define PGM16_H

#include <string>
#include <fstream>
#include <iostream>
#include "mat.h"
using namespace std;

bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue);

#endif