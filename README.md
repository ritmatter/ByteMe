ByteMe : a homomorphic compression API
======
Anish Athalye, Nikihl Buduma, Matt Ritter
Palantir Hackathon, 7/26/14

Current compression algorithms can make text file sizes smaller, but the compression is not homomorphic.  This means that the compressed texts cannot be operated upon without being decompressed first, which incurs a huge performance cost if these files are manipulated often. Recently, homomorphic encryption techniques have been employed on various data stores, but homomorphic compression is still a subject of active research.

To tackle this problem, we created ByteMe, an API that performs homomorphic compression on text. ByteMe uses the key insight that a small subset of English words make up the vast majority of words used. With this knowledge, we created a variable-length encoding and lookup table compression that maps extremely common words (the top 64) to one byte and maps common words (the next 16,384) to two bytes. As it turns out, the most common 1000 English words make up 75% of words used. This lets us compress a large portion of the text. In addition to just frequency, the length of a word factors into its placement in our maps. Thus, a very long word can rank higher and have a greater chance of ending up in the lookup table.

Words that are not in our most common caches are left uncompressed. One character words are also left uncompressed, since they would not be made smaller. Additionally addition, substrings of words that are not common will be compressed. 


COMPRESSION_ANALYSIS

TITLE			GZIP	ByteMe
—————————————————————————————————————————
Pride and Prejudice 	 36%	  51%
The Scarlet Letter	 39% 	  54%
2BR02B			 44%	  56%

This trade-off in space more than makes up for itself with the advantage of homomorphic compression. Compressed strings can be compared, concatenated, and searched in the same asymptotic time as their uncompressed counterparts. Moreover, these operations do NOT require the data to be uncompressed and recompressed during manipulation. As a result, our implementation is both CPU and memory efficient. 
