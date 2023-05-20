#include <vector>
#include <string>
#include <fstream>
#include "mat.h"
#include "types.h"

using namespace std;


int conv_ycbcr(uint8_t elem, bool is_c) {
	if (elem < 16)
		return 16;

	if ((elem > 235) && (!is_c))
		return 235;


	if ((elem > 240) && (is_c))
		return 240;
	
	return 0;
}


bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames) {

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

	mat<vec3b> frame(height, width);
	mat<uint8_t> y(height, width);
	mat<uint8_t> cb(height / 2, width / 2);
	mat<uint8_t> cr(height / 2, width / 2);

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
		
		is.read(y.rawdata(), y.rawsize());
		is.read(cb.rawdata(), cb.rawsize());
		is.read(cr.rawdata(), cr.rawsize());
	
		int R, G, B;

		for (int r = 0; r < y.rows(); r++) {
			for (int c = 0; c < y.cols(); c++) {

				/* Conversioni */
				y(r, c) = conv_ycbcr(y(r, c), false);
				cb(r / 2, c / 2) = conv_ycbcr(cb(r / 2, c / 2), true);
				cr(r / 2, c / 2) = conv_ycbcr(cr(r / 2, c / 2), true);

				R = (1164 * (y(r, c) - 16) + 0 * (cb(r / 2, c / 2) - 128) + 1596 * (cr(r / 2, c / 2) - 128)) / 1000;
				G = (1164 * (y(r / 2, c / 2) - 16) - 392 * (cb(r / 2, c / 2) - 128) - 813 * (cr(r / 2, c / 2) - 128)) / 1000;
				B = (1164 * (y(r / 2, c / 2) - 16) + 2017 * (cb(r / 2, c / 2) - 128) + 0 * (cr(r / 2, c / 2) - 128)) / 1000;

				R = R < 0 ? 0 : R>255 ? 255 : R;
				G = G < 0 ? 0 : G>255 ? 255 : G;
				B = B < 0 ? 0 : B>255 ? 255 : B;

				frame(r, c) = { uint8_t(R), uint8_t(G), uint8_t(B) };
			}
		}




		frames.push_back(frame);
	}

	return true;
}
/*
int main(int args, char** argv) {

	vector<mat<vec3b>> frames;

	y4m_extract_color(argv[1], frames);
	return 0;

}*/