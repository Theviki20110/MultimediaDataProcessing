#include "base64.h"

using namespace std;

/*
	Base64	/ Ascii val
	0 - 25	/ 65 - 90	:	Lettere Maiuscole
	26 - 51 / 97 - 122	:	Lettere Minuscole
	52 - 61 / 48 - 57	:	Numeri da 0 a 9
	62 e 63 / 43 e 47	:	Simboli + e /
*/

uint8_t get_index(uint8_t value) {

	if ((value >= 65) && (value <= 90)) return value - 65;
	if ((value >= 97) && (value <= 122)) return value - 71;
	if ((value >= 48) && (value <= 57)) return value + 4 ;
	if (value == 43) return 62;
	if (value == 47) return 63;

	//Default value: forbidden character
	return '$';
}


string decode(const string& trp){

	char value = 0;
	uint8_t index;
	size_t n = 7, n_bytes = 1;
	string out;

	for (size_t i = 0; i < trp.size(); i++) {

		/* Ottengo l'indice a partire dalla codifica */
		index = get_index(trp[i]);
		if (index == '$')
			continue;
		
		/* Estraggo i 6 bit di interesse */
		for (size_t j = 6; j > 0; j--) {
			value = (value << 1) | ((index >> (j - 1)) & 1);

			/* Ho letto 8 bit */
			if (n == 0) {
				out += value;

				value = 0;
				n = 7;
				continue;
			}

			n--;
		}
	}
	return out;
}


std::string base64_decode(const std::string& input)
{
	if (input.empty())
		return string();

	string out, canonical_input;

	/* Numero di bit da leggere */
	size_t len = input.size();

	for (const auto& elem : input) {
		if (elem == '\n')
			continue;

		canonical_input += elem;
	}


	for (size_t i = 0; i <= len - 4; i += 4) {
		
		string tmp(canonical_input.begin() + i, canonical_input.begin() + i + 4);

		out += decode(tmp);
	}

	cout << out << endl;

	return out;
}