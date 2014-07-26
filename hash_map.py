#!/usr/bin/python
import pickle

words = pickle.load( open( "sortedWords.p", "rb" ) )
SMALL_MAP_SIZE = 63
LARGE_MAP_SIZE = 16383

# Write the beginning of the file
file = open("word_maps.cpp", "w")
file.write("#include <map>\n")
file.write("#include <string>\n")

file.write("std::map<std::string, int> short_map = {")

i = 0
while i < SMALL_MAP_SIZE:
    file.write("{\"" + words[i] + "\"," + str(i) + "},")
    i += 1
file.write("{\"" + words[i] + "\"," + str(i) + "}};\n")

file.write("std::map<std::string, int> long_map = {")
while i < LARGE_MAP_SIZE:
    file.write("{\"" + words[i] + "\"," + str(i) + "},")
    i += 1
file.write("{\"" + words[i] + "\"," + str(i) + "}};")

file.close()
