#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "mat.h"
#include "utils.h"

using namespace std;

bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames) {
	ifstream is(filename, ios::binary);
	
	if (!is)
		return false;

	/* Campi Header */
	string stream_header;
	int weight;
	int height;
	string chroma = "420jpeg";
	char interlacing = 'p';
	int framerate;
	int aspect_ratio;
	int app_dep;

	/* Memorizzo lo stream header */
	is >> stream_header;

	if (stream_header != "YUV4MPEG2")
		return false;

	string tag;
	
	while(true) {
		
		tag = is.get();
		
		if (tag == " ")
			continue;

		if (tag == "")
			break;

		/* Tag Weight */
		if (tag == "W") {
			is >> weight;
			continue;
		}
		
		/* Tag Height */
		if (tag == "H") {
			is >> height;
			continue;
		}

		/* Tag Chroma */
		if (tag == "C") {
			is >> chroma;
			if (chroma != "420jpeg")
				return false;
			continue;
		}
		
		/* Tag Interlacing */
		if (tag == "I") {
			is >> interlacing;
			continue;
		}
		
		/* Tag Frame */
		if (tag == "F") {
			is >> framerate;
			continue;
		}

		/* Tag Aspect */
		if (tag == "A") {
			is >> aspect_ratio;
			continue;
		}
		
		if (tag == "X") {
			is >> app_dep;
			continue;
		}
	}
	cout << 1;
	return true;
}

int main(int args, char** argv) {

	vector<mat<uint8_t>>frames;
	y4m_extract_gray(string(argv[1]), frames);

	return 0;
}