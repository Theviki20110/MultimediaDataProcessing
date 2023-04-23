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

	for (const auto& elem : sym) {
		if (elem == '\n') {
			n_elems += 2;
			continue;
		}

		n_elems++;
	}

	if (n_elems > 127)
		return 127;

	return n_elems - 1;
}


void encode(ifstream& is, ofstream& os) {
	char curr_char;
	char last_char = NULL;
	uint8_t rep_sym = 0;
	vector<char> sym;
	bitwriter bw(os);
	bitreader br(is);

	while ((curr_char = br(8))) {
		 
		/* CHECK EOF */
		if (br.fail())
			break;

		/* Durante la prima lettura nessun simbolo è stato letto prima */
		if (last_char == NULL) {
			last_char = curr_char;
			continue;
		}

		/* CASO: simbolo ripetuto */
		if (last_char == curr_char) {

			/* CASO: se dovessi trovare una newline lo inserisco direttamente e non lo conto come ripetizione */
			if (curr_char == '\n') {
				sym.push_back(last_char);
				last_char = curr_char;
				continue;
			}
			
			rep_sym++;

			/* CASO: se dovessi superare la soglia limite fermo il conteggio della ripetizione */
			if (rep_sym == 128) {
				bw(129, 8);
				bw(last_char, 8);
				rep_sym = 0;
				continue;
			}


			/* CASO: fine di una serie di simboli differenti */
			if (sym.size() != 0) {
				
				uint32_t n_elem = count_elem(sym);

				bw(n_elem, 8);
				
				for (size_t i = 0; i < sym.size(); i++) {
					if (i == 128)
						bw(0, 8);

					bw(sym[i], 8);
				}
					
				
				sym.clear();
			}
		}

		/* CASO: simbolo differente */
		else {
		
			/* CASO: fine di una ripetizione di simboli */
			if (rep_sym != 0) {
				uint8_t val = 257 - rep_sym - 1;
				bw(val, 8);
				bw(last_char, 8);
				rep_sym = 0;
			}

			else {
				sym.push_back(last_char);
			}
		}

		last_char = curr_char;
	}

	/* CASO caratteri residui alla fine della lettura */
	if (rep_sym == 0)
		sym.push_back(last_char);

	/* CASO simboli non scritti alla fine della lettura */
	if (sym.size() != 0) {
		uint32_t n_elem = count_elem(sym);
		bw(n_elem, 8);
		for (size_t i = 0; i < sym.size(); i++) {
			if (i == 128)
				bw(0, 8);

			bw(sym[i], 8);
		}
	}

	/* CASO ultimi due simboli non scritti alla fine della lettura */
	if (last_char == curr_char) {
		bw(257 - rep_sym - 1, 8);
		bw(last_char, 8);
	}

	/* Scrivo EOB alla fine della lettura del file */
	bw(128, 8);
}

void decode(ifstream& is, ofstream& os) {

}

int main(int argc, char** argv) {
	
	if (argc != 4) {
		cout << "Error parameters" << endl;
		return 1;
	}
	
	string mode(argv[1]);

	ifstream is(argv[2]);
	
	ofstream os(argv[3]);

	if ((is.fail()) || (os.fail())) {
		cout << "Error opening files" << endl;
		return 2;
	}

	if (mode == "d")
		decode(is, os);

	if (mode == "c")
		encode(is, os);

	return 0;
}