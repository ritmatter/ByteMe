#ifndef BYTEME_SSTRING_HPP_
#define BYTEME_SSTRING_HPP_

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

public:

    sstring(const char *data);

    ~sstring();

private:

    char *buf;
    size_t buf_len;

};

}

#endif // BYTEME_SSTRING_HPP_
