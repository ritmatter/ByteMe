#!/usr/bin/env python

from os import listdir, path
from os.path import isfile, join
import re, pickle

suffix = '.txt'

mypath = path.dirname(path.abspath(__file__))
txtfiles = [ f for f in listdir(mypath) if isfile(join(mypath,f)) ]
txtfiles = [f for f in txtfiles if f.endswith(suffix)]

word_list = []

for f in txtfiles:
	file_handle = open(f, 'r')
	string = file_handle.read()
	word_list.extend([w for w in re.split('\W', string) if w])

word_freq = {}

for word in word_list:
	if word in word_freq:
		word_freq[word] += 1
	else:
		word_freq[word] = 1

sorted_words = [word for word in word_freq.keys() if len(word) > 1]
sorted_words = sorted(sorted_words, key=lambda x: -word_freq[x])

small = sorted_words[:64]
large = sorted_words[64:]
large = [word for word in large if len(word) > 2]
small.extend(large)

pickle.dump(small, open("../sortedWords.p", "wb"))
