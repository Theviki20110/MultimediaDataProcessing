#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>

using namespace std;

struct node {
	char sim_;
	float prob_;
	//bool direction;
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

	bool ifleaf() {
		if ((left_e_ == NULL) || (right_e_ == NULL))
			return false;

		return true;
	}
};

int comp(node a, node b) { return a.prob_ < b.prob_; }

node gen_tree(vector<node>& v) {
	if (v.size() < 2)
		//DA SISTEMARE
		exit(3);

	cout << "START:" << endl << endl;
	
	for (auto& elem : v)
		cout << elem.sim_ << " : " << elem.prob_ << endl;

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

		cout << endl;


		for(auto& elem: v)
			cout << elem.sim_ << " : " << elem.prob_ << endl;

		cout << endl;
	}

	cout << "END" << endl;

	return v[0];
}


void print_tree(node* t) {

	if ((t->left_e_ == NULL) || (t->right_e_ == NULL)) {
		cout << "LEAF: " << t->prob_ << endl;
		return;
	}

	cout << "NODE: " << t->prob_ << endl;

	print_tree(t->left_e_);
	
	print_tree(t->right_e_);

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

void encode(const char* input_file, const char* output_file) {
	ifstream is (input_file, ios::binary);
	
	if (is.fail())
		exit(3);
	
	ofstream os(output_file, ios::binary);

	if (os.fail())
		exit(4);

	uint8_t freq;

	map<char, float> m = frequencies(is, &freq);

	vector<node> v = getnodes(m);

	node tree = gen_tree(v);

	os << "HUFFMAN1";

	os << freq;
	
	print_tree(&tree);

}

void decode(const char* input_file, const char* output_file) {
	ifstream is(input_file, ios::binary);

	if (is.fail())
		exit(3);

	ofstream os(output_file);

	if (os.fail())
		exit(4);

}



int main(int args, char** argv) {

	if (args != 4) {
		cout << "Error wrong input parameters";
		return 1;
	}

	string mode(argv[1]);

	if (mode == "c"){
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