#include "sstring.hpp"
#include "word_maps.hpp"

#include <string>
#include <utility>

#ifdef DEBUG
#include <iostream>
#endif

namespace byteme
{

sstring::sstring() {

}

sstring::sstring(std::string from) : sstring(from.c_str()) {

}

sstring::sstring(std::vector<char> &&from) : buf(from) {

}

sstring::sstring(const char *from) {
#ifdef DEBUG
    assert(is_ascii(from));
#endif
    encode(from);
}

sstring::sstring(const char *from, size_t length) {
#ifdef DEBUG
    assert(is_ascii(from));
#endif
    encode(from, length);
}

inline bool sstring::is_alphanumeric(char c) {
    return (('a' <= c && c <= 'z')
            || ('A' <= c && c <= 'Z')
            || ('0' <= c && c <= '9'));
}

void sstring::encode(const char *from, size_t length) {
    const char *current = from;
    const char *eof = from + length;
    while (current != eof) {
        if (is_alphanumeric(*current)) {
            const char *end = current + 1;
            while (is_alphanumeric(*end)) {
                end++;
            }
            // region is [current, end)
            std::string lookup(current, end - current);
#ifdef DEBUG
            std::cerr << "Looking up <" << lookup << ">" << std::endl;
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

void sstring::encode(const char *from) {
    const char *current = from;
    while (*current != '\0') {
        if (is_alphanumeric(*current)) {
            const char *end = current + 1;
            while (is_alphanumeric(*end)) {
                end++;
            }
            // region is [current, end)
            std::string lookup(current, end - current);
#ifdef DEBUG
            std::cerr << "Looking up <" << lookup << ">" << std::endl;
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

std::string sstring::decode(const char *from, size_t length) {
    std::vector<char> output;
    for (size_t i = 0; i < length; i++) {
        char current = from[i];
        if ((current & ~0x7f) == 0) {
#ifdef DEBUG
            std::cerr << "decoding ascii" << std::endl;
#endif
            output.push_back(current);
        } else if (((current & 0xc0) >> 6) == 0x02) {
            // single byte encoding
#ifdef DEBUG
            std::cerr << "decoding single byte" << std::endl;
#endif
            int index = current & 0x3f;
            for (char c : short_decode_array[index]) {
                output.push_back(c);
            }
        } else {
            // double byte encoding
#ifdef DEBUG
            std::cerr << "decoding double byte" << std::endl;
#endif
#ifdef DEBUG
            assert(i < length - 1);
#endif
            int index = ((static_cast<int>(current & 0x3f) & 0xff) << 8) | (static_cast<int>(from[i + 1]) & 0xff);
            for (char c : long_decode_array[index]) {
                output.push_back(c);
            }
            i++; // account for second byte
        }
    }
    return std::string(output.data(), output.size());
}

std::string sstring::decode() {
    return sstring::decode(buf.data(), buf.size());
}

size_t sstring::size() {
    return buf.size();
}

#ifdef DEBUG
bool sstring::is_ascii(const char *from) {
    for (; *from != '\0'; from++) {
        if ((*from & ~0x7f) != 0) {
            // high order bit is set
            return false;
        }
    }
    return true;
}
#endif

const char *sstring::data() {
    return buf.data();
}

sstring sstring::operator+(const sstring &that) {
    std::vector<char> concatenated = buf;
    concatenated.insert(concatenated.end(), that.buf.begin(), that.buf.end());
    return sstring(std::move(concatenated));
}

std::ostream &operator<<(std::ostream &out, sstring ss) {
    out << ss.decode();
    return out;
}

bool sstring::operator<(const sstring &that) {
    size_t here = 0, there = 0;
    size_t hs = 0, ts = 0;
#ifdef DEBUG
    std::cerr << buf.size() << std::endl;
#endif
    while (here < buf.size() && there < that.buf.size()) {
#ifdef DEBUG
    std::cerr << here << std::endl;
#endif
        int l = buf[here] & 0xff;
#ifdef DEBUG
        std::cerr << "l to int at here: " << here << " l: " << l << std::endl;
#endif
        int r = that.buf[there] & 0xff;

        bool cont = false;
        int cont_here_incr = 0;
        int cont_there_incr = 0;

        // compute left char
        char lc = '\0';
        bool increment_hs = false;
        bool increment_here = false;
        if ((l & ~0x7f) == 0) {
            // ascii
#ifdef DEBUG
            std::cerr << "lc ascii" << std::endl;
#endif
            lc = buf[here];
            increment_here = true;
        } else if ((l & 0xc0 >> 6) == 0x02) {
            // single byte encoding
#ifdef DEBUG
            std::cerr << "lc single byte: " << std::endl;
#endif
            int index = buf[here] & 0x3f;
            const std::string &lstr = short_decode_array[index];
            if (hs >= lstr.size()) {
                hs = 0;
                cont = true;
                cont_here_incr = 1;
            } else {
                increment_hs = true;
                lc = lstr[hs];
            }
        } else {
            // two byte encoding
#ifdef DEBUG
            std::cerr << "lc double byte" << std::endl;
#endif
            int index = ((static_cast<int>(buf[here] & 0x3f) & 0xff) << 8) | (static_cast<int>(buf[here + 1]) & 0xff);
            const std::string &lstr = long_decode_array[index];
            if (hs >= lstr.size()) {
                hs = 0;
                cont = true;
                cont_here_incr = 2;
            } else {
                increment_hs = true;
                lc = lstr[hs];
            }
        }

        // compute right char
        char rc = '\0';
        bool increment_ts = false;
        bool increment_there = false;
        if ((r & ~0x7f) == 0) {
            // ascii
#ifdef DEBUG
            std::cerr << "rc ascii" << std::endl;
#endif
            rc = that.buf[there];
            increment_there = true;
        } else if ((r & 0xc0 >> 6) == 0x02) {
            // single byte encoding
#ifdef DEBUG
            std::cerr << "rc single byte" << std::endl;
#endif
            int index = that.buf[there] & 0x3f;
            const std::string &rstr = short_decode_array[index];
            if (ts >= rstr.size()) {
                ts = 0;
                cont = true;
                cont_there_incr = 1;
            } else {
                increment_ts = true;
                rc = rstr[ts];
            }
        } else {
            // two byte encoding
#ifdef DEBUG
            std::cerr << "rc double byte" << std::endl;
#endif
            int index = ((static_cast<int>(that.buf[there] & 0x3f) & 0xff) << 8) | (static_cast<int>(that.buf[there + 1]) & 0xff);
            const std::string rstr = long_decode_array[index];
            if (ts >= rstr.size()) {
                ts = 0;
                cont = true;
                cont_there_incr = 2;
            } else {
                increment_ts = true;
                rc = rstr[ts];
            }
        }

        if (cont) {
            here += cont_here_incr;
            there += cont_there_incr;
            continue;
        }

        if (increment_here) {
            here++;
        }
        if (increment_there) {
            there++;
        }
        if (increment_hs) {
            hs++;
        }
        if (increment_ts) {
            ts++;
        }
#ifdef DEBUG
        std::cerr << "comparing " << lc << " with " << rc << std::endl;
#endif
        if (lc != rc) {
#ifdef DEBUG
            std::cerr << "equals detected difference: " << lc << " <? " << rc << std::endl;
#endif
            return lc < rc;
        }
    }
#ifdef DEBUG
    std::cerr << "equals reached end of buffer" << std::endl;
#endif

    if (here >= buf.size() && there >= that.buf.size()) {
        return false;
    } else {
        return here >= buf.size();
    }
}

bool sstring::operator==(const sstring &that) {
    size_t here = 0, there = 0;
    size_t hs = 0, ts = 0;
#ifdef DEBUG
    std::cerr << buf.size() << std::endl;
#endif
    while (here < buf.size() && there < that.buf.size()) {
#ifdef DEBUG
    std::cerr << here << std::endl;
#endif
        int l = buf[here] & 0xff;
#ifdef DEBUG
        std::cerr << "l to int at here: " << here << " l: " << l << std::endl;
#endif
        int r = that.buf[there] & 0xff;

        bool cont = false;
        int cont_here_incr = 0;
        int cont_there_incr = 0;

        // compute left char
        char lc = '\0';
        bool increment_hs = false;
        bool increment_here = false;
        if ((l & ~0x7f) == 0) {
            // ascii
#ifdef DEBUG
            std::cerr << "lc ascii" << std::endl;
#endif
            lc = buf[here];
            increment_here = true;
        } else if ((l & 0xc0 >> 6) == 0x02) {
            // single byte encoding
#ifdef DEBUG
            std::cerr << "lc single byte: " << std::endl;
#endif
            int index = buf[here] & 0x3f;
            const std::string &lstr = short_decode_array[index];
            if (hs >= lstr.size()) {
                hs = 0;
                cont = true;
                cont_here_incr = 1;
            } else {
                increment_hs = true;
                lc = lstr[hs];
            }
        } else {
            // two byte encoding
#ifdef DEBUG
            std::cerr << "lc double byte" << std::endl;
#endif
            int index = ((static_cast<int>(buf[here] & 0x3f) & 0xff) << 8) | (static_cast<int>(buf[here + 1]) & 0xff);
            const std::string &lstr = long_decode_array[index];
            if (hs >= lstr.size()) {
                hs = 0;
                cont = true;
                cont_here_incr = 2;
            } else {
                increment_hs = true;
                lc = lstr[hs];
            }
        }

        // compute right char
        char rc = '\0';
        bool increment_ts = false;
        bool increment_there = false;
        if ((r & ~0x7f) == 0) {
            // ascii
#ifdef DEBUG
            std::cerr << "rc ascii" << std::endl;
#endif
            rc = that.buf[there];
            increment_there = true;
        } else if ((r & 0xc0 >> 6) == 0x02) {
            // single byte encoding
#ifdef DEBUG
            std::cerr << "rc single byte" << std::endl;
#endif
            int index = that.buf[there] & 0x3f;
            const std::string &rstr = short_decode_array[index];
            if (ts >= rstr.size()) {
                ts = 0;
                cont = true;
                cont_there_incr = 1;
            } else {
                increment_ts = true;
                rc = rstr[ts];
            }
        } else {
            // two byte encoding
#ifdef DEBUG
            std::cerr << "rc double byte" << std::endl;
#endif
            int index = ((static_cast<int>(that.buf[there] & 0x3f) & 0xff) << 8) | (static_cast<int>(that.buf[there + 1]) & 0xff);
            const std::string rstr = long_decode_array[index];
            if (ts >= rstr.size()) {
                ts = 0;
                cont = true;
                cont_there_incr = 2;
            } else {
                increment_ts = true;
                rc = rstr[ts];
            }
        }

        if (cont) {
            here += cont_here_incr;
            there += cont_there_incr;
            continue;
        }

        if (increment_here) {
            here++;
        }
        if (increment_there) {
            there++;
        }
        if (increment_hs) {
            hs++;
        }
        if (increment_ts) {
            ts++;
        }
#ifdef DEBUG
        std::cerr << "comparing " << lc << " with " << rc << std::endl;
#endif
        if (lc != rc) {
#ifdef DEBUG
            std::cerr << "equals detected difference: " << lc << " != " << rc << std::endl;
#endif
            return false;
        }
    }
#ifdef DEBUG
    std::cerr << "equals reached end of buffer" << std::endl;
#endif

    return (here >= buf.size() && there >= that.buf.size());
}

bool sstring::operator<=(const sstring &that){
    return (*this < that) || (*this == that);
}

sstring sstring::substr(size_t position, size_t length) {

    size_t here = 0;
    size_t count = 0;
    std::vector<char> output;
    std::string left_word;

    while (count < position && here < buf.size()){
        char left = buf[here];
        if((left & 0xff) >> 6 == 0x03){
            int index = ((static_cast<int>(left & 0x3f) & 0xff) << 8) | (static_cast<int>(buf[here + 1]) & 0xff);
            left_word = long_decode_array[index];
            count += left_word.size();
            here += 2;
        } else if(((left & 0xc0) >> 6) == 0x02){
            int index = left & 0x3f;
            left_word = short_decode_array[index];
            count += left_word.size();
            here++;
        } else{
            count++;
            here++;
        }
    }

    if (count > position){
        size_t counter2 = count - left_word.size();
        for (char c : left_word) {
            if (counter2 >= position and output.size() < length){
                output.push_back(c);
            }
            counter2++;
        }
    }

    while(output.size() < length && here < buf.size()){
        char left = buf[here];
        if((left & 0xff) >> 6 == 0x03){
            int index = ((static_cast<int>(left & 0x3f) & 0xff) << 8) | (static_cast<int>(buf[here + 1]) & 0xff);
            left_word = long_decode_array[index];
            for (char c : left_word){
                if(output.size() < length){
                    output.push_back(c);
                }
            }
            here += 2;
        } else if(((left & 0xc0) >> 6) == 0x02){
            int index = left & 0x3f;
            left_word = short_decode_array[index];
            for (char c : left_word){
                if(output.size() < length){
                    output.push_back(c);
                }
            }
            here++;
        } else{
            output.push_back(left);
            here++;
        }

    }

    return sstring(std::move(output));


}

sstring::~sstring() {

}

}
