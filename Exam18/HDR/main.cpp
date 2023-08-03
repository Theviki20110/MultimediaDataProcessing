#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <math.h>

using namespace std;

struct range {
	double min_ = INFINITY;
	double max_ = 0.0;

	range(vector<vector<double>>& img_float) {
		for (const auto& elem : img_float) {
			for (const auto& pixel : elem) {
				if (pixel <= min_)
					min_ = pixel;

				if (pixel > max_)
					max_ = pixel;
			}
		}
	}

	range() = default;
};


struct hdr_header {
	string magic_; //#?RADIANCE;
	string format_; //32-bit_rle_rgbe

	hdr_header() = default;

	int build(ifstream& is) {
		
		is >> magic_;
		
		if (magic_ != "#?RADIANCE")
			return 1;

		char tmp;
		string tmp_string;

		is.get(tmp);

		while (true) {
			is.get(tmp);
			
			// Fine dell'header
			if (tmp == '\n')
				break;

			// Possiamo ignorare tutti i commenti
			if (tmp == '#') {
				while(tmp != '\n')
					is.get(tmp);
				continue; 
			}
			
			// Variable
			while (tmp != '\n') {
				tmp_string.push_back(tmp);
				is.get(tmp);
			}


			if (tmp_string.find("FORMAT") != string::npos) {
				size_t pos = tmp_string.find("=");
				string format(tmp_string.begin() + pos + 1, tmp_string.end());

				if (format != "32-bit_rle_rgbe")
					return 1;

				format_ = format;
			}

			tmp_string.clear();
		}

		return 0;
	}
};


struct hdr_content {
	uint32_t y_;
	uint32_t x_;
	vector<vector<uint8_t>> img_;

	int read_res_string(ifstream& is) {
		string tmp;
		is >> tmp;
		if (tmp == "-Y") {
			is >> y_;
		}
		else
			return 1;

		is >> tmp;

		if (tmp == "+X") {
			is >> x_;
		}
		else
			return 1;

		return 0;
	}

	size_t size() {
		return img_.size()*img_[0].size();
	}

	void resize() {
		img_.resize(y_ * x_);
	}

	int decode(ifstream& is, vector<uint8_t>& out) {

		uint16_t decoded_values = 0;
		
		uint8_t tmp = 0, copy = 0, run = 0;
	
			while(decoded_values < x_) {
				is.read(reinterpret_cast<char*>(&tmp), 1);
				if (tmp <= 127) {
					for (size_t copies = 0; copies < tmp; copies++) {
						is.read(reinterpret_cast<char*>(&copy), 1);
						out.push_back(copy);
						decoded_values++;
					}
				}
				else {
					is.read(reinterpret_cast<char*>(&run), 1);
					for (size_t rep = 0; rep < tmp - 128; rep++) {
						out.push_back(run);
						decoded_values++;
					}
				}
			}


		return 0;
	}

	int decode_img(ifstream& is) {
		vector<uint8_t> channel;
		uint8_t tmp = 0;
		uint16_t len = 0;
		is.read(reinterpret_cast<char*>(&tmp), 1);

		for (uint32_t j = 0; j < y_; j++) {

			is.read(reinterpret_cast<char*>(&tmp), 1);
			if (tmp != 2)
				return 1;

			is.read(reinterpret_cast<char*>(&tmp), 1);
			if (tmp != 2)
				return 1;

			for (size_t i = 0; i < 2; i++) {
				is.read(reinterpret_cast<char*>(&tmp), 1);
				len = (len << 8) | tmp;
			}

			if (len != x_)
				return 1;

			for (size_t i = 0; i < 4; i++) {
				decode(is, channel);
				for(uint32_t c = 0; c < x_; c++)
					img_[c + j*x_].push_back(channel[c]);
				channel.clear();
			}
		}
		return 0;
	}

	vector<vector<double>> build_float_img() {
		vector<vector<double>> out(y_ * x_);
		uint8_t exp;
		for (size_t i = 0; i < out.size(); i++) {
			exp = img_[i][3];
			for (size_t ch = 0; ch < 3; ch++)
				out[i].push_back((img_[i][ch] + 0.5) / 256.0 * pow(2, exp - 128));
		}

		return out;
	}

	vector<vector<double>> build_img(range& r, vector<vector<double>> img) {
		vector<vector<double>> out(img.size());
		double arg;
		for (size_t i = 0; i < out.size(); i++) {
			for (size_t ch = 0; ch < 3; ch++) {
				arg = (img[i][ch] - r.min_) / (r.max_ - r.min_);
				out[i].push_back(255 * pow(arg, 0.45));
			}
		}


		return out;
	}

};




int main(int argc, char** argv) {

	if (argc != 3)
		return EXIT_FAILURE;

	string sInput(argv[1]);

	ifstream is(sInput, ios::binary);
	if (!is)
		return EXIT_FAILURE;


	string sOutput(argv[2]);

	ofstream os(sOutput, ios::binary);
	if (!os)
		return EXIT_FAILURE;

	hdr_header hdr_h;
	if (hdr_h.build(is))
		return EXIT_FAILURE;

	hdr_content hdr_c;
	if (hdr_c.read_res_string(is))
		return EXIT_FAILURE;
	
	hdr_c.resize();

	if(hdr_c.decode_img(is))
		return EXIT_FAILURE;

	vector<vector<double>> img_double(hdr_c.build_float_img());
	range min_max(img_double);

	img_double = hdr_c.build_img(min_max, img_double);


	os << "P7\n" << "WIDTH " << hdr_c.x_ << "\n" << "HEIGHT " << hdr_c.y_ 
		<< "\n" << "DEPTH 3\n" << "MAXVAL 255\n" << "TUPLTYPE RGB\nENDHDR\n";

	for (uint32_t i = 0; i < img_double.size(); i++) {
		for (size_t ch = 0; ch < 3; ch++)
			os.put(img_double[i][ch]);
	}

	return EXIT_SUCCESS;
}