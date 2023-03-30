#include <fstream>
#include <iostream>
#include <vector>
using namespace std;


void write_int11(std::ifstream& is, std::ofstream& os) {

	uint8_t buffer;
	
	vector<uint8_t> v{
		istream_iterator<uint8_t>(is),
		istream_iterator<uint8_t>()
	};

	for (const auto& x : v) {
		if ((x < -1000) || (x > 1000))
			continue;


	
	
	
	}


}


int main(int argc, char** argv) {

	if (argc != 3) {
		cout << "Error: Wrong parameters number";
		return 1;
	}


	ifstream is(argv[1]);
	
	if (is.fail()) {
		cout << "Error while opening input file";
		return 2;
	}
	
	ofstream os(argv[2], ios::binary);
	
	if (os.fail()) {
		cout << "Error while opening output file";
		return 3;
	}

	write_int11(is, os);

	return 0;
}