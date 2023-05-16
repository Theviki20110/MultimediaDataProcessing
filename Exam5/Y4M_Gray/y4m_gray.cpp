#include <vector>
#include <string>
#include <fstream>
#include "mat.h"

using namespace std;

bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames) {
	
	ifstream is(filename);
	if (is.fail())
		return false;

	string stream_header;
	is >> stream_header;
	if (stream_header != "YUV4MPEG2")
		return false;

	/* read tagged-field */
	char tmp, intl;
	string chroma("420jpeg"), x_field, frame_tag;
	uint32_t height, width, frame_rate, shape_factor;

	/* white space */
	is.get(tmp);

	while (true) {

		is.get(tmp);
		
		if (tmp == 10)
			break;
		
		switch (tmp) {
		break; case 'H': is >> height;
		break; case 'W': is >> width;
		break; case 'C': is >> chroma;
		break; case 'I': is >> intl;
		break; case 'X': is >> x_field;
		break; case 'F': is >> frame_rate;
		break; case 'A': is >> shape_factor;
		}
	}

	if ((height < 0) || (width < 0) || (chroma != "420jpeg"))
		return false;

	mat<uint8_t> frame(width, height);
	
	while (true) {

		is >> frame_tag;
		if (frame_tag != "FRAME")
			break;
		while (is.get(tmp)) {
			for (size_t r = 0; r < width; r++) {
				for (size_t c = 0; c < height; c++) {

				}
			}
		}
	}
	
	return true;
}


int main(int args, char** argv) {

	vector<mat<uint8_t>> frames;

	y4m_extract_gray(argv[1], frames);
	return 0;
}