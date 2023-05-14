#include "pbm.h"


void add(Image& img, uint8_t elem) {
	if (elem == 0)
		img.ImageData.push_back(0);
	else
		img.ImageData.push_back((elem | 255));
}



Image BinaryImageToImage(const BinaryImage& bimg)
{
	Image img;
	if ((bimg.H == NULL) || (bimg.W == NULL))
		return img;
 
	img.H = bimg.H;
	img.W = bimg.W;


	for (int r = 0; r < img.H; r++) {
		for (int c = 0; c < img.W; c++) {
			add(img, bimg.ImageData[r * img.W + c]);
		}
	}

	return img;
}
