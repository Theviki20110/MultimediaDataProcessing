#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

/*

	write -> for(int i = n - 1; i > 0; i++) (number >> i) estraiamo il bit più significativo
	write_bit -> estraggo il bit più significativo, lo inserisco nel buffer ed aspetto di essere arrivato ad 8 bit
					(buffer << 1) | (bit & 1)
*/

template<typename T>
ostream& raw_write(ofstream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*> (& val), size);
}

class bitwriter {
	ofstream& os_;
	uint8_t buffer_ = 0;
	int n_ = 0;

	ofstream& write_bit(uint32_t bit) {
		
		buffer_ = (buffer_ << 1) | (bit & 1);
		n_++;

		if (n_ == 8) {
			n_ = 0;
			raw_write(os_, buffer_);
		}
		
		return os_;
	}
public:
	/* Costruttore */
	bitwriter(ofstream& os) : os_(os) {};


	ofstream& write(uint32_t u, uint8_t n) {

		while (n-- > 0) {
			write_bit(u >> n);
		}

		return os_;
	}


	ofstream& operator ()(uint32_t u, uint8_t n) {
		return write(u, n);
	}

	ofstream& flush() {
		while (n_ > 0)
			write_bit(0);
		
		return os_;
	}

};

bool line_search(vector<string>& dict, string& elem) {
	for (const auto& dict_elem : dict) {
		if (dict_elem == elem)
			return true;
	}
	return false;
}

uint32_t find_index(vector<string>& dict, string& elem) {
	uint32_t index = 0;

	for (; index < dict.size(); index++) {
		if (dict[index] == elem)
			break;
	}
	index++;
	return index;
}

uint8_t count_bits(size_t dict_size, uint8_t n_bits) {
	
	if (dict_size > pow(2, n_bits)) {
		n_bits++;
		return n_bits;
	}

	return n_bits;
}

struct encoder {

	vector<string> dict_;
	uint32_t index_;
	int maxbits_;
	bitwriter& bw_;
	/*
		Nel codice non mi preoccuperò di resettare n_bits in quanto sarà ricalcolato
		ad ogni nuova sequenza individuata e lo utilizzo per resettare il dizionario
	*/

	uint8_t n_bits_ = 0;
	string seq_;

	encoder(int maxbits, bitwriter& bw) : bw_(bw) {
		maxbits_ = maxbits;
	};

	bool encode(char tmp, bool eof) {

		if (tmp != 0)
			seq_.push_back(tmp);


		/* Nel caso in cui dovessimo utilizzare più bit di quelli previsti devo pulire il dizionario */
		if (dict_.size() == pow(2, maxbits_)) {
			n_bits_ = 0;
			dict_.clear();
		}

		/* Se non trovo corrispondenza nel dict allora posso aggiungere la nuova sequenza*/

		if (!line_search(dict_, seq_) || (eof)) {

			/* Caso: ho trovato un simbolo non presente nel dict (0, seq_) */
			if (seq_.size() == 1) {

				dict_.push_back(seq_);

				if (!dict_.empty()) {
					n_bits_ = count_bits(dict_.size(), n_bits_);
					bw_(0, n_bits_);
				}

				bw_(tmp, 8);

				seq_.clear();

				//cout << 0 << ", " << tmp << " N-bits: " << int(n_bits_) << endl;

				return true;
			}

			/*

				Rimuovo l'ultimo simbolo dalla sequenza e lo conservo.
				Faccio questo perchè voglio ottenere la posizione della
				sequenza già presente nel dizionario

			*/

			tmp = seq_.back();
			seq_.pop_back();

			/* Trovo l'indice della sequenza memorizzata */
			index_ = find_index(dict_, seq_);

			dict_.push_back(seq_ + tmp);
			
			/* Calcolo quanti bit utilizzare per scrivere l'indice nel file */
			n_bits_ = count_bits(dict_.size(), n_bits_);


			bw_(index_, n_bits_);
			bw_(tmp, 8);

			seq_.clear();

			/* Debug print */
			//cout << index_ << ", " << tmp << " N-bits: " << int(n_bits_) << endl;
		}
		return true;
	}

};

bool lz78encode(const std::string& input_filename, const std::string& output_filename, int maxbits) {
	
	ifstream is(input_filename, ios::binary);
	if (is.fail())
		return false;

	ofstream os(output_filename, ios::binary);
	if (os.fail())
		return false;

	if ((maxbits > 30) || (maxbits < 1))
		return false;
	
	os << "LZ78";
	
	bitwriter bw(os);
	encoder lz78(maxbits, bw);
	char tmp;

	bw(maxbits, 5);
	
	/* Leggo da file */
	while (true) {

		is.get(tmp);

		if (is.peek() == EOF) {
			lz78.encode(tmp, true);
			break;
		}

		lz78.encode(tmp, false);
	}

	/* Padding */
	bw.flush();
	
	return true;
}



int main(int args, char** argv) {

	lz78encode(argv[1], argv[2], 20);

	return 0;

}