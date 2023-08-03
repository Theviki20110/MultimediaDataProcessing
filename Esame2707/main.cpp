#include <fstream>
#include <iostream>
#include <string>
#include <map>


using namespace std;

map<uint8_t, string> table = {
{0b00000000, 	"sml_d"},
{0b00000001, 	"sml_d"},
{0b00000010, 	"sml_d"},
{0b00000011, 	"sml_d"},
{0b00000100, 	"sml_d"},
{0b00000101, 	"sml_d"},
{0b00000110, 	"eos"},
{0b00000111, 	"lrg_d"},
{0b00001000, 	"sml_d"},
{0b00001001, 	"sml_d"},
{0b00001010, 	"sml_d"},
{0b00001011, 	"sml_d"},
{0b00001100, 	"sml_d"},
{0b00001101, 	"sml_d"},
{0b00001110, 	"nop"},
{0b00001111, 	"lrg_d"},
{0b00010000, 	"sml_d"},
{0b00010001, 	"sml_d"},
{0b00010010, 	"sml_d"},
{0b00010011, 	"sml_d"},
{0b00010100, 	"sml_d"},
{0b00010101, 	"sml_d"},
{0b00010110, 	"nop"},
{0b00010111, 	"lrg_d"},
{0b00011000, 	"sml_d"},
{0b00011001, 	"sml_d"},
{0b00011010, 	"sml_d"},
{0b00011011, 	"sml_d"},
{0b00011100, 	"sml_d"},
{0b00011101, 	"sml_d"},
{0b00011110, 	"udef"},
{0b00011111, 	"lrg_d"},
{0b00100000, 	"sml_d"},
{0b00100001, 	"sml_d"},
{0b00100010, 	"sml_d"},
{0b00100011, 	"sml_d"},
{0b00100100, 	"sml_d"},
{0b00100101, 	"sml_d"},
{0b00100110, 	"udef"},
{0b00100111, 	"lrg_d"},
{0b00101000, 	"sml_d"},
{0b00101001, 	"sml_d"},
{0b00101010, 	"sml_d"},
{0b00101011, 	"sml_d"},
{0b00101100, 	"sml_d"},
{0b00101101, 	"sml_d"},
{0b00101110, 	"udef"},
{0b00101111, 	"lrg_d"},
{0b00110000, 	"sml_d"},
{0b00110001, 	"sml_d"},
{0b00110010, 	"sml_d"},
{0b00110011, 	"sml_d"},
{0b00110100, 	"sml_d"},
{0b00110101, 	"sml_d"},
{0b00110110, 	"udef"},
{0b00110111, 	"lrg_d"},
{0b00111000, 	"sml_d"},
{0b00111001, 	"sml_d"},
{0b00111010, 	"sml_d"},
{0b00111011, 	"sml_d"},
{0b00111100, 	"sml_d"},
{0b00111101, 	"sml_d"},
{0b00111110, 	"udef"},
{0b00111111, 	"lrg_d"},
{0b01000000, 	"sml_d"},
{0b01000001, 	"sml_d"},
{0b01000010, 	"sml_d"},
{0b01000011, 	"sml_d"},
{0b01000100, 	"sml_d"},
{0b01000101, 	"sml_d"},
{0b01000110, 	"pre_d"},
{0b01000111, 	"lrg_d"},
{0b01001000, 	"sml_d"},
{0b01001001, 	"sml_d"},
{0b01001010, 	"sml_d"},
{0b01001011, 	"sml_d"},
{0b01001100, 	"sml_d"},
{0b01001101, 	"sml_d"},
{0b01001110, 	"pre_d"},
{0b01001111, 	"lrg_d"},
{0b01010000, 	"sml_d"},
{0b01010001, 	"sml_d"},
{0b01010010, 	"sml_d"},
{0b01010011, 	"sml_d"},
{0b01010100, 	"sml_d"},
{0b01010101, 	"sml_d"},
{0b01010110, 	"pre_d"},
{0b01010111, 	"lrg_d"},
{0b01011000, 	"sml_d"},
{0b01011001, 	"sml_d"},
{0b01011010, 	"sml_d"},
{0b01011011, 	"sml_d"},
{0b01011100, 	"sml_d"},
{0b01011101, 	"sml_d"},
{0b01011110, 	"pre_d"},
{0b01011111, 	"lrg_d"},
{0b01100000, 	"sml_d"},
{0b01100001, 	"sml_d"},
{0b01100010, 	"sml_d"},
{0b01100011, 	"sml_d"},
{0b01100100, 	"sml_d"},
{0b01100101, 	"sml_d"},
{0b01100110, 	"pre_d"},
{0b01100111, 	"lrg_d"},
{0b01101000, 	"sml_d"},
{0b01101001, 	"sml_d"},
{0b01101010, 	"sml_d"},
{0b01101011, 	"sml_d"},
{0b01101100, 	"sml_d"},
{0b01101101, 	"sml_d"},
{0b01101110, 	"pre_d"},
{0b01101111, 	"lrg_d"},
{0b01110000, 	"udef"},
{0b01110001, 	"udef"},
{0b01110010, 	"udef"},
{0b01110011, 	"udef"},
{0b01110100, 	"udef"},
{0b01110101, 	"udef"},
{0b01110110, 	"udef"},
{0b01110111, 	"udef"},
{0b01111000, 	"udef"},
{0b01111001, 	"udef"},
{0b01111010, 	"udef"},
{0b01111011, 	"udef"},
{0b01111100, 	"udef"},
{0b01111101, 	"udef"},
{0b01111110, 	"udef"},
{0b01111111, 	"udef"},
{0b10000000, 	"sml_d"},
{0b10000001, 	"sml_d"},
{0b10000010, 	"sml_d"},
{0b10000011, 	"sml_d"},
{0b10000100, 	"sml_d"},
{0b10000101, 	"sml_d"},
{0b10000110, 	"pre_d"},
{0b10000111, 	"lrg_d"},
{0b10001000, 	"sml_d"},
{0b10001001, 	"sml_d"},
{0b10001010, 	"sml_d"},
{0b10001011, 	"sml_d"},
{0b10001100, 	"sml_d"},
{0b10001101, 	"sml_d"},
{0b10001110, 	"pre_d"},
{0b10001111, 	"lrg_d"},
{0b10010000, 	"sml_d"},
{0b10010001, 	"sml_d"},
{0b10010010, 	"sml_d"},
{0b10010011, 	"sml_d"},
{0b10010100, 	"sml_d"},
{0b10010101, 	"sml_d"},
{0b10010110, 	"pre_d"},
{0b10010111, 	"lrg_d"},
{0b10011000, 	"sml_d"},
{0b10011001, 	"sml_d"},
{0b10011010, 	"sml_d"},
{0b10011011, 	"sml_d"},
{0b10011100, 	"sml_d"},
{0b10011101, 	"sml_d"},
{0b10011110, 	"pre_d"},
{0b10011111, 	"lrg_d"},
{0b10100000, 	"med_d"},
{0b10100001, 	"med_d"},
{0b10100010, 	"med_d"},
{0b10100011, 	"med_d"},
{0b10100100, 	"med_d"},
{0b10100101, 	"med_d"},
{0b10100110, 	"med_d"},
{0b10100111, 	"med_d"},
{0b10101000, 	"med_d"},
{0b10101001, 	"med_d"},
{0b10101010, 	"med_d"},
{0b10101011, 	"med_d"},
{0b10101100, 	"med_d"},
{0b10101101, 	"med_d"},
{0b10101110, 	"med_d"},
{0b10101111, 	"med_d"},
{0b10110000, 	"med_d"},
{0b10110001, 	"med_d"},
{0b10110010, 	"med_d"},
{0b10110011, 	"med_d"},
{0b10110100, 	"med_d"},
{0b10110101, 	"med_d"},
{0b10110110, 	"med_d"},
{0b10110111, 	"med_d"},
{0b10111000, 	"med_d"},
{0b10111001, 	"med_d"},
{0b10111010, 	"med_d"},
{0b10111011, 	"med_d"},
{0b10111100, 	"med_d"},
{0b10111101, 	"med_d"},
{0b10111110, 	"med_d"},
{0b10111111, 	"med_d"},
{0b11000000, 	"sml_d"},
{0b11000001, 	"sml_d"},
{0b11000010, 	"sml_d"},
{0b11000011, 	"sml_d"},
{0b11000100, 	"sml_d"},
{0b11000101, 	"sml_d"},
{0b11000110, 	"pre_d"},
{0b11000111, 	"lrg_d"},
{0b11001000, 	"sml_d"},
{0b11001001, 	"sml_d"},
{0b11001010, 	"sml_d"},
{0b11001011, 	"sml_d"},
{0b11001100, 	"sml_d"},
{0b11001101, 	"sml_d"},
{0b11001110, 	"pre_d"},
{0b11001111, 	"lrg_d"},
{0b11010000, 	"udef"},
{0b11010001, 	"udef"},
{0b11010010, 	"udef"},
{0b11010011, 	"udef"},
{0b11010100, 	"udef"},
{0b11010101, 	"udef"},
{0b11010110, 	"udef"},
{0b11010111, 	"udef"},
{0b11011000, 	"udef"},
{0b11011001, 	"udef"},
{0b11011010, 	"udef"},
{0b11011011, 	"udef"},
{0b11011100, 	"udef"},
{0b11011101, 	"udef"},
{0b11011110, 	"udef"},
{0b11011111, 	"udef"},
{0b11100000, 	"lrg_l"},
{0b11100001, 	"sml_l"},
{0b11100010, 	"sml_l"},
{0b11100011, 	"sml_l"},
{0b11100100, 	"sml_l"},
{0b11100101, 	"sml_l"},
{0b11100110, 	"sml_l"},
{0b11100111, 	"sml_l"},
{0b11101000, 	"sml_l"},
{0b11101001, 	"sml_l"},
{0b11101010, 	"sml_l"},
{0b11101011, 	"sml_l"},
{0b11101100, 	"sml_l"},
{0b11101101, 	"sml_l"},
{0b11101110, 	"sml_l"},
{0b11101111, 	"sml_l"},
{0b11110000, 	"lrg_m"},
{0b11110001, 	"sml_m"},
{0b11110010, 	"sml_m"},
{0b11110011, 	"sml_m"},
{0b11110100, 	"sml_m"},
{0b11110101, 	"sml_m"},
{0b11110110, 	"sml_m"},
{0b11110111, 	"sml_m"},
{0b11111000, 	"sml_m"},
{0b11111001, 	"sml_m"},
{0b11111010, 	"sml_m"},
{0b11111011, 	"sml_m"},
{0b11111100, 	"sml_m"},
{0b11111101, 	"sml_m"},
{0b11111110, 	"sml_m"},
{0b11111111, 	"sml_m"}
};


int lzvn_decode(ifstream& is, ofstream& os) {

	string block_type;
	string out;
	char c_tmp;
	while (true) {

		for (size_t i = 0; i < 4; i++) {
			is.read(reinterpret_cast<char*> (&c_tmp), 1);
			block_type.push_back(c_tmp);
		}

		/* End of stream */
		if (block_type == "bvx$")
			break;

		/* Gestiremo solo blocchi LZVN */
		if (block_type != "bvxn")
			return EXIT_FAILURE;

		block_type.clear();

		uint32_t output_byte = 0, compressed_byte = 0;
		uint16_t match_distance = 0;
		uint8_t op_code = 0,  tmp = 0, lit = 0;

		is.read(reinterpret_cast<char*>(&output_byte), 4);
		is.read(reinterpret_cast<char*>(&compressed_byte), 4); /* include 8 byte del terminatore */
		
		while (true) {
			if (out.size() > 2267536)
				cout << out;


			uint8_t lenght = 0, match_lenght = 0;
			
			is.read(reinterpret_cast<char*>(&op_code), 1);
			compressed_byte--;

			if (table[op_code] == "sml_d") {
				lenght = (op_code & 0b11000000) >> 6;
				match_lenght = ((op_code & 0b00111000) >> 3) + 3;
				match_distance = op_code & 0b00000111;

				if ((match_distance == 0b111) || (match_distance == 0b110))
					return EXIT_FAILURE;

				is.read(reinterpret_cast<char*> (&tmp), 1);
				match_distance = (match_distance << 8) | tmp;
				compressed_byte--;
			}

			if (table[op_code] == "med_d") {

				uint16_t tmp_16 = 0;
				lenght = (op_code >> 3) & 0b11;
				match_lenght = (op_code & 0b111);

				is.read(reinterpret_cast<char*> (&tmp), 1);
				compressed_byte--;

				match_distance = (tmp >> 2) & 0b111111;
				match_lenght = ((match_lenght << 2) | (tmp & 0b11)) + 3;


				is.read(reinterpret_cast<char*> (&tmp_16), 1);
				compressed_byte--;
				
				match_distance = (tmp_16 << 6) | (match_distance);


			}

			if (table[op_code] == "lrg_d") {
				uint8_t tmp_2 = 0;
				match_distance = 0;
				lenght = (op_code >> 6) & 0b11;

				match_lenght = ((op_code >> 3) & 0b111) + 3;

				is.read(reinterpret_cast<char*> (&tmp), 1);
				compressed_byte--;

				is.read(reinterpret_cast<char*> (&tmp_2), 1);
				compressed_byte--;

				match_distance = match_distance | tmp_2;
				match_distance = (match_distance << 8) | tmp;
			}

			if (table[op_code] == "pre_d") {
				lenght = (op_code >> 6) & 0b11;
				match_lenght = ((op_code >> 3) & 0b111) + 3;
			}

			if (table[op_code] == "sml_m") {
				match_lenght = op_code & 0b00001111;
				
			}

			if (table[op_code] == "lrg_m") {
				is.read(reinterpret_cast<char*>(&match_lenght), 1);
				match_lenght += 16;
			}

			if (table[op_code] == "sml_l") {
				lenght = op_code & 0b00001111;
			}

			if (table[op_code] == "lrg_l") {
				is.read(reinterpret_cast<char*>(&lenght), 1);
				lenght += 16;
			}

			if (table[op_code] == "eos") {
				for (size_t i = 0; i < 7; i++) {
					is.read(reinterpret_cast<char*>(&tmp), 1);
					if (tmp != 0)
						return EXIT_FAILURE;
				}
				compressed_byte -= 7;
				break;
			}


			if(lenght != 0){

				for (size_t i = 0; i < lenght; i++) {
					is.read(reinterpret_cast<char*>(&lit), 1);
					//cout << lit;
					out.push_back(lit);
				}
				compressed_byte -= lenght;

				for (size_t i = 0; i < match_lenght; i++) {
					//cout << out[out.size() - match_distance];
					out.push_back(out[out.size() - match_distance]);
				}
			}
			else {
				for (size_t i = 0; i < match_lenght; i++) {
					//cout << out[out.size() - match_distance];
					out.push_back(out[out.size() - match_distance]);
				}
			}
			
		}
	}
	os.write(reinterpret_cast<const char*>(&out[0]), out.size());
	return EXIT_SUCCESS;
}



int main(int argc, char** argv) {

	string sInput(argv[1]);
	string sOutput(argv[2]);


	ifstream is(sInput, ios::binary);
	if (!is)
		return EXIT_FAILURE;


	ofstream os(sOutput, ios::binary);
	if (!os)
		return EXIT_FAILURE;


	if(lzvn_decode(is, os))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}