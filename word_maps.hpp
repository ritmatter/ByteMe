#ifndef BYTEME_WORD_MAPS_HPP_
#define BYTEME_WORD_MAPS_HPP_

#include <map>
#include <string>

namespace byteme
{

const int SHORT_SIZE = (1 << 6);
const int LONG_SIZE = (1 << 14);

extern std::map<std::string, int> short_map;

extern std::map<std::string, int> long_map;

extern const std::string short_decode_array[SHORT_SIZE];

extern const std::string long_decode_array[LONG_SIZE];

}

#endif // BYTEME_WORD_MAPS_HPP_
