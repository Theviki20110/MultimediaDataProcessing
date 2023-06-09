#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

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
		for (int i = n - 1; i >= 0; --i) {
			write_bit(u >> i);
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

struct node {
	char sim_;
	float prob_;
	node* left_e_;
	node* right_e_;

	node(char sim, float prob, node* left_e, node* right_e) {
		sim_ = sim;
		prob_ = prob;
		left_e_ = left_e;
		right_e_ = right_e;
	}

	node(char sim, float prob) {
		sim_ = sim;
		prob_ = prob;
		left_e_ = NULL;
		right_e_ = NULL;
	}

	bool isleaf() {
		if ((left_e_ == NULL) || (right_e_ == NULL))
			return true;

		return false;
	}
};

int comp(node a, node b) { return a.prob_ < b.prob_; }

node* gen_tree(vector<node>& v) {
	if (v.size() < 2)
		//DA SISTEMARE
		exit(4);

	//cout << "START:" << endl << endl;
	/*for (auto& elem : v)
		cout << elem.sim_ << " : " << elem.prob_ << endl;*/

	while (v.size() != 1) {

		// Create new node sum of two probabilities and linked with previous nodes
		node* v_0 = new node(v[0].sim_, v[0].prob_, v[0].left_e_, v[0].right_e_);
		node* v_1 = new node(v[1].sim_, v[1].prob_, v[1].left_e_, v[1].right_e_);

		node tmp(v_0->sim_, v_0->prob_ + v_1->prob_, v_0, v_1);

		// Remove used nodes
		v.erase(v.begin(), v.begin() + 2);

		// Insert sum node
		v.push_back(tmp);

		// Sort vector
		sort(v.begin(), v.end(), comp);


		//cout << endl;
		/*for (auto& elem : v)
			cout << elem.sim_ << " : " << elem.prob_ << endl;*/
			//cout << endl;
	}

	/*cout << "END" << endl << endl;*/

	node* root = new node(v[0].sim_, v[0].prob_, v[0].left_e_, v[0].right_e_);

	return root;
}

void print_encode(node* t, string encoding, unordered_map<char, string>& codes, unordered_map<char, uint32_t> lenght_codes) {

	if (t->isleaf()) {
		////cout << encoding << " " << t->sim_ << endl;

		codes[t->sim_] = encoding;
		lenght_codes[t->sim_] = encoding.size();
		return;
	}

	////cout << "NODE: " << t->prob_ << endl;

	print_encode(t->left_e_, encoding.append("0"), codes, lenght_codes);
	encoding.pop_back();

	print_encode(t->right_e_, encoding.append("1"), codes, lenght_codes);
}

void printBT(const std::string& prefix, const node* node, bool isLeft)
{
	if (node != nullptr)
	{
		std::cout << prefix;

		std::cout << (isLeft ? "|--" : "L--");

		// print the value of the node
		std::cout << node->prob_ << std::endl;

		// enter the next tree level - left and right branch
		if (node->left_e_ != nullptr) {
			printBT(prefix + (isLeft ? "|   " : "    "), node->left_e_, true);
		}

		if (node->right_e_ != nullptr) {
			printBT(prefix + (isLeft ? "|   " : "    "), node->right_e_, false);
		}
	}
	else {
		return;
	}
}

void printBT(const node* node)
{
	printBT("", node, false);
}

vector<node> getnodes(map<char, float> m) {
	vector<node> v;

	for (const auto& item : m) {
		auto tmp = node(item.first, item.second);
		v.push_back(tmp);
	}

	sort(v.begin(), v.end(), comp);

	return v;
}

map<char, float> frequencies(ifstream& is, uint8_t* sum) {
	char c;
	int count = 0;
	map<char, float> out;

	while (is.read(&c, sizeof(char))) {
		if (is.fail())
			break;

		count++;

		if (out.count(c)) {
			out[c]++;
			continue;
		}

		out[c] = 1;
	}

	if (count == 256)
		*sum = 0;

	*sum = count;

	for (auto& elem : out)
		elem.second /= count;

	/*for (auto elem : out)
		cout << elem.first << ": " << elem.second << endl;*/

	return out;
}

unordered_map<string, char> get_encode(unordered_map<char, uint32_t> codes) {
	vector<string> v;
	unordered_map<string, char> out;
	for (const auto& elem : codes) {
		
		/* First Element */
		if (out.size() == 0) {
			v.push_back("0");
			out["0"] = elem.first;
			continue;
		}

		string last_code = v.back();
		string* tmp = new string(last_code);
		tmp->back() = '1';

		/* Found new code */
		if (last_code.size() != elem.second)
			tmp->push_back('0');

		v.push_back(*tmp);
		out[*tmp] = elem.first;
	}

	return out;
}

void encode(const char* input_file, const char* output_file) {

	ifstream is(input_file, ios::binary);
	ofstream os(output_file, ios::binary);

	if ((os.fail()) || (is.fail()))
		exit(3);

	os << "HUFFMAN2";

	uint8_t freq;

	map<char, float> m = frequencies(is, &freq);

	bitwriter bw(os);

	bw.write(m.size(), 8);

	vector<node> v = getnodes(m);

	node* tree = gen_tree(v);

	unordered_map<char, string> codes;

	unordered_map<char, uint32_t> lenght_codes;

	print_encode(tree, string(""), codes, lenght_codes);


	for (const auto& elem : codes) {
		
		////cout << elem.first << " " << elem.second << endl;

		uint32_t len = elem.second.length();

		bw.write(elem.first, 8);

		bw.write(len, 5);

	}

	bw.write(freq, 32);

	is.clear();
	is.seekg(0);

	char sim;

	while (is >> sim) {
		string elem(codes[sim]);
		for (size_t i = 0; i < elem.length(); i++)
			bw.write(elem[i], 1);
	}

	//printBT(tree);
}

void decode(const char* input_file, const char* output_file) {

	ifstream is(input_file, ios::binary);
	ofstream os(output_file, ios::binary);

	if ((is.fail()) || (os.fail()))
		return;

	bitreader br(is);

	/* Read MagicNumber */
	string magicnumber("");
	while (magicnumber.length() != 8)
		magicnumber.push_back(br.read(8));
	//cout << "MAGICNUMBER: " << magicnumber << endl << endl;

	/* Read TableEntries 8 bit */
	uint32_t table_entries = br.read(8);
	//cout << "TableEntries: " << table_entries << endl << endl;

	/* Read HuffmanTable */
	unordered_map<char, uint32_t> sym;
	for (size_t i = 0; i < table_entries; i++) {

		char c = br.read(8);
		uint32_t len = br.read(5);
		
		sym[c] = len;
	}

	/* Get codes from HuffmanTable */
	unordered_map<string, char> codes = get_encode(sym);

	//for (const auto& elem : sym)
		//cout << elem.first << " " << elem.second << endl;

	//cout << endl;

	//for (const auto& elem : codes)
		//cout << elem.first << " " << elem.second << endl;

	//cout << endl;

	/* Read NumSymbols */
	uint32_t num_symbols = br.read(32);

	//cout << "NumSymbols: " << num_symbols << endl;

	/* Decode */
	string code;
	uint32_t i = 0;
	while (i < num_symbols) {
		code.push_back(br(1) + '0');

		if (codes.find(code) != codes.end()) {
			i++;
			os << codes[code];
			code.clear();
		}
	}
}


int main(int args, char** argv) {

	if (args != 4) {
		cout << "Error wrong input parameters";
		return 1;
	}

	string mode(argv[1]);

	if (mode == "c") {
		encode(argv[2], argv[3]);
		return 0;
	}
	if (mode == "d") {
		decode(argv[2], argv[3]);
		return 0;
	}
	else {
		cout << "Error: wrong letter mode";
		return 2;
	}
}