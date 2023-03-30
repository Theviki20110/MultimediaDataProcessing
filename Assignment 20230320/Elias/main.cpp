#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>

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
		while (n --> 0) {
		  write_bit(u >> n);
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


void elias(const char& mode, const string& is_name, const string& os_name) {
	int buf;
	uint32_t mapped_value;
	int code_lenght = 0;
	int zeros = 0;

	if (mode == 'c') {
		ifstream is(is_name);
		ofstream os(os_name, ios::binary);
		if (is.fail()) {
			cout << "Error while opening input file";
			return ;
		}


		if (os.fail()) {
			cout << "Error while opening output file";
			return ;
		}
		
		bitwriter bw(os);

		while(is >> buf){

			//Lo mappo
			if (buf < 0)
				mapped_value = 2 * abs(buf);
			else
				mapped_value = 2 * buf + 1;

			//Conto i numero di bits
			while (mapped_value >= pow(2, code_lenght))
				code_lenght++;
			
			code_lenght = (code_lenght * 2) - 1;
			
			//write bit
			bw(mapped_value, code_lenght);

			code_lenght = 0;
		}
	}
	
	else{
		ifstream is(is_name, ios::binary);
		ofstream os(os_name);

		if (is.fail()) {
			cout << "Error while opening input file";
			return ;
		}


		if (os.fail()) {
			cout << "Error while opening output file";
			return ;
		}

		bitreader br(is);
	
	}
}


int main(int args, char** argv) {

	if (args != 4) {
		cout << "Error";
		return 1;
	}
	
	elias(argv[1][0], argv[2], argv[3]);

	return 0;
}