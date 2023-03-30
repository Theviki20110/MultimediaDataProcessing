#include <fstream>
#include <iostream>
#include <vector>
#include <stdint.h>

using namespace std;

void read_int32(ifstream& is, ofstream& os) {

	vector<int32_t> v;
	int32_t val;

	while (is.read((char*) &val, sizeof(int32_t)))
		v.push_back(val);

	for (auto& it : v)
		os << it << '\n';
}

int main(int args, char** argv) {
	
	if (args != 3) {
		cout << "Not enough parameters";
		return 1;
	}

	ifstream is(argv[1], ios::binary);

	if (is.fail()) {
		cout << "Error while opening input file\n" << 2;
		return 2;
	}

	ofstream os(argv[2]);

	if (os.fail()) {
		cout << "Error while opening output file\n" << 3;
		return 3;
	}

	read_int32(is, os);

	return 0;
}