#include "mat.h"
#include "ppm.h"
#include <fstream>

using namespace std;

void readTwoBytes(ifstream& is, mat<vec3b>& img) {
	uint16_t value;
	for (int r = 0; r < img.rows(); r++) {
		for (int c = 0; c < img.cols(); c++) {
			for (int i = 0; i < 3; i++) {
				is.read(reinterpret_cast<char*>(&value), sizeof(uint16_t));
				img(r, c)[i] = value;
			}
		}
	}

}


bool LoadPPM(const std::string& filename, mat<vec3b>& img) {

	ifstream is(filename, ios::binary);
	if (is.fail())
		return false;

	/* Read MagicNumber */
	string magic_number;
	is >> magic_number;
	if (magic_number != "P6")
		return false;

	/* Read whitespace */
	is.get();

	/* Eventually, read comment */
	if (is.peek() == '#') {
		string comment;

		/* Read '#' */
		is.get();

		while (is.peek() != '\n')
			is >> comment;
	}

	/* Read other parameters */
	int width, height, maximum_color_value;

	is >> width;
	is >> height;

	img.resize(height, width);

	is >> maximum_color_value;

	if ((maximum_color_value < 0) || (maximum_color_value > 65536))
		return false;

	/* Read whitespace */
	is.get();

	/* Read image values with maximum_color_value < 256 */
	if (maximum_color_value < 256)
		is.read(img.rawdata(), img.rawsize());
	else
		readTwoBytes(is, img);



	return true;
}

void SplitRGB(const mat<vec3b>& img, mat<uint8_t>& img_r, mat<uint8_t>& img_g, mat<uint8_t>& img_b) {

	img_r.resize(img.rows(), img.cols());
	img_g.resize(img.rows(), img.cols());
	img_b.resize(img.rows(), img.cols());
	
	for (int r = 0; r < img.rows(); r++) {
		for (int c = 0; c < img.cols(); c++) {
			img_r(r, c) = img(r, c)[0];
			img_g(r, c) = img(r, c)[1];
			img_b(r, c) = img(r, c)[2];
		}
	}
}

int main(int args, char** argv) {

	mat<vec3b> img;

	if (!LoadPPM(argv[1], img))
		return 1;
	mat<uint8_t> img_r;
	mat<uint8_t> img_g;
	mat<uint8_t> img_b;

	SplitRGB(img, img_r, img_g, img_b);

	return 0;
}