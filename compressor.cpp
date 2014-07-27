#include "sstring.hpp"
#include "word_maps.hpp"

#include <iostream>
#include <fstream>

using namespace byteme;
using namespace std;

int main(int argc, const char *argv[])
{
    if (argc != 4) {
        cerr << "usage: " << argv[0] << " [-ed] <input file> <output file>" << endl;
        return 1;
    }

    string flag(argv[1]), input(argv[2]), output(argv[3]);

    ifstream fin(input, ios::in | ios::binary | ios::ate);
    size_t input_length = fin.tellg();
    fin.seekg(0, fin.beg);
    char *input_data = new char[input_length];
    fin.read(input_data, input_length);

    ofstream fout(output, ios::out | ios::binary);

    if (flag == "-e") {
        sstring encoded(input_data, input_length);
        fout.write(encoded.data(), encoded.size());
    } else if (flag == "-d") {
        string decoded = sstring::decode(input_data, input_length);
        fout.write(decoded.c_str(), decoded.size());
    }

    delete[] input_data;
}
