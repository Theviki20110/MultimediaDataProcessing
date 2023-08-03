#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>

using namespace std;

template<typename T>
struct mat {
    size_t rows_, cols_;
    std::vector<T> data_;

    mat(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows*cols) {}

    const T& operator[](size_t i) const { return data_[i]; }
    T& operator[](size_t i) { return data_[i]; }

    size_t size() const { return rows_ * cols_; }
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }

    const char* rawdata() const {
        return reinterpret_cast<const char*>(data_.data());
    }
    size_t rawsize() const { return size() * sizeof(T); }
};

template<typename T>
istream& raw_read(istream& is, T& buffer, size_t size = sizeof(T)) {
    return is.read(reinterpret_cast<char*>(&buffer), size);
}

struct qoi_header {
    string magic_;
    uint32_t width = 0;
    uint32_t height = 0;
    uint8_t channels = 0;
    uint8_t colorspace = 0;

    qoi_header() = default;
    int build(ifstream& is) {
        char tmp;
        for (size_t i = 0; i < 4; i++) {
            is.get(tmp);
            magic_.push_back(tmp);
        }

        if (magic_ != "qoif")
            return 1;

        for (size_t i = 0; i < 4; i++) {
            is.get(tmp);
            width = (width << 8) | tmp;
        }

        for (size_t i = 0; i < 4; i++) {
            is.get(tmp);
            height = (height << 8) | tmp;
        }

        raw_read(is, channels);
        raw_read(is, colorspace);

        return EXIT_SUCCESS;
    }
};

uint8_t wrapAround(int first, int second) {
    if (first + second < 0) { // Difetto
        return first + second + 256;
    }
    if (first + second > 255) { // Eccesso
        return first + second - 256;
    }
    return first + second; // Non necessito del wraparound
}

int checkEnd(ifstream& is) {

    uint8_t n_zeros = 0;
    uint8_t tmp = 0;
    while (tmp == 0) {
        n_zeros++;
        is.read(reinterpret_cast<char*> (&tmp), 1);
    }
    is.read(reinterpret_cast<char*>(&tmp), 1);
    if ((n_zeros == 7) && (tmp == 1))
        return 1;
    return 0;
}


int main(int argc, char *argv[])
{
    // TODO: Manage the command line 
    if (argc != 3)
        return EXIT_FAILURE;

    string sInput(argv[1]);
    string sOutput(argv[2]);
   
    // TODO: Lettura dell'header e delle dimensioni dell'immagine 
    ifstream is(sInput, ios::binary);
    if (!is)
        return EXIT_FAILURE;

    qoi_header qh;
    
    if (qh.build(is))
        return EXIT_FAILURE;

    using rgba = std::array<uint8_t, 4>; // Potete modificare questa definizione!
    
    mat<rgba> img(qh.height, qh.width); // TODO: Dovete mettere le dimensioni corrette!

    // TODO: decodificare il file QOI in input e inserire i dati nell'immagine di output
    rgba prev_pixel = { 0, 0, 0, 255 };
    array<rgba, 64> dict = {};

    uint8_t tmp = 0, n_rep = 1, index = 0;
    size_t g_index = 0;
    uint64_t size = qh.width * qh.height;

   while(raw_read(is, tmp)) {

        /* Index */
        if (((tmp >> 6) & 0b11) == 0) {

            if ((tmp == 0) && (is.peek() == 0)) {
                if (checkEnd(is))
                    break;
            }

            index = tmp & 0b00111111;
            prev_pixel = dict[index];
        }

        /* Difference */
        if (((tmp >> 6) & 0b11) == 1) {
            uint8_t dr = 0, dg = 0, db = 0;

            dr = (tmp >> 4) & 0b11;
            dg = (tmp >> 2) & 0b11;
            db = tmp        & 0b11;

            prev_pixel[0] = wrapAround(dr - 2, prev_pixel[0]);
            prev_pixel[1] = wrapAround(dg - 2, prev_pixel[1]);
            prev_pixel[2] = wrapAround(db - 2, prev_pixel[2]);
        }

        /*LUMA*/
        if (((tmp >> 6) & 0b11) == 2) {

            uint8_t diff_green = tmp & 0b111111;

            raw_read(is, tmp);
            
            uint8_t diff_red = (tmp >> 4) & 0b1111;
            uint8_t diff_blue = tmp & 0b1111;

            prev_pixel[0] = wrapAround(diff_red - 8 ,  prev_pixel[0] + diff_green - 32);
            prev_pixel[1] = diff_green - 32 + prev_pixel[1];
            prev_pixel[2] = wrapAround(diff_blue - 8, prev_pixel[2] + diff_green - 32);
        }


        /* RGBA pixel or RUN */
        if (((tmp >> 6) & 0b11) == 3) {
            
            /*Pixel*/
            if (tmp == 255) {
                raw_read(is, prev_pixel[0]);
                raw_read(is, prev_pixel[1]);
                raw_read(is, prev_pixel[2]);
                raw_read(is, prev_pixel[3]);
            }

            if (tmp == 254) {
                raw_read(is, prev_pixel[0]);
                raw_read(is, prev_pixel[1]);
                raw_read(is, prev_pixel[2]);
            }

            /*Run*/
            else {
                n_rep = (tmp & 0b111111) + 1;
            }
        }


        index = (prev_pixel[0] * 3 + prev_pixel[1] * 5 + prev_pixel[2] * 7 + prev_pixel[3] * 11) % 64;
        dict[index] = prev_pixel;

        for (uint8_t i = 0; i < n_rep; i++){
            if (g_index >= img.size())
                break;
            img[g_index] = prev_pixel;
            g_index++;
        }
        n_rep = 1;
   }
   
    // Questo è il formato di output PAM. È già pronto così, ma potete modificarlo se volete
    std::ofstream os(sOutput, std::ios::binary); // Questo utilizza il secondo parametro della linea di comando!
    os << "P7\nWIDTH " << img.cols() << "\nHEIGHT " << img.rows() <<
        "\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
    os.write(img.rawdata(), img.rawsize());

    return EXIT_SUCCESS;
}