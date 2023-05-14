#include "pbm.h"
#include <iostream>

bool check_header(ifstream& is, int* width, int* height) {

	string tmp;
	char c;

	/* Read MagicNumber */
	is >> tmp;
	
	if (tmp != "P4")
		return false;

	/* Read Line Feed */
	is.get(c);

	/* Read optional comment */
	if (is.peek() == '#') {
		is.get(c);
		string comment;
		is >> comment;

		/* Read Line Feed */
		is.get(c);
	}

	/* Read width and height */
	is >> *width;
	is >> *height;

	/* Read Line Feed */
	is.get(c);

	return true;
}


bool BinaryImage::ReadFromPBM(const std::string& filename)
{
	ifstream is(filename, ios::binary);
	if (is.fail())
		return false;

	int width, height;

	if(!check_header(is, &width, &height))
		return false;

	this->W = width;
	this->H = height;

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width/8; c++) {
			this->ImageData.push_back(is.get());
		}

		if(width % 8 != 0)
			this->ImageData.push_back(is.get());
	}


	return true;
}


/*int main(int args, char** argv) {
	BinaryImage bi;
	bi.ReadFromPBM(argv[1]);
	return 0;
}*/
