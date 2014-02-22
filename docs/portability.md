# Portability

CAGL is compatible with ANSI C 1989 compliant compilers, as well as subsequent standard versions of C. The 1989 C definition specifies an archaic limitation that external variable names that share the first six characters do not have to be treated as unique. This is not a limitation for any modern C compiler and since adhering to it would have rendered CAGL impractical, it is ignored, as it is by most useful C libraries.

The test suite currently compiles without warnings and executes 100% successfully using GNU C and Clang with these options:

*-Wall -pedantic -Wstrict-prototypes -Wextra*

It is intended that the test suite of future versions of CAGL will also be compiled, without generating warnings, and executed with Microsoft's and Intel's C compilers.

Currently CAGL is tested under GNU/Linux. Future tests should also be carried out under Windows and OS X.
