#include "sstring.hpp"
#include "word_maps.hpp"

#include <iostream>
#include <cstdio>

using namespace byteme;
using namespace std;

int main()
{
    string test = "If you asked me, Nikhil.";
    sstring comp(test);
    // sstring comp("If you asked me, Nikhil.");
    cout << "original size: " << test.size() << endl;
    cout << "compressed size: " << comp.bufferSize() << endl;
    cout << "original text: " << test << endl;
    cout << "decoded text: " << comp.decode() << endl;
}
