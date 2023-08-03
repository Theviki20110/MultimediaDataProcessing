#include <fstream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
istream& raw_read(istream& is, T& buffer, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&buffer), size);
}

template <typename T>
ostream& raw_write(ostream& os, T& buffer, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&buffer), size);
}

struct bitreader {

	uint8_t n_ = 8;
	uint8_t buffer_ = 0;
	ifstream& is_;

	bitreader(ifstream& is) :  is_(is) {}

	uint8_t read_bit() {
		if (n_ == 0) {
			n_ = 8;
			raw_read(is_, buffer_);
		}

		n_--;
		return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint32_t n) {
		uint32_t u = 0;

		while (n-- > 0) {
			u = (u << n) | read_bit();
		}

		return u;
	}

	uint32_t operator()(uint32_t n) {
		return read(n);
	}
};

struct bitwriter {

	uint8_t n_ = 0;
	uint8_t buffer_ = 0;
	ofstream& os_;

	bitwriter(ofstream& os) : os_(os) {}

	void write_bit(uint8_t bit) {
		n_++;
		buffer_ = (buffer_ << 1) | bit;

		if (n_ == 8) {
			n_ = 0;
			raw_write(os_, buffer_);
			buffer_ = 0;
		}
	}

	void write(uint32_t n, uint32_t u) {
		uint8_t bit = 0;
		while (u-- > 0) {
			write_bit((n >> u) & 1);
		}

	}

	void operator()(uint32_t n, uint32_t u) {
		write(n, u);
	}

};


struct header {
	string magic_number_ = "HUFFDIFF";
	uint32_t width_;
	uint32_t height_;

	header(uint32_t width, uint32_t height): width_(width), height_(height) {}
};


struct img {
	uint32_t width;
	uint32_t height;
	vector<uint8_t> data;

	void resize(uint32_t h, uint32_t w) {
		data.resize(h * w);
	}

	void read_pam(ifstream& is) {
		
	}

	void save_pam(ofstream& is) {

	}
};


int main(int argc, char** argv) {
	
	if (argc != 4)
		return EXIT_FAILURE;

	
	string mode(argv[1]);
	string sInput(argv[2]);
	string sOutput(argv[3]);
	
	ifstream is(sInput, ios::binary);
	if (!is)
		return EXIT_FAILURE;

	ofstream os(sOutput, ios::binary);
	if (!os)
		return EXIT_FAILURE;

	bitreader br(is);
	bitwriter bw(os);




	
	return EXIT_SUCCESS;
}