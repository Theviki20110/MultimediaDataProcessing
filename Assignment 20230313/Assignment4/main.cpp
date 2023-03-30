#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>

template<typename T>
std::istream& raw_read(std::istream& is, const T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<const char*>(&val), size);
}


class bitreader {
	// INIT LETTURA A BIT

	uint8_t buffer_;
	int n_ = 0; // at first the buffer has 0 bits, il costruttore chiamerà prima di tutto l'inizializzazione della variabile

	/*
		Gli stream non supportano l'assegnamento perciò l'unica cosa che possiamo pensare di fare
		di passarlo per copia
	*/

	std::istream& is_;
	uint32_t read_bit() {

		if (n_ == 0) {
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		return (buffer_ >> n_) & 1;
	}
public:
	bitreader(std::istream& is) : is_(is) {}


	uint32_t read(uint8_t n) {
		uint8_t u = 0;
		// Se dovessero essere rimasti dei bit letti ma non sufficienti per raggiungere il byte scriviamo 0
		while (n -- > 0) {
			u = (u << 1) | read_bit();
		}

		return u;
	}

	uint32_t operator() (uint8_t n) {
		return read_bit();
	}

	bool fail() const {
		return is_.fail();
	}

	std::ostream& flush(uint8_t bit = 0) {
		while (n_ > 0) {
			read_bit();
		}
	}

	~bitreader() {
		flush();
	}
};

int main(int argc, char* argv[])
{
	if (argc != 3) {
		return 1;
	}

	std::ifstream is(argv[1]);
	if (!is) {
		return 1;
	}

	std::vector<int32_t> v{
		std::istream_iterator<int32_t>(is),
		std::istream_iterator<int32_t>()
	};

	std::ofstream os(argv[2], std::ios::binary);
	if (!os) {
		return 1;
	}

	bitreader br(is);

	while (1) {
		int num = br(11);
		if (br.fail()) {
			break;
		}

		if (num >= 1024) {
			num -= 2048;
		}

		v.push_back(num);
	}

	return 0;
}