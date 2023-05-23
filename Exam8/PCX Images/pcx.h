#if!defined PCX_H
#define PCX_H
#include "mat.h"
#include <string>
#include <fstream>

using namespace std;

bool load_pcx(const std::string& filename, mat<uint8_t>& img);

#endif