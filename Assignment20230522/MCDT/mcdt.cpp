#define _USE_MATH_DEFINES

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct elems {

	vector<double> cos_;
	vector<double> sin_;
	int window_;
	double tmp;
	elems(int window) {
		window_ = window;


		for (int k = 0; k < window; k++) {
			for (int n = 0; n < 2 * window; n++) {
				
				if (sin_.size() < 2 * static_cast<unsigned long long>(window)) {
					tmp = sin((n + 0, 5) * (M_PI / (2 * window)));
					sin_.push_back(tmp);
				}

				tmp = cos((M_PI / window) * (n + 0, 5 + window / static_cast<double>(2)) * (k + 0, 5));
				cos_.push_back(tmp);
			}
		}
	
	};

};


map<int16_t, int> read_file(const string& filename, vector<int16_t>& sym){


	ifstream is(filename, ios::binary);
	map<int16_t, int> freq;

	int16_t value;

	while (true) {
		is.read(reinterpret_cast<char*> (&value), sizeof(int16_t));

		if (is.fail())
			break;

		sym.push_back(value);
		
		freq[value]++;
	}

	return freq;

}


template <typename T>
bool save_file(const string& filename, vector<T>& sym) {

	ofstream os(filename, ios::binary);

	if (os.fail())
		return false;

	for (size_t i = 0; i < sym.size(); i++)
		os.write(reinterpret_cast<char*>(&sym[i]), sizeof(int16_t));

	return true;
}


template <typename T, typename Q>
double compute_entropy(map<T, Q>& freq, uint32_t len) {
	
	double entropy = 0;


	for (const auto& elem : freq) {
		double p_x = (double) elem.second / len ;
		entropy += (p_x * log2(1/p_x));
	}

	return entropy;
}

template <typename T>
vector<double> compute_mcdt(vector<T>& sym, elems& e, bool coeff) {


	vector<double> out;

	if (coeff) {
		out.resize(e.window_);

		/* Padding */
		for (size_t i = 0; i < e.window_; i++) {
			sym.push_back(0);
			sym.insert(sym.begin(), 0);
		}

		for (int k = 0; k < e.window_; k++) {

			for (int n = 0; n < 2 * e.window_; n++) {

				//cout << i << " " << k << " " << n << endl;
				out[k] += (sym[n] * e.sin_[n] * e.cos_[k + n]);
			}
		}
		
	}
	else {
		out.resize(2 * e.window_);
		for (int k = 0; k < 2 * e.window_; k++) {

			for (int n = 0; n < e.window_; n++) {

				//cout << i << " " << k << " " << n << endl;
				out[k] += (2 / e.window_) * (sym[n] * e.sin_[k] * e.cos_[k + n]);
			}
		}
	}
	
	return out;
}


int main(int args, char** argv) {

	vector<int16_t> sym;
	map<int16_t, int> freq = read_file(argv[1], sym);
	uint32_t len = (uint32_t)sym.size();

	//cout << compute_entropy(freq, len) << endl;

	elems e(1024);

	vector<double> coeff = compute_mcdt(sym, e, true);


	vector<int32_t> sym_quantized, sym_dequantized, rebuilt_signal;
	
	for (const auto& elem : coeff)
		sym_quantized.push_back((uint32_t)elem / 10000);

	map<int32_t, int> freq_quantized;
	
	for (const auto& elem : sym_quantized)
		freq_quantized[elem]++;

	cout << compute_entropy(freq_quantized, len) << endl;
	
	for (const auto& elem : sym_quantized)
		sym_dequantized.push_back((uint32_t)elem * 10000);
	
	vector<double> recostructed_coeff = compute_mcdt(rebuilt_signal, e, false);

	save_file("output_qt.raw", recostructed_coeff);

	vector<uint32_t> diff;
	for (size_t i = 0; i < sym.size(); i++)
		diff.push_back(coeff[i] - sym_quantized[i]);

	save_file("error_qt.raw", diff);




	return 0;
}