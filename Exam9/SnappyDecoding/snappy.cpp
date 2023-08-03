#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
istream& raw_read(istream& is, T& buffer, size_t size=sizeof(T)) {
	return is.read(reinterpret_cast<char*> (&buffer), size);
}


struct bitreader {
	uint8_t n_ = 0;
	uint8_t buffer_;
	istream& is_;

	bitreader(istream& is) : is_(is) {};

	uint8_t read_bit() {
		if (n_ == 0) {
			n_ = 8;
			raw_read(is_, buffer_);
		}
		--n_;
		return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint32_t n) {
		uint32_t u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return u;
	}

	uint32_t operator()(uint32_t n) {
		return read(n);
	}

	bool operator!() {
		return is_.fail();
	}


};


uint32_t preamble(bitreader& br) {
	uint8_t tmp = br(1), n_shift = 1;
	uint32_t out = br(7);

	while (tmp) {
		tmp = br(1);
		out = (br(7) << n_shift*7) | out;
		n_shift++;
	}

	return out;
}


int main(int argc, char** argv) {
	
	if (argc != 3)
		return 1;

	ifstream is(argv[1], ios::binary);
	if (is.fail())
		return 1;

	ofstream os(argv[2], ios::binary);
	if (os.fail())
		return 1;

	bitreader br(is);
	uint8_t tag, is_copy, tmp;
	int64_t offset;
	string out;
	uint32_t original_size = preamble(br);
	uint64_t lenght = 0;
	int residual = original_size;

	while (residual) {

		tag = br(6);
		is_copy = br(2);


		/* Literal */
		if (!is_copy) {
			if (tag < 60) { lenght = tag; }
			if (tag == 60) { lenght = br(8); }
			if (tag == 61) { is.read(reinterpret_cast<char*>(&lenght), 2 * sizeof(uint8_t)); }
			if (tag == 62) { is.read(reinterpret_cast<char*>(&lenght), 3 * sizeof(uint8_t)); }
			if (tag == 63) { is.read(reinterpret_cast<char*>(&lenght), 4 * sizeof(uint8_t)); }

			for (uint64_t i = 0; i <= lenght; i++) {
				tmp = br(8);
				out.push_back(tmp);
				//cout << tmp;
				residual--;
			}
		}
		else {

			/* Copy with 1-byte offset */
			if (is_copy == 1) {
				offset = 0;
				offset = (((tag >> 3) & 0b00000111) << 8) | br(8);

				lenght = tag & 0b00000111;
				lenght += 4;

			}

			/* Copy with 2-byte offset */
			if (is_copy == 2) {
				offset = 0;
				raw_read(is, offset, 2);
				lenght = tag + 1;
			}

			/* Copy with 4-byte offset */
			if (is_copy == 3) {
				offset = 0;
				lenght = tag + 1;
				raw_read(is, offset, 4);

			}

			for (int i = 0; i < lenght; i++) {
				char substring = out[out.size() - size_t(offset)];
				out.push_back(substring);
				//cout << substring;
				residual--;
			}
		}

	}
	os.write(reinterpret_cast<const char*> (&out[0]), out.size());
	
	return 0;
}