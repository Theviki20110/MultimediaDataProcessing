#include <fstream>
#include <iostream>
#include <string>

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

void bayer_decode(ifstream& is, string output_prefix) {

	char c;
	output_prefix.append("_gray.pgm");
	ofstream os(output_prefix, ios::binary);

	/* Leggo il magic number */
	string magic_number;

	for (size_t i = 0; i < 2; i++) {
		is >> c;
		magic_number += c;
	}

	cout << magic_number << endl;

	/* Leggo il whitespace */
	is >> c;

	/* Leggo l'alterzza */
	string height;
	for (size_t i = 0; i < 2; i++) {
		is >> c;
		height += c;
	}
	cout << height << endl;

	/* Leggo il whitespace in ASCII */
	is >> c;

	/* Leggo la lunghezza */
	string weight;
	for (size_t i = 0; i < 2; i++) {
		is >> c;
		weight += c;
	}
	cout << weight << endl;
	
	/* Leggo il whitespace */
	is >> c;

	/* Leggo il  maximum gray value (Maxval) */
	string maxval;
	for (size_t i = 0; i < 5; i++) {
		is >> c;
		maxval += c;
	}
	cout << maxval << endl;

	/* Leggo il newline */
	is >> c;

	/* Leggo il contenuto del file */
	while (is >> c) {
		os.write(to_string(c - '0' / 256).c_str(), 8);
	}
	
}


int main(int args, char** argv) {
	if (args != 3)
		return 1;

	string name(argv[1]);
	string output_prefix(argv[2]);

	// Verifico che il file passato in input abbia il formato corretto
	if (!name.find(".pgm"))
		return 1;
	
	ifstream is(argv[1]);
	
	bayer_decode(is, output_prefix);

	return 0;
}