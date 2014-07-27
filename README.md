# ByteMe
## A Homomorphic Compression Scheme

#### Anish Athalye, Nikihl Buduma, Matt Ritter
##### Palantir Hackathon, 7/26/14

## Summary

Current compression algorithms can make text file sizes smaller, but the
compression is not homomorphic.  This means that the compressed texts cannot be
operated upon without being decompressed first, which incurs a huge performance
cost if these files are manipulated often. Recently, homomorphic encryption
techniques have been employed on various data stores, but homomorphic
compression is still a subject of active research.

To tackle this problem, we created ByteMe, an API that performs homomorphic
compression on text. ByteMe uses the key insight that a small subset of English
words make up the vast majority of words used. With this knowledge, we created
a variable-length encoding and lookup table compression that maps extremely
common words (the top 64) to one byte and maps common words (the next 16,384)
to two bytes. As it turns out, the most common 1000 English words make up 75%
of words used. This lets us compress a large portion of the text. In addition
to just frequency, the length of a word factors into its placement in our maps.
Thus, a very long word can rank higher and have a greater chance of ending up
in the lookup table.

Words that are not in our most common caches are left uncompressed. One
character words are also left uncompressed, since they would not be made
smaller. Additionally addition, substrings of words that are not common will be
compressed. 

## Example Code

Take a look at code samples in [compressor.cpp](compressor.cpp),
[concat.cpp](concat.cpp), and [compare.cpp](compare.cpp)!

## Compression Scheme

Our schema involves two different lookup tables. The first contains the 64
highest priority words, determined by a combination of frequency and length.
The second contains the next 16,384 highest priority words. The rest of the
words we encounter are placed under the category of printable ASCII. This three
categories allow us to create an encoding scheme like so:

## Byte Boundaries
 
```
|------| |------|
0xxxxxxx            printable ascii
10xxxxxx            frequent words 2^6 = 64
11xxxxxx xxxxxxxx   less frequent words 2^14 = 16384
```

Bytes that are encoded as printable ASCII being with a 0. Bytes beginning with
a “1” exist in the lookup tables. Our 64-word lookup table lets us fit all of
the highest priority words into one byte, since each word hashes to its value
“10” plus the lookup value in the table. Our 16,384-word lookup table lets us
fit all of the second highest priority words into two bytes, since each word
hashes to its value in the table. To get back to uncompressed words, we store
two arrays. A high priority word that hashes to the value “30”, for example,
can be found again at the 30th index of the small array. The same technique
applies to the second priority words.


## Compression Analysis

| TITLE                |  GZIP  |  ByteMe |
|----------------------|--------|---------|
| Pride and Prejudice  |   36%  |   51%   |
| The Scarlet Letter   |   39%  |   54%   |
| 2BR02B               |   44%  |   56%   |

This trade-off in space more than makes up for itself with the advantage of
homomorphic compression. Compressed strings can be compared, concatenated, and
searched in the same asymptotic time as their uncompressed counterparts.
Moreover, these operations do NOT require the data to be decompressed and
recompressed during manipulation. As a result, our implementation is both CPU
and memory efficient.
