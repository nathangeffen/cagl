# CAGL To do List

## Bug fixes and testing

- Recovery from malloc errors has not been tested. In some cases there would
  definitely be leaks.

- Test under Microsoft and Intel C compilers.

- Test with non-gnu C preprocessors

- Test on embedded systems.

## Ugly code

- The generic stable sort must be rewritten. It's ugly.

- Although the library interface is consistent (or mostly so), there are inconsistencies throughout the innards of the library, reflecting different experiments on how to code the macros. There needs to be some discussion on which style is best and then all non-conforming code must be brought in line with this standard.

## Installation

- Automated installations needed. Autotools? Not sure. Unnecessarily complicated
  perhaps. For this project, which must always remain simple, manually
  maintained Makefile should be sufficient for widest portability.

- Debian etc.

- Automated install on Microsoft and Apple OSes.

## Enhancements

- Safe mode (define CAG_SAFER during development to catch bugs and for safer use
  of the library

- Priority Queues

- Stacks

- Multi-dimensional arrays

- Graphs
    - Adjacency lists
    - Adjacency matrices

- Declare CAGL functions as static so that they are not global. I've thought
  about this a lot but have not come up with a satisfactory solution.

## Documentation

- Examples of every function.
