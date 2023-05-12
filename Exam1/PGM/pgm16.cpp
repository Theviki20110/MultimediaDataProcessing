#include "pgm16.h"


template<typename T>
istream& raw_read(istream& is, T &val, size_t size = sizeof(T)) {
    return is.read(reinterpret_cast<char*>(&val), size);
}


class bitreader {
    uint8_t buffer_;
    uint8_t n_ = 0;
    istream& is_;
public:
    bitreader(istream& is) : is_(is) {}

    uint32_t read_bit() {
        if (n_ == 0) {
            n_ = 8;
            raw_read(is_, buffer_);
        }
        n_--;
        return (buffer_ >> n_) & 1;
    }

    uint32_t read(uint8_t n) {
        uint32_t u = 0;
        while (n -- > 0) {
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

};
void read_comment(ifstream& is, string& comment) {
    char tmp;
    while (is.get(tmp) && (tmp != '\n'))
        comment += tmp;
}


bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue)
{

    // Apro il file PGM indicato
    ifstream is(filename, ios::binary);
    if (!is)
        return false;
    bitreader br(is);
    /*
        Leggo i dati e conservo tutto nella matrice a 16 bit
        e mentre leggo devo salvare il maxvalue del file 
    */
    
    /* Variabile di appoggio */
    char tmp;

    /* Leggo il magic number */
    uint16_t magicnumber;
    magicnumber = br(16);
    

    /* 20533 == P5 ho scelto questa forma per il evitare il warning in OLJ */
    if (magicnumber != 20533)
        return false;

    /* A capo */
    is.get(tmp);

    /* Se il prossimo carattere è # allora c'è un commento */
    tmp = is.peek();

    /* Leggo il commento */
    if (tmp == '#') {
        is.get(tmp);
        string comment;
        read_comment(is, comment);
        //cout << comment << endl;
    }
    
    /* Leggo righe e colonne */
    uint16_t rows, cols;
    is >> cols;
    is >> rows;
    
    /* A capo */
    is.get(tmp);
    
    /* Leggo il maxvalue e lo salvo*/
    is >> maxvalue;

    uint8_t bytes;
    if (maxvalue > 255)
        bytes = 16;
    else
        bytes = 8;

    /* A capo */
    is.get(tmp);
    

    /* Resize della matrice */
    img.resize(rows, cols);
   
    /* Leggo il file e salvo i valori nella matrice */
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            img(r, c) = br(bytes);
        }
    }

    return true;
}


/*int main(int args, char** argc) {

    string filename(argc[1]);
    mat<uint16_t> img;
    uint16_t maxvalue;
    load(filename, img, maxvalue);
    return 0;
}*/