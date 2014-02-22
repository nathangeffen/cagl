# Design principles

CAGL is designed with these principles in mind:

1. Correctness: Containers and their functions should do what they intend to do. The documentation must be consistent with the library.

1. Simplicity: CAGL containers must be easy to use. Functions must be consistently named across container types and also behave consistently.

1. Efficiency: Container functions should be fast and occupy minimal space. Speed is usually more important than space.

1. Practical: CAGL is intended to be useful. It is not intended to be a comprehensive implementation of algorithm theory, not even close. It is not intended to emulate the C++ STL. It is only intended to provide programmers with a set of often-used containers and algorithms.

CAGL containers and their supporting functions are intended to cover common use cases, to reduce the drudgery and repetition of programming, as well as to relieve programmers from the hassle of coding more complicated data structures like hash tables and red-black trees, so that they can focus on their core work.

It is not the aim of CAGL to provide specialised containers. For example, hash tables are a vital part of chess programs. A chess programmer might use the CAGL to define a hash table for storing chess positions, but as the program develops and every last ounce of speed and efficiency must be squeezed out of the hash table, the CAGL hash table would likely have to be replaced with a specialised one. On the other hand, the same program might also make use of CAGL arrays, trees, lists and other hash tables that do not need to be specialised and whose performance is adequate.

While containers other than arrays, lists, trees and hash tables are envisaged for CAGL, feature creep will be avoided. CAGL is meant to bring some of the *niceness* of the STL to C users, but it is not intended to be as comprehensive or flexible as the STL, nor as obscure as the STL can sometimes be.

See the file TODO.md for the features being considered for CAGL.
