#include <vector>
#include <string>

using namespace std;

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
		while(v_new.size() % 3 != 0)
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


/*int main() {

	vector<uint8_t> v = { 254, 0, 254, 255,254,0,254,255,254,0,254,255,254,0,254,255,254,0,254,255,254,0,254, 255, 128, 255 };

	Base64Encode(v);

	return 0;
}*/