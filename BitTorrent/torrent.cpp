#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

string found_string(ifstream& is, ofstream& os, bool is_key, int tab_level);
void found_integer(ifstream& is, ofstream& os, int tab_level);
void found_list(ifstream& is, ofstream& os, int tab_level);
void found_dict(ifstream& is, ofstream& os, bool is_key, int tab_level);
void check_type(ifstream& is, ofstream& os, int tab_level);


void print_tab(ofstream& os, int n_tab) {
	for (int i = 0; i < n_tab; i++)
		os << "\t";
}

void check_type(ifstream& is, ofstream& os, int tab_level) {
	char c = is.peek();
	switch (c) {
	break; case 'd': found_dict(is, os, true, tab_level + 1);
	break; case 'l': found_list(is, os, tab_level + 1);
	break; case 'i': found_integer(is, os, tab_level);
	break; default: found_string(is, os, false, tab_level);
	}
}

string found_string(ifstream& is, ofstream& os, bool is_key, int tab_level) {

	char elem, colum;
	size_t len;
	string out;

	if(is_key)
		print_tab(os, tab_level);
	else
		os << "\"";
	
	is >> len >> colum;

	for (int i = 0; i < len; i++) {

		is.get(elem);

		if ((elem < 32) || (elem > 126)) {
			os << ".";
			continue;
		}

		os << elem;
		if (is_key)
			out += elem;

	}

	if (!is_key)
		os << "\"" << endl;

	return out;
}

void found_integer(ifstream& is, ofstream& os, int tab_level){

	int64_t value;
	char start, stop;

	/* Leggo il valore, la lettera finale e riporto il valore*/
	is >> start >> value >> stop;
	os << value << "\n";
}

void found_list(ifstream& is, ofstream& os, int tab_level) {
	char start, stop;
	is >> start;
	os << "[\n";

	while (is.peek() != 'e') {

		print_tab(os, tab_level + 1);

		check_type(is, os, tab_level);
	}
	print_tab(os, tab_level);
	os << "]\n";
	is >> stop;
}

void print_pieces(ifstream& is, ofstream& os, int tab_level){

	char elem, colum;
	size_t len;
	string out;

	is >> len >> colum;

	for (int i = 0; i < len; i++) {
		is.get(elem);

		if (i % 20 == 0) {
			os << endl;
			print_tab(os, tab_level);
		}
		
		printf("%x", elem & 0xff);
	
	}
	os << endl;
}

void found_dict(ifstream& is, ofstream& os, bool is_key, int tab_level) {
	
	char start, stop;
	os << "{\n";
	is >> start;
	string key;

	while (is.peek() != 'e') {

		/* Chiave */
		key = found_string(is, os, true, tab_level + 1);

		os << " => ";

		if (key == "pieces") {
			print_pieces(is, os, tab_level + 1);
			continue;
		}

		/* Valore */
		check_type(is, os, tab_level);
	}

	print_tab(os, tab_level);
	os << "}\n";

	is >> stop;

}



int main(int args, char** argv) {

	string filename;
	string input(argv[1]);
	for (const auto& elem : input) {
		if (elem == '.')
			break;
		filename.push_back(elem);
	}

	ifstream is(argv[1], ios::binary);
	if (is.fail())
		return -1;

	ofstream os(filename + ".dump");
	if (os.fail())
		return -1;

	check_type(is, os, -1);

	return 0;
}