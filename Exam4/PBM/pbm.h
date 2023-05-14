#if!defined PBM_H
#define PGM_H

#include <fstream>
#include <vector>

using namespace std;

struct BinaryImage {
    int W;
    int H;
    std::vector<uint8_t> ImageData;

    bool ReadFromPBM(const std::string& filename);
};

#endif