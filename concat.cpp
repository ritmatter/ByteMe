#include "sstring.hpp"
#include "word_maps.hpp"

#include <iostream>
#include <fstream>

using namespace byteme;
using namespace std;

int main()
{
    sstring first("hello world");
    sstring second(" - this is a test!");
    sstring concat = first + second;
    cout << "Concatenated text: " << concat << endl;
    cout << "Compressed size: " << concat.size() << endl;
    cout << "Decoded size: " << concat.decode().size() << endl;
}
