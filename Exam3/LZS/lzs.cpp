#include "lzs.h"

template<typename T>
istream& raw_read(istream& is, T& buffer, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*> (&buffer), size);
}


class bitreader {
	uint8_t buffer_;
	int n_;
	std::istream& is_;

	uint8_t read_bit() {
		if (n_ == 0) {
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		return (buffer_ >> n_) & 1;
	}

public:
	bitreader(std::istream& is) : is_(is), n_(0) {}

	uint32_t read(uint8_t n) {
		uint32_t val = 0;
		while (n-- > 0) {
			val = (val << 1) | read_bit();
		}
		return val;
	}

	uint32_t operator() (uint8_t n) {
		return read(n);
	}

	bool fail() const {
		return is_.fail();
	}

	explicit operator bool() const {
		return !fail();
	}
};

uint32_t get_lenght(bitreader& br) {
	
	uint8_t lenght = br(2), tmp = 15;
	uint32_t flen, n = 0;
	if (lenght < 3)
		return lenght + 2;
	
	lenght = lenght << 2 | br(2);
	
	if (lenght < 15)
		return lenght - 7;

	while (tmp == 15) {
		tmp = br(4);
		n++;
	}

	flen = tmp +  15*n - 7;
	return flen;
}

void lzs_decompress(std::istream& is, std::ostream& os) {
	
	bitreader br(is);
	uint8_t is_offset = 0;
	uint8_t literal, tmp;
	uint32_t lenght, offset;
	string dict;

	while (1) {

		is_offset = br(1);
		if (!is_offset) {
			literal = br(8);
			dict.push_back(literal);
			//cout << literal;
		}
		else {
			tmp = br(1);
			if (tmp) {
				
				/* Caso di offset < 128 */
				offset = br(7);
				tmp = offset;
				/* End marker */
				if (tmp == 0) {
					os.write(reinterpret_cast<const char*>(&dict[0]), dict.size());
					return;
				}
			}

			else {
				/* Offset > 128 */
				offset = br(11);
			}

			/* Lenght */
			lenght = get_lenght(br);

			for (size_t i = 0; i < lenght; i++) {
				//cout << dict[dict.size() - offset];
				dict.push_back(dict[dict.size() - offset]);
			}
			
		}
		is_offset = 0;
	}
}




/*int main(int args, char** argv) {

	ifstream is(argv[1], ios::binary);
	ofstream os(argv[2], ios::binary);

	lzs_decompress(is, os);

	return 0;
}*/