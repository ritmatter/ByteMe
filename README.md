ByteMe : a homomorphic compression API
======
Anish Athalye, Nikihl Buduma, Matt Ritter
Palantir Hackathon, 7/26/14

Current compression algorithms make text smaller, but the compression is not homomorphic.  This means that the compressed texts cannot be operated upon without being decompressed first. Recently, homomorphic encryption has been employed on data. With this technique, perations can still be performed on this encrypted data. Our team researched the concept of homomorphic compression, which would allow us to compress data and perform operations on that data without decompression. This field of study is still highly experimental.

To tackle this problem, we created ByteMe, an API that performs homomorphic compression. ByteMe uses the key insight that a small subset of English words make up the vast majority of words used. With this knowledge, we created a caching system that maps extremely common words (the top 64) to one byte and maps common words (the next 16,384) to two bytes. As it turns out, the most common 1000 English words make up 75% of words used. This lets us compress a large portion of the text.

Words that are not in our most common caches are left unencrypted. One character words are also left unencrypted, since they would not be made smaller. Additionally addition, substrings of words that are not common will be encrypted.

The end result of our compression reduced Moby Dick to 55% of its original size, compared with approximately 42% compression from Gzip. This trade-off in space more than makes up for itself with the advantage of homomorphic compression. Compressed strings can be compared, concatenated, and searched in the same asymptotic time as their uncompressed counterparts.
