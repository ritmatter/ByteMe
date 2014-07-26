#include "sstring.hpp"

using namespace byteme;

sstring::sstring(const char *data) : buf(nullptr), buf_len(0) {
#ifdef DEBUG
    assert(is_ascii(data));
#endif
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
    if (buf != nullptr) {
        delete buf;
    }
}
