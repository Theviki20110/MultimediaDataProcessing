#include "lzs.h"

template<typename T>
istream& raw_read(istream& is, T& buffer, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*> (&buffer), size);
}

struct bitreader {
	istream& is_;
	uint8_t n_ = 0;
	uint8_t buffer_;


	bitreader(istream& is) : is_(is) {}

	uint32_t read_bit() {

		if (n_ == 0) {
			n_ = 8;
			raw_read(is_, buffer_);
		}


		--n_;
		return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint32_t n) {
		uint32_t out = 0;
		while (n-- > 0) {
			out = (out << 1) | read_bit();
		}

		return out;
	}


	uint32_t operator ()(uint8_t n) {
		return read(n);
	}

};

uint32_t get_lenght(bitreader& br) {
	
	uint8_t lenght = br(2), tmp;
	uint32_t flen, n = 1;
	if (lenght < 3)
		return lenght + 2;
	
	lenght = lenght << 2 | br(2);
	
	if (lenght < 15)
		return lenght - 7;

	while ((tmp = br(4)) == 15)
		n++;

	flen = tmp + ((n * 15) - 7);

	return flen;
}


void add(vector<uint8_t>& dict, uint8_t elem) {
	if (dict.size() > 2047)
		dict.erase(dict.begin());
	dict.push_back(elem);
}


void lzs_decompress(std::istream& is, std::ostream& os) {
	
	bitreader br(is);
	uint8_t is_offset = 0;
	uint8_t literal, tmp;
	uint32_t lenght, offset;
	vector<uint8_t> dict;

	while (1) {

		is_offset = br(1);

		if (!is_offset) {
			literal = br(8);
			add(dict, literal);
			cout << literal;
		}
		else {
			if (br(1)) {
				
				tmp = br(7);
				
				/* End marker */
				if (tmp == 0)
					break;

				/* Caso di offset < 128 */
				offset = tmp;

			}
			else {
				/* Offset > 128 */
				offset = br(11);
			}

			/* Lenght */
			lenght = get_lenght(br);

			if (offset == 1) {
				tmp = dict.back();
				for (size_t i = 0; i < lenght; i++) {
					cout << tmp;
					add(dict, tmp);
				}
			}
			else {

				while (offset > dict.size())
					offset -= dict.size();

				if (lenght > offset)
					lenght -= offset;

				for (size_t i = 0; i < lenght; i++) {
					tmp = dict[dict.size() - offset];
					cout << tmp;
					add(dict, tmp);
				}
			}
		}
		is_offset = 0;
	}
}




int main(int args, char** argv) {

	ifstream is(argv[1]);
	ofstream os(argv[2]);

	lzs_decompress(is, os);

	return 0;
}