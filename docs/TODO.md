# CAGL To do List

## Bug fixes and testing

- Recovery from malloc errors has not been tested. In some cases there would
  definitely be leaks.

- Test under Microsoft and Intel C compilers.

- Test with non-gnu C preprocessors

- Test on embedded systems.

## Improve code

- The generic stable sort must be rewritten. It's ugly. (The stable sort for singly linked lists is ok.)

- Although the library interface is consistent (or mostly so), there are inconsistencies throughout the innards of the library, reflecting different experiments on how to code the macros. There needs to be some discussion on which style is best and then all non-conforming code must be brought in line with this standard.

- Analyse the return values for the function blueprints. Identify which ones need to be changed to improve the interface. Then change them.

- Analyse the

## Installation

- Automated installations needed. Autotools? Not sure. Unnecessarily complicated
  perhaps. For this project, which must always remain simple, manually
  maintained Makefile should be sufficient for widest portability.

- Debian etc.

- Automated install on Microsoft and Apple OSes.

## Enhancements

- Improve safety for development by putting in many more assert statements.

- Go through the C++ STL and identify which functions should be implemented in CAGL. Then implement them. But CAGL must not be too comprehensive. The primary aim is for it to be practical and useful, not an ode to the purity of the theory of algorithms.

- Containers to consider (but possibly reject, because we don't want feature creep):

    - Priority Queues

    - Stacks

    - Multi-dimensional arrays

    - Graphs

        - Adjacency lists
        - Adjacency matrices

- Maybe, a CAGL string/utf8 type declared as an array.

- Declare CAGL functions as static so that they are not global. I've thought
  about possible ways to do this but have not come up with a satisfactory
  solution.

- Perhaps implement a version of CAGL with algorithms, such as *sort_C* that are multithreaded. This will require extensive thought and discussion before proceeding.

## Documentation

- Examples for every function. Improve the reference section.

- Remove numerous inconsistencies of language and numbering throughout documentation.

- Proofread the documentation closely.

- Simplify documentation for users.
