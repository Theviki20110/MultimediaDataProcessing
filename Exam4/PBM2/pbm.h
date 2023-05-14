#if!defined PBM_H
#define PBM_H

#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
struct BinaryImage {
    int W;
    int H;
    std::vector<uint8_t> ImageData;
};

struct Image {
    int W;
    int H;
    std::vector<uint8_t> ImageData;
};

Image BinaryImageToImage(const BinaryImage& bimg);

#endif