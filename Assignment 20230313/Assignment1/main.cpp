#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

using namespace std;


void write_int32(ifstream& is, ofstream& os) {

	istream_iterator<int32_t> is_start(is);
	istream_iterator<int32_t> is_stop;

	vector<int32_t> v(is_start, is_stop);


	//SOSTITUITO DALLE 3 RIGHE SOPRA
	/*while (is >> val) {

		if ((val < -1000) || (val > 1000))
			continue;

		v.push_back(val);
	}*/

	for (auto& it : v)
		os.write((const char*)&it, sizeof it);

}


int main(int args, char** argv) {

	if (args != 3) {
		cout << "Error: wrong number of parameters\n" << 1;
		return 1;
	}


	ifstream is(argv[1]);
	
	if (is.fail()) {
		cout << "Error: Opening input file\n" << 2;
		return 2;
	}

	ofstream os(argv[2], ios::binary);

	if (os.fail()) {
		cout << "Error: Opening output file\n" << 3;
		return 3;
	}


	write_int32(is, os);

	return 0;
}