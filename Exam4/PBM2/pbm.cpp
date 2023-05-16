#include "pbm.h"

void add(Image& img, uint8_t elem) {
	if (elem)
		img.ImageData.push_back(0);
	else
		img.ImageData.push_back(255);
}

Image BinaryImageToImage(const BinaryImage& bimg)
{
	Image img;
	if ((bimg.H == NULL) || (bimg.W == NULL))
		return img;
 
	img.H = bimg.H;
	img.W = bimg.W;

	int index = 0;
 	for (int r = 0; r < img.H; r++) {
		for (int c = 0; c < img.W/8; c++) {
			for (size_t i = 8; i > 0; i--) 
				add(img, ((bimg.ImageData[index] >> (i - 1)) & 1));
			index++;

		}
		if (img.W % 8 != 0) {

			for (size_t i = 0; i < (size_t)img.W % 8; i++)
				add(img, (bimg.ImageData[index] >> (7 - i)) & 1);

			index++;
		}
	}

	return img;
}

/*int main(int args, char** argv) {

	BinaryImage img;
	img.ReadFromPBM(argv[1]);
	Image cimg = BinaryImageToImage(img);

	return 0;
}*/