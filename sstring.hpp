#ifndef BYTEME_SSTRING_HPP_
#define BYTEME_SSTRING_HPP_

#include <vector>
#include <string>
#include <cstring>

#ifdef DEBUG
#include <cassert>
#endif

namespace byteme
{

/*
 * sstrings are NOT null terminated
 * constructors expect null terminated strings
 */
class sstring {

private:

    void encode(const char *data);

#ifdef DEBUG
    bool is_ascii(const char *data);
#endif

    inline bool is_alphanumeric(char c);

public:

    sstring();

    sstring(std::string data);

    sstring(const char *data);

    std::string decode();

    ~sstring();

    size_t bufferSize();

private:

    // TODO replace with char*
    std::vector<char> buf;

};

}

#endif // BYTEME_SSTRING_HPP_
