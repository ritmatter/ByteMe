#!/usr/bin/python
import pickle

#words = ["a", "b", "apple", "pear", "banana"]
#SMALL_MAP_SIZE = 1
#LARGE_MAP_SIZE = 2
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
i += 1

j = 0
file.write("std::map<std::string, int> long_map = {")
while j < LARGE_MAP_SIZE:
    file.write("{\"" + words[i] + "\"," + str(j) + "},")
    i += 1
    j += 1
file.write("{\"" + words[i] + "\"," + str(j) + "}};\n")

i = 0
file.write("const string small_decode_array[] = {")
while i < SMALL_MAP_SIZE:
    file.write("\"" + words[i] + "\",")
    i += 1
file.write("\"" + words[i] + "\"};\n")
i+= 1

j = 0
file.write("const string large_decode_array[] = {")
while j < LARGE_MAP_SIZE:
    file.write("\"" + words[i] + "\",")
    i += 1
    j += 1
file.write("\"" + words[i] + "\"};")

file.close()
