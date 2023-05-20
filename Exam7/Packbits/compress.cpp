#include "mat.h"
#include "ppm.h"
#include <vector>
//#include <fstream>

using namespace std;

void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded) {

	uint8_t last_num = img(0, 0), curr_num, rep = 0;
	vector<uint8_t> tmp;

	for (int r = 0; r < img.rows(); r++) {
		for (int c = 1; c < img.cols(); c++) {


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

	rep++;



	/* Fine sequenza di caratteri ripetuti */
	if (rep != 0) {
		encoded.push_back(257 - rep);
		encoded.push_back(last_num);
		rep = 0;
	}

	tmp.push_back(last_num);



	/* Inserisco l'EOD */
	encoded.push_back(128);
}

/*int main(int args, char** argv) {
	
	mat<vec3b> img;

	if (!LoadPPM(argv[1], img))
		return 1;

	mat<uint8_t> r;
	mat<uint8_t> g;
	mat<uint8_t> b;

	SplitRGB(img, r, g, b);
	
	vector<uint8_t> encoded;

	PackBitsEncode(g, encoded);
	
	return 0;
}*/