#include "sstring.hpp"
#include "word_maps.hpp"

#include <string>

#ifdef DEBUG
#include <iostream>
#endif

using namespace byteme;

sstring::sstring() {

}

sstring::sstring(std::string data) : sstring(data.c_str()) {

}

sstring::sstring(const char *data) {
    (void) data;
#ifdef DEBUG
    assert(is_ascii(data));
#endif
    encode(data);
}

inline bool sstring::is_alphanumeric(char c) {
    return (('a' <= c && c <= 'z')
            || ('A' <= c && c <= 'Z')
            || ('0' <= c && c <= '9'));
}

void sstring::encode(const char *data) {
    const char *current = data;
    while (*current != '\0') {
        if (is_alphanumeric(*current)) {
            const char *end = current + 1;
            while (is_alphanumeric(*end)) {
                end++;
            }
            // region is [current, end)
            std::string lookup(current, end - current);
#ifdef DEBUG
            std::cout << "Looking up <" << lookup << ">" << std::endl;
#endif
            int key;
            if ((key = short_map[lookup]) != 0) {
                // encode word, point current to next part
                char encoding = 0x80 | ((key - 1) & 0x3f);
                buf.push_back(encoding);
                current = end;
            } else if ((key = long_map[lookup]) != 0) {
                // encode word, point current to next part
                char first_byte = 0xc0 | (((key - 1) >> 8) & 0x3f);
                char second_byte = (key - 1) & 0xff;
                buf.push_back(first_byte);
                buf.push_back(second_byte);
                current = end;
            } else {
                // encode single character
                buf.push_back(*current);
                current++;
            }
        } else {
            // encode single character
            buf.push_back(*current);
            current++;
        }
    }
}

std::string sstring::decode() {
    std::vector<char> output;
    for (size_t i = 0; i < buf.size(); i++) {
        char current = buf[i];
        if ((current & ~0x7f) == 0) {
            output.push_back(current);
        } else if (((current & 0xc0) >> 6) == 0x02) {
            // single byte encoding
            int index = current & 0x3f;
            for (char c : short_decode_array[index]) {
                output.push_back(c);
            }
        } else {
            // double byte encoding
#ifdef DEBUG
            assert(i < buf.size() - 1);
#endif
            int index = ((static_cast<int>(current & 0x3f) & 0xff) << 8) | (static_cast<int>(buf[i + 1]) & 0xff);
            for (char c : long_decode_array[index]) {
                output.push_back(c);
            }
            i++; // account for second byte
        }
    }
    return std::string(output.data(), output.size());
}

size_t sstring::bufferSize() {
    return buf.size();
}

#ifdef DEBUG
bool sstring::is_ascii(const char *data) {
    for (; *data != '\0'; data++) {
        if ((*data & ~0x7f) != 0) {
            // high order bit is set
            return false;
        }
    }
    return true;
}
#endif

sstring::~sstring() {

}
