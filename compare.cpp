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

    // cout << "TESTING LESS THAN" << endl;
    // cout << ((hello + world) < hw) << endl; // should be false
    // cout << (hw < (hello + world)) << endl; // should be false
    // cout << (hw < world) << endl; // should be true
    // cout << (hello < world) << endl; // should be true
    // cout << (world < hello) << endl; // should be false
    // cout << (world < hw) << endl; // should be false
    // cout << (hello < hello) << endl; // should be false
    // cout << (hello < (hel + lo)) << endl; // should be false
    // cout << ((hel + lo) < hello) << endl; // should be false

    // cout << "TESTING EQUAL TO" << endl;
    // cout << (hello == (hel + lo)) << endl; // should be true
    // cout << (hello == world) << endl; // should be false
    cout << ((hello + world) == hw) << endl; // should be true
    cout << (hello + world) << endl;
    cout << hw << endl;

    // cout << "TESTING LESS THAN OR EQUAL TO" << endl;
    // cout << ((hello + world) <= hw) << endl; // should be true
    // cout << (hw <= (hello + world)) << endl; // should be true
    // cout << (hw <= world) << endl; // should be true
    // cout << (hello <= world) << endl; // should be true
    // cout << (world <= hello) << endl; // should be false
    // cout << (world <= hw) << endl; // should be false
    // cout << (hello <= hello) << endl; // should be true
    // cout << (hello <= (hel + lo)) << endl; // should be true
    // cout << ((hel + lo) <= hello) << endl; // should be true
}
