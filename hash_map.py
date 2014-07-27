#!/usr/bin/python
import pickle

words = pickle.load( open( "sortedWords.p", "rb" ) )
SMALL_MAP_SIZE = 63
LARGE_MAP_SIZE = 16383

# Write a cpp file for the maps and arrays
file = open("word_maps.cpp", "w")
file.write("#include \"word_maps.hpp\"\n")

# Write the small map
file.write("std::map<std::string, int> byteme::short_map = {")
i = 0
while i < SMALL_MAP_SIZE:
    file.write("{\"" + words[i] + "\"," + str(i + 1) + "},")
    i += 1
file.write("{\"" + words[i] + "\"," + str(i + 1) + "}};\n")
i += 1

# Write the large map
j = 0
file.write("std::map<std::string, int> byteme::long_map = {")
while j < LARGE_MAP_SIZE:
    file.write("{\"" + words[i] + "\"," + str(j + 1) + "},")
    i += 1
    j += 1
file.write("{\"" + words[i] + "\"," + str(j + 1) + "}};\n")

# Write the small array
i = 0
short_size = (1 << 6)
file.write("const std::string byteme::short_decode_array[%d] = {" % short_size)
while i < SMALL_MAP_SIZE:
    file.write("\"" + words[i] + "\",")
    i += 1
file.write("\"" + words[i] + "\"};\n")
i+= 1

# Write the large array
j = 0
long_size = (1 << 14)
file.write("const std::string byteme::long_decode_array[%d] = {" % long_size)
while j < LARGE_MAP_SIZE:
    file.write("\"" + words[i] + "\",")
    i += 1
    j += 1
file.write("\"" + words[i] + "\"};")

file.close()
