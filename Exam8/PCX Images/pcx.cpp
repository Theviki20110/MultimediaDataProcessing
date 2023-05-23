#include "pcx.h"
#include "pgm.h"
#include <iostream>
/* Si noti che: 1 byte di PCX == 8 pixel img */

struct header {
	uint8_t manufacturer_;
	uint8_t version_;
	uint8_t encoding_;
	uint8_t bits_per_plane_;
	uint16_t windowXmin_;
	uint16_t windowYmin_;
	uint16_t windowXmax_;
	uint16_t windowYmax_;
	uint8_t colorplanes_;
	uint16_t bytes_per_planeline_;

	header(ifstream& is) {
		manufacturer_ = is.get();
		
		is >> version_;
		is >> encoding_;
		is >> bits_per_plane_;

		is.read(reinterpret_cast<char*>(&windowXmin_), sizeof(windowXmin_));
		is.read(reinterpret_cast<char*>(&windowYmin_), sizeof(windowYmin_));
		is.read(reinterpret_cast<char*>(&windowXmax_), sizeof(windowXmax_));
		is.read(reinterpret_cast<char*>(&windowYmax_), sizeof(windowYmax_));

		/* Ignore 53 bytes */
		for (size_t i = 0; i < 53; i++)
			is.get();

		is >> colorplanes_;
		is.read(reinterpret_cast<char*>(&bytes_per_planeline_), sizeof(bytes_per_planeline_));

		/* Ignore 60 bytes */
		for (size_t i = 0; i < 60; i++)
			is.get();
	};
};

template<typename T>
istream& raw_read(T& buffer, istream& is, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&buffer), size);
}


struct bitreader {

	ifstream& is_;
	uint8_t buffer_;
	int n_ = 0;


	bitreader(ifstream& is) : is_(is) {};

	uint8_t read_bit() {

		if (n_ == 0) {

			n_ = 8;
			raw_read(buffer_, is_);
		}
		
		n_--;

		return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint8_t n) {
		/* Creiamo la variabile che andremo a riempire con n bit da leggere*/
		
		uint32_t u = 0;
		
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		
		return u;
	}


	uint32_t operator ()(uint8_t n) {
		return read(n);
	}

};

void increase(uint16_t& r, uint16_t& c, mat<uint8_t>& img) {
	if ((c < img.cols() - 1) && (r < img.rows()))
		c++;
	else {
		if (r < img.rows()) {
			r++;
			c = 0;
		}
		else {
			r = img.rows();
			c = img.cols();
		}

	}
}

bool load_pcx(const std::string& filename, mat<uint8_t>& img)
{
	ifstream is(filename, ios::binary);
	if (is.fail())
		return false;

	
	header h(is);
	
	uint16_t xsize = h.windowXmax_ - h.windowXmin_ + 1;
	uint16_t ysize = h.windowYmax_ - h.windowYmin_ + 1;
	uint32_t totalbytes = h.bits_per_plane_ * h.bytes_per_planeline_;

	img.resize(ysize, xsize);
	bitreader br(is);

	uint8_t is_duplicate = 0, data, bit = 0;
	uint32_t read_bytes = 0;

	for(uint16_t r = 0; r < img.rows();) {
		for (uint16_t c = 0; c < img.cols();) {

			is_duplicate = br(2);

			if (is_duplicate == 3) {

				uint8_t reps = br(6);
				
				data = br(8);

				for (uint8_t i = 0; i < reps; i++) {

					for (size_t j = 8; j > 0; j--) {
						bit = (bit << 1) | (data >> (j - 1) & 1);
						img(r, c) = (bit) ? 255 : 0;
						increase(r, c, img);
					}
				}
			}
			else {
				br(6);
				
				data = br.buffer_;

				for (size_t j = 8; j > 0; j--) {
					bit = (bit << 1) | (data >> (j - 1) & 1);
					img(r, c) = (bit) ? 255 : 0;
					increase(r, c, img);
				}
			}
			increase(r, c, img);
		}
		
	}

	/* Da commentare */
	save_pgm("output.pgm", img);

	return true;
}


int main(int args, char** argv) {
	mat<uint8_t> img;
	load_pcx(argv[1], img);
	return 0;
}