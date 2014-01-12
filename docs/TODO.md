# CAGL To do List

## Bug fixes and testing

- Recovery from malloc errors has not been tested. In some cases there would
  definitely be leaks.

- Test under Microsoft and Intel C compilers.

- Test with non-gnu C preprocessors

- Test on embedded systems.

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
