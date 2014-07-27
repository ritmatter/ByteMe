#include "sstring.hpp"
#include "word_maps.hpp"

#include <iostream>
#include <fstream>

using namespace byteme;
using namespace std;

int main()
{
    sstring hw("HelloWorld");
    sstring hello("Hello");
    sstring world("World");
    sstring hel("Hel");
    sstring lo("lo");
    cout << ((hello + world) < hw) << endl; // should be false
    cout << (hw < (hello + world)) << endl; // should be false
    cout << (hw < world) << endl; // should be true
    cout << (hello < world) << endl; // should be true
    cout << (world < hello) << endl; // should be false
    cout << (world < hw) << endl; // should be false
    cout << (hello < hello) << endl; // should be false
    cout << (hello < (hel + lo)) << endl; // should be false
    cout << ((hel + lo) < hello) << endl; // should be false
}
