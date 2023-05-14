#include "pbm.h"

Image BinaryImageToImage(const BinaryImage& bimg)
{
	Image img;
	img.H = bimg.H;
	img.W = bimg.W;


	for (int r = 0; r < img.W; r++) {
		for (int c = 0; c < img.H; c++) {
			cout << bimg.ImageData[r * img.W + c];
			if (bimg.ImageData[r * img.W + c] == 0)
				img.ImageData[r * img.W + c] = 0;
			else 
				img.ImageData[r * img.W + c] = ((img.ImageData[r * img.W + c] << 6) | 255);
		}
	}

	return img;
}
