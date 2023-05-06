#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;


string found_string(ifstream& is, bool is_key, int tab_level);
void found_integer(ifstream& is, int tab_level);
void found_list(ifstream& is, int tab_level);
void found_dict(ifstream& is, bool is_key, int tab_level);
void check_type(ifstream& is, int tab_level);


void print_tab(int n_tab) {
	for (int i = 0; i < n_tab; i++)
		cout << "\t";
}

void check_type(ifstream& is, int tab_level) {
	char c = is.peek();
	switch (c) {
	break; case 'd': found_dict(is, true, tab_level + 1);
	break; case 'l': found_list(is, tab_level + 1);
	break; case 'i': found_integer(is, tab_level);
	break; default: found_string(is, false, tab_level);
	}
}

string found_string(ifstream& is, bool is_key, int tab_level) {

	char elem, colum;
	size_t len;
	string out;

	if(is_key)
		print_tab(tab_level);

	cout << "\"";
	
	is >> len >> colum;

	for (int i = 0; i < len; i++) {

		is.get(elem);

		if ((elem < 32) || (elem > 126)) {
			cout << ".";
			continue;
		}

		cout << elem;
		if (is_key)
			out += elem;

	}

	cout << "\"";

	if (!is_key)
		cout << endl;

	return out;
}

void found_integer(ifstream& is, int tab_level){

	int64_t value;
	char start, stop;

	/* Leggo il valore, la lettera finale e riporto il valore*/
	is >> start >> value >> stop;
	cout << value << "\n";
}

void found_list(ifstream& is, int tab_level) {
	char start, stop;
	is >> start;
	cout << "[\n";

	while (is.peek() != 'e') {

		print_tab(tab_level + 1);

		check_type(is, tab_level);
	}
	print_tab(tab_level);
	cout << "]\n";
	is >> stop;
}

void print_pieces(ifstream& is, int tab_level){

	char elem, colum;
	size_t len;
	string out;

	is >> len >> colum;

	for (int i = 0; i < len; i++) {
		is.get(elem);

		if (i % 20 == 0) {
			cout << endl;
			print_tab(tab_level);
		}
		
		cout << hex << setfill('0') << setw(2) << (int)(unsigned char)elem;
	}
	cout << endl;
}

void found_dict(ifstream& is, bool is_key, int tab_level) {
	
	char start, stop;
	cout << "{\n";
	is >> start;
	string key;

	while (is.peek() != 'e') {

		/* Chiave */
		key = found_string(is, true, tab_level + 1);

		cout << " => ";

		if (key == "pieces") {
			print_pieces(is, tab_level + 2);
			continue;
		}

		/* Valore */
		check_type(is, tab_level);
	}

	print_tab(tab_level);
	cout << "}\n";

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

	check_type(is, -1);

	return 0;
}