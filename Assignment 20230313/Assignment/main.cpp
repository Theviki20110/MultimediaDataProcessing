#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <map>

using namespace std;

void frequencies(std::ifstream& is, std::ofstream& os) {
	char data;
	std::map<char, int> map;


	//Read element
	while (is.read(&data, sizeof(uint8_t)))
		map[data]++;

	// Write
	for (auto start = map.begin(); start != map.end(); start++)
		os << hex << setfill('0') << setw(2) << uppercase << (int)start->first << "\t" << dec << start->second << "\n";
}


int main(int argc, char** argv) {
	if (argc != 3) {
		std::cout << "Error: Not enough parameters\n" << 1;
		return 1;
	}

	std::ifstream is(argv[1], ios::binary);

	if (is.fail()) {
		std::cout << "Error: Opening input file\n" << 2;
		return 2;
	}

	std::ofstream os(argv[2], ios::binary);

	if (os.fail()) {
		std::cout << "Error: Opening output file\n" << 3;
		return 3;
	}

	frequencies(is, os);

	is.close();
	os.close();
	return 0;
}
