#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}

template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}

class bitwriter {
	uint8_t buffer_;
	int n_ = 0;
	std::ostream& os_;

	std::ostream& write_bit(uint32_t bit) {
		buffer_ = (buffer_ << 1) | (bit & 1);
		++n_;
		if (n_ == 8) {
			raw_write(os_, buffer_);
			n_ = 0;
		}
		return os_;
	}

public:
	bitwriter(std::ostream& os) : os_(os) {}

	std::ostream& write(uint32_t u, uint8_t n) {
		for (int i = n - 1; i >= 0; --i) {
			write_bit(u >> i);
		}

		return os_;
	}

	std::ostream& operator()(uint32_t u, uint8_t n) {
		return write(u, n);
	}

	std::ostream& flush(uint32_t bit = 0) {
		while (n_ > 0) {
			write_bit(bit);
		}
		return os_;
	}

	~bitwriter() {
		flush();
	}
};

class bitreader {
	uint8_t buffer_;
	uint8_t n_ = 0;
	std::istream& is_;

public:
	bitreader(std::istream& is) : is_(is) {}

	uint32_t read_bit() {
		if (n_ == 0) {
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint8_t n) {
		uint32_t u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return u;
	}

	uint32_t operator()(uint8_t n) {
		return read(n);
	}

	bool fail() const {
		return is_.fail();
	}

	explicit operator bool() const {
		return !fail();
	}
};

uint32_t count_elem(vector <char> sym) {
	uint32_t n_elems = 0;

	for (const auto& elem : sym)
		n_elems++;


	if (n_elems > 127)
		return 127;

	return n_elems - 1;
}

void encode(ifstream& is, ofstream& os) {
	char curr_elem, last_elem = NULL;
	int rep = 0;
	vector<char> elems;
	bitwriter bw(os);

	while (is.get(curr_elem)) {

		/* Inizio lettura */
		if (last_elem == NULL) {
			last_elem = curr_elem;
			continue;
		}

		/* Trovo una ripetizione */
		if (last_elem == curr_elem) {

			rep++;

			/* Caso in cui siano 128 ripetizioni*/
			if (rep == 128) {
				
				bw(257 - rep, 8);
				bw(last_elem, 8);

				rep = 0;
				
				last_elem = curr_elem;
				continue;
			}

			/* Esco da caratteri differenti */
			if ((elems.size() != 0) && (rep > 1)) {
				if(elems.size() > 127)
					bw(127, 8);
				else
					bw(elems.size() - 1, 8);

				for (size_t i = 0; i < elems.size(); i++) {
					if ((i % 128 == 0) && (i != 0)) {
						if (elems.size() - i > 127)
							bw(127, 8);
						else
							bw(elems.size() - i - 1, 8);
					}

					bw(elems[i], 8);
				}
				elems.clear();
			}

			last_elem = curr_elem;
			continue;
		}

		/* Caratteri differenti */
		else {

			/* Ho trovato solo 2 caratteri uguali quindi la run continua*/
			if (rep == 1) {

				elems.push_back(last_elem);
				rep = 0;

			}

			/* Esco da una ripetizione */
			if (rep != 0) {
				bw(257 - 1 - rep, 8);
				bw(last_elem, 8);
				rep = 0;
				last_elem = curr_elem;
				continue;
			}

			elems.push_back(last_elem);
		}

		last_elem = curr_elem;

	}

	if (elems.size() != 0) {

		elems.push_back(curr_elem);

		if (rep == 1) 
			elems.push_back(curr_elem);

			bw(elems.size() - 1, 8);

			for (const auto& c : elems)
				bw(c, 8);
		}

		/* Controllo sull'ultimo carattere */
		if ((curr_elem == last_elem) && (rep > 1)) {

			/* Fine di una ripetizione */
			bw(257 - 1 - rep, 8);
			bw(last_elem - 1, 8);

		}

		/* EOF: fine della codifica */
		bw.write(128, 8);
	}


void decode(ifstream& is, ofstream& os) {
	
	char elem;
	unsigned char rep;
	vector<char> elems;

	while ((is.get(elem)) || (elem == '0')) {
		
		if (is.fail())
			break;

		rep = elem;

		/* Caso ripetizione */
		if ((rep > 128) && (rep < 256)) {
			is >> elem;
			rep = 257 - rep;
			for (int i = 0; i < rep; i++)
				os << elem;
			continue;
		}

		/* Caso simboli differenti */
		if ((rep >= 0) && (rep < 128)) {
			for (int i = 0; i < rep + 1; i++) {
				is.get(elem);
				if (is.fail())
					break;
				os << elem;
			}
			continue;
		}
	}

}

int main(int argc, char** argv) {

	if (argc != 4) {
		cout << "Error parameters" << endl;
		return 1;
	}

	string mode(argv[1]);

	ifstream is(argv[2], ios::binary);

	ofstream os(argv[3], ios::binary);

	if ((is.fail()) || (os.fail())) {
		cout << "Error opening files" << endl;
		return 1;
	}

	if (mode == "d")
		decode(is, os);

	if (mode == "c")
		encode(is, os);

	return 0;
}