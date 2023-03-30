#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}


class bitwriter {
	// INIT SCRITTURA A BIT

	uint8_t buffer_;
	int n_ = 0; // at first the buffer has 0 bits, il costruttore chiamerà prima di tutto l'inizializzazione della variabile

	/*
		Gli stream non supportano l'assegnamento perciò l'unica cosa che possiamo pensare di fare
		di passarlo per copia
	*/

	std::ostream& os_;

	std::ostream& write_bit(uint8_t bit) {


		/* Scrivo nel buffer il risultato */
		buffer_ = (buffer_ << 1) | bit;

		++n_;

		/* Se abbiamo scritto 8 bit */
		if (n_ == 8) {
			raw_write(os_, buffer_);
			n_ = 0;
		}

		return os_;
	}
public:
	bitwriter(std::ostream& os) : os_(os) {}


	std::ostream& write(uint8_t u, uint8_t n) {
		// Se dovessero essere rimasti dei bit letti ma non sufficienti per raggiungere il byte scriviamo 0
		while (n --> 0) { 
			write_bit(u >> n);
		}
	}

	std::ostream& flush(uint8_t bit = 0) {
		while (n_ > 0) {
			write_bit(bit);
		}
	}

	~bitwriter() {
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

	bitwriter bw(os);
	for (const auto& x : v) { // dobbiamo sostituirlo con qualcosa che scriva 11 bit alla volta
		// scriviamo un bit alla volta
		/*
		uint8_t cur_bit;	// deve essere uno degli 11 bit di x
		for (int i = 10; i >= 0; --i) { // Per 11 volte scriviamo un bit nel buffer
			Prendo il bit meno significativo
			cur_bit = (x >> i) & 1;
		}
		*/
		bw.write(x, 11);
	}
	return 0;
}