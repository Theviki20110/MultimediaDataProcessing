#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <functional>
#include <exception>

using namespace std;

#include "ppm.h"

struct canvas{
	unsigned w_ = 0;
	unsigned h_ = 0;
	vector<unsigned int> background_;


	canvas() = default;
	int build(ifstream& is) {
		char tmp = 0;
		string key;
		uint8_t len, val;
		uint8_t llen = 0;
		while (true) {

			is.get(tmp);
			if (tmp == '}')
				return 0;

			if (tmp == '{')
				continue;

			if (tmp != 'i')
				return 1;

			//key
			is.get(tmp);
			len = tmp;

			for (size_t i = 0; i < len; i++) {
				is.get(tmp);
				key.push_back(tmp);
			}

			//values
			if (key == "width") {
				is.get(tmp);
				if (tmp == 'i') {
					is.read(reinterpret_cast<char*>(&w_), 1);
				}
				if (tmp == 'I') {
					is.read(reinterpret_cast<char*>(&llen), 1);
					w_ = llen;
					llen = 0;
					is.read(reinterpret_cast<char*>(&llen), 1);
					w_ = (w_ << 8) | llen;
					llen = 0;
				}
			}
			if (key == "height") {
				is.get(tmp);
				if (tmp == 'i') {
					is.read(reinterpret_cast<char*>(&h_), 1);
				}
				if (tmp == 'I') {
					is.read(reinterpret_cast<char*>(&llen), 1);
					h_ = llen;
					llen = 0;
					is.read(reinterpret_cast<char*>(&llen), 1);
					h_ = (h_ << 8) | llen;
					llen = 0;
				}

			}
			if (key == "background") {
				is.get(tmp);
				is.get(tmp);


				// Should be 'U'
				is.get(tmp);

				if (tmp != 'U')
					return 1;
				
				// # indicates number of elements
				is.get(tmp);
				is.get(tmp);
				if (tmp != 'i')
					return 1;
				is.get(tmp);
				len = tmp;
				for (size_t i = 0; i < len; i++) {
					is.get(tmp);
					val = tmp;
					background_.push_back(val);
				}
			}
			key.clear();

		}
	}
};

struct json_image{
	
	unsigned x_ = 0;
	unsigned y_ = 0;
	unsigned width_ = 0;
	unsigned height_ = 0;
	vector<vec3b> data_;

	json_image() = default;

	int build(ifstream& is) {
		char tmp = 0;
		string key;
		uint16_t len = 0, val;
		int64_t llen = 0;
		vec3b tmp_vec(0,0,0);
		while (true) {

			is.get(tmp);
			if (tmp == '}')
				break;

			if (tmp == '{')
				continue;

			if (tmp != 'i')
				return 1;

			//key
			is.get(tmp);
			len = tmp;

			for (size_t i = 0; i < len; i++) {
				is.get(tmp);
				key.push_back(tmp);
			}
			cout << key << ",";
			//values
			if (key == "x") {
				is.get(tmp);
				if ((tmp == 'i') || (tmp == 'U')) {
					is.read(reinterpret_cast<char*>(&x_), 1);
				}
				if (tmp == 'I') {
					is.read(reinterpret_cast<char*>(&llen), 1);
					x_ = llen;
					llen = 0;
					is.read(reinterpret_cast<char*>(&llen), 1);
					x_ = (x_ << 8) | llen;
					llen = 0;
				}

			}

			if (key == "y") {
				is.get(tmp);
				if ((tmp == 'i') || (tmp == 'U')) {
					is.read(reinterpret_cast<char*>(&y_), 1);
				}
				if (tmp == 'I') {
					is.read(reinterpret_cast<char*>(&llen), 1);
					y_ = llen;
					llen = 0;
					is.read(reinterpret_cast<char*>(&llen), 1);
					y_ = (y_ << 8) | llen;
					llen = 0;
				}

			}

			if (key == "width") {
				is.get(tmp);
				if ((tmp == 'i') || (tmp == 'U')) {
					is.read(reinterpret_cast<char*>(&width_), 1);
				}
				if (tmp == 'I') {
					is.read(reinterpret_cast<char*>(&llen), 1);
					width_ = llen;
					llen = 0;
					is.read(reinterpret_cast<char*>(&llen), 1);
					width_ = (width_ << 8) | llen;
					llen = 0;
				}

			}
			if (key == "height") {
				is.get(tmp);
				if ((tmp == 'i')||(tmp == 'U')) {
					is.read(reinterpret_cast<char*>(&height_), 1);
				}
				if (tmp == 'I') {
					is.read(reinterpret_cast<char*>(&llen), 1);
					height_ = llen;
					llen = 0;
					is.read(reinterpret_cast<char*>(&llen), 1);
					height_ = (height_ << 8) | llen;
					llen = 0;
				}

			}
			if (key == "data") {
				is.get(tmp);
				is.get(tmp);


				// Should be 'U'
				is.get(tmp);

				if (tmp != 'U')
					return 1;

				// # indicates number of elements
				is.get(tmp);
				is.get(tmp);
				if ((tmp != 'i') && (tmp != 'I') &&(tmp != 'l'))
					return 1;

				if (tmp == 'l')
					is.read(reinterpret_cast<char*>(&llen), 4);
				if (tmp == 'I')
					is.read(reinterpret_cast<char*>(&len), 2);
				if (tmp == 'i') {
					is.get(tmp);
					len = tmp;
				}
				for (size_t i = 0; i < width_*height_; i++) {
					for (size_t j = 0; j < 3; j++) {
						is.get(tmp);
						val = tmp;
						tmp_vec[j] = val;
					}
					data_.push_back(tmp_vec);
				}
			}
			key.clear();

		}
		cout << endl;
		return 0;
	}
};

void print_obj(ifstream& is) {
	char tmp = 0;
	uint8_t len;
	uint32_t val;
	vector<int> list;
	string name; 

	is.get(tmp);
	while (tmp != '}') {

		//key
		is.get(tmp);
		if (tmp != 'i')
			break;

		is.get(tmp);
		len = tmp;

		for (size_t i = 0; i < len; i++) {
			is.get(tmp);
			name.push_back(tmp);
		}
		
		cout << name << ",";
		
		is.get(tmp);

		//value
		if (tmp == 'i')
			is.read(reinterpret_cast<char*>(&val), 1);
		
		if (tmp == 'I') 
			is.read(reinterpret_cast<char*>(&val), 2);

		if (tmp == '[') {
			is.get(tmp);


			// Should be 'U'
			is.get(tmp);

			if (tmp != 'U')
				return ;

			// # indicates number of elements
			is.get(tmp);
			is.get(tmp);

			if (tmp != 'i')
				return ;

			is.get(tmp);
			len = tmp;
			for (size_t i = 0; i < len; i++) {
				is.get(tmp);
				val = tmp;
				list.push_back(val);
			}
		}
		name.clear();	
	}
	cout << endl;
}

int convert(const string& sInput, const string& sOutput) {

	// Dal file UBJ devo estrarre le informazioni e creare il canvas
	ifstream is(sInput, ios::binary);
	if (!is)
		return EXIT_FAILURE;
	
	canvas cv;
	vector<json_image> ubj_img;
	string name;
	uint8_t len;
	char tmp;

	is.get(tmp);
	if (tmp != '{')
		return EXIT_FAILURE;

	while (true) {
		is.get(tmp);
		
		if (tmp == '}')
			break;

		//key
		if (tmp != 'i')
			return EXIT_FAILURE;
		
		if (name.size() != 0)
			name.clear();

		is.get(tmp);
		len = tmp;
		
		for (size_t i = 0; i < len; i++) {
			is.get(tmp);
			name.push_back(tmp);
		}

		//value
		if (name == "canvas") {

			if (cv.build(is))
				return EXIT_FAILURE;
			image<vec3b> output_canvas(cv.w_, cv.h_);

			for (unsigned r = 0; r < output_canvas.width(); r++) {
				for (unsigned c = 0; c < output_canvas.height(); c++) {
					for (size_t i = 0; i < 3; i++)
						output_canvas(r, c)._data[i] = cv.background_[i];
				}
			}

			if (!writeP6("canvas.ppm", output_canvas))
				return EXIT_FAILURE;

			continue;
		}

		if (name == "elements") {
			is.get(tmp);
			continue;
		}

		cout << name << " : ";

		if (name == "image") {
			json_image js_i;
			if (js_i.build(is))
				return EXIT_FAILURE;
			ubj_img.push_back(js_i);

			image<vec3b> output_img(js_i.width_, js_i.height_);
			string output_name = "image";
			output_name += to_string(ubj_img.size());

			for (unsigned r = 0; r < output_img.width(); r++) {
				for (unsigned c = 0; c < output_img.height(); c++) {
						output_img(r, c) = js_i.data_[c*output_img.width() + r];
				}
			}

			if (!writeP6(output_name + ".ppm", output_img))
				return EXIT_FAILURE;
			continue;
		}

		print_obj(is);
	}



	unsigned w = cv.w_; // TODO : modificare
	unsigned h = cv.h_; // TODO : modificare

	image<vec3b> img(w, h);

	// Salvare le immagini individuate ed il canvas

	// Per accedere ai pixel di img posso usare img(x,y) oppure img.begin() e img.end()
	// Dal file UBJ devo estrarre le informazioni sulle immagini da incollare su img 

	for (unsigned r = 0; r < img.width(); r++) {
		for (unsigned c = 0; c < img.height(); c++) {
			for (size_t i = 0; i < 3; i++)
				img(r, c)._data[i] = cv.background_[i];
		}
	}

	for (const auto& elem : ubj_img) {
		for (unsigned r = 0; r < elem.width_; r++) {
			for (unsigned c = 0; c < elem.height_; c++) {
				img(r + elem.x_, c + elem.y_) = elem.data_[c * elem.width_+ r];
			}
		}
	}

	// Output in formato PPM
	if (!writeP6(sOutput, img))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

	if (argc != 3)
		return EXIT_FAILURE;


	// TODO : gestire la linea di comando
	string sInput(argv[1]);
	string sOutput(argv[2]);

	return convert(sInput, sOutput);
}