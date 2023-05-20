#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "mat.h"
#include "ppm.h"


using namespace std;


/* LoadPPM utils */
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

/* SplitRGB utils */
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


/* Packbits utils */
void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded) {

	uint8_t last_num = img(0, 0), curr_num, rep = 0;
	vector<uint8_t> tmp;

	for (int r = 0; r < img.rows(); r++) {
		for (int c = 0; c < img.cols(); c++) {

			if ((r == 0) && (c == 0))
				continue;

			curr_num = img(r, c);


			if (curr_num == last_num) {

				/* Fine sequenza di caratteri differenti */
				if (tmp.size() != 0) {
					encoded.push_back((uint8_t)tmp.size() + 1);
					for (const auto& elem : tmp)
						encoded.push_back(elem);
					tmp.clear();
				}

				rep++;
			}

			else {
				/* Fine sequenza di caratteri ripetuti */
				if (rep != 0) {
					encoded.push_back(257 - rep);
					encoded.push_back(last_num);
					rep = 0;
				}

				tmp.push_back(last_num);
			}

			last_num = curr_num;
		}
	}



	/* Fine sequenza di caratteri differenti */
	if (tmp.size() != 0) {
		encoded.push_back((uint8_t)tmp.size() + 1);
		for (const auto& elem : tmp)
			encoded.push_back(elem);
		tmp.clear();
	}

	/* Fine sequenza di caratteri ripetuti */
	if (rep != 0) {
		encoded.push_back(257 - rep - 1);
		encoded.push_back(last_num);
		rep = 0;
	}

	tmp.push_back(last_num);



	/* Inserisco l'EOD */
	encoded.push_back(128);
}

/* Base64 utils */
uint8_t convert(char elem) {
	if ((elem >= 0) && (elem <= 25)) return elem + 'A';
	if ((elem >= 26) && (elem <= 51)) return elem + 'a' - 26;
	if ((elem >= 52) && (elem <= 61)) return elem + '0' - 52;
	if (elem == 62) return '+';
	if (elem == 63) return '/';
	return 64;
}

std::string Base64Encode(const std::vector<uint8_t>& v) {

	vector<uint8_t> v_new(v);
	uint8_t buffer = 0;
	uint8_t n = 0;
	string out;

	/* padding */
	if (v.size() % 3 != 0) {
		while (v_new.size() % 3 != 0)
			v_new.push_back(128);
	}


	for (auto& elem : v_new) {

		for (size_t i = 8; i > 0; i--) {
			buffer = (buffer << 1) | ((elem >> (i - 1)) & 1);
			n++;
			if (n == 6) {
				out += convert(buffer);
				buffer = 0;
				n = 0;
			}
		}

	}

	return out;
}


std::string JSON(const std::string& filename) {

	mat<vec3b> img;
	if (!LoadPPM(filename, img))
		return "{}";

	string out;

	out += "{\n\t";
	out += "\"width\": ";
	out += to_string(img.rows());
	out += ",\n\t";
	out += "\"heigh\": ";
	out += to_string(img.cols());
	out += ",\n\t";

	mat<uint8_t> r;
	mat<uint8_t> g;
	mat<uint8_t> b;

	SplitRGB(img, r, g, b);

	vector<uint8_t> encoded_r;
	vector<uint8_t> encoded_g;
	vector<uint8_t> encoded_b;

	PackBitsEncode(r, encoded_r);
	PackBitsEncode(g, encoded_g);
	PackBitsEncode(b, encoded_b);

	string encoded64_r;
	string encoded64_g;
	string encoded64_b;

	encoded64_r = Base64Encode(encoded_r);
	encoded64_g = Base64Encode(encoded_g);
	encoded64_b = Base64Encode(encoded_b);

	out += "\"red\": \"";
	out += encoded64_r;
	out += "\",\n\t";
	out += "\"green\": \"";
	out += encoded64_g;
	out += "\",\n\t";
	out += "\"blue\": \"";
	out += encoded64_b;
	out += "\"\n";
	out += "}";

	return out;
}

/*int main(int args, char** argv) {
	
	cout << JSON(argv[1]);
	
	return 0;
}*/