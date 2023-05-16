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
	int height = -1, width = -1, frame_rate, shape_factor;

	/* white space */
	is.get(tmp);

	while (is.get(tmp)) {
		
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

	mat<uint8_t> frame(height, width);
	mat<uint8_t> cb(height / 2, width / 2);
	mat<uint8_t> cr(height / 2, width/ 2);

	while (is >> frame_tag) {

		if (frame_tag != "FRAME")
			break;

		while (is.get(tmp)) {

			if (tmp == 10)
				break;

			switch (tmp) {
			break; case 'I': is >> intl;
			break; case 'X': is >> x_field;
			break; default: return false;
			}

		}

		is.read(frame.rawdata(), frame.rawsize());
		is.read(cb.rawdata(), cb.rawsize());
		is.read(cr.rawdata(), cr.rawsize());

		frames.push_back(frame);
	}
	
	return true;
}


/*int main(int args, char** argv) {

	vector<mat<uint8_t>> frames;

	y4m_extract_gray(argv[1], frames);
	return 0;
}*/