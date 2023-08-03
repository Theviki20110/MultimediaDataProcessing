#include "mat.h"
#include "ppm.h"
#include <vector>
#include <fstream>

using namespace std;

void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded) {

	uint8_t last_num, curr_num, rep = 1;
	vector<uint8_t> tmp;
	
	for (int r = 0; r < img.rows(); r++) {
		for (int c = 0; c < img.cols(); c++) {

			if ((r == 0) && (c == 0)) {
				last_num = img(r, c);
				continue;
			}

			curr_num = img(r, c);

			/*CASO1: caratteri uguali*/
			if (curr_num == last_num) {

				if (tmp.size() != 0) {

					/* CASO1.2: Fine sequenza di caratteri differenti */					
					encoded.push_back((uint8_t)tmp.size() - 1);
					
					for (const auto& elem : tmp)
						encoded.push_back(elem);
					
					tmp.clear();
				}

				rep++;
			}

			else {
				/*CASO2: Fine sequenza di caratteri ripetuti */
				if (rep != 1) {
					encoded.push_back(257 - rep);
					encoded.push_back(last_num);
					last_num = curr_num;
					rep = 1;
					continue;
				}

				tmp.push_back(last_num);
			}

			last_num = curr_num;
		}
	}
	
	tmp.push_back(curr_num);
	/* Fine sequenza di caratteri ripetuti */
	if (rep != 1) {
		encoded.push_back(257 - rep);
		encoded.push_back(last_num);
	}
	/* Fine sequenza di caratteri differenti */
	if (tmp.size() != 1) {
		encoded.push_back((uint8_t)tmp.size() - 1);
		for (const auto& elem : tmp)
			encoded.push_back(elem);
		tmp.clear();
	}

	/* Inserisco l'EOD */
	encoded.push_back(128);
}
