## Example: Adjacency list {-}

This example shows how to declare and define a container whose elements are containers. An adjacency list is used to represent graphs in computer science. It is typically implemented as an array of lists. In our example, the list elements are integers. This is simple to do.

We first declare and define our list.

```C
#include <cagl/array.h>
#include <cagl/slist.h>

CAG_DEC_DEF_SLIST(islist, int);
```

Among the functions automatically generated when we declare any container type are *new_from_C* and *free_C* where *C* is the name of our container type. So *new_from_islist* and *free_islist* now exist.

These functions are essential when we declare and define our array of lists:

```C
CAG_DEC_DEF_ALL_ARRAY(adj_slist, islist, CAG_STRUCT_ALLOC_STYLE,
                      new_from_islist, free_islist, CAG_BYADR);
```

The parameters are as follows:

- *adj_slist* is the the name of the adjacency matrix type we're declaring and defining.

- *islist* is the name of the type of singly-linked list of integers we've already declared and defined.

- *CAG_STRUCT_ALLOC_STYLE* is a CAGL macro that is typically used when the elements of a container are a struct whose memory management must be handled by the higher-level container. In this case we want the singly linked lists to be memory managed by CAGL.

- *new_from_islist* is a CAGL function defined for our singly linked list of integers. Every CAGL container has a *new_from_[container]* function defined. It is analogous to a C++ copy constructor and is needed for when containers are elements of other containers.

- *free_islist* is needed to destroy lists and return them to the heap when our container is finished with them.

- *CAG_BYADR* tells CAGL that the *new_from_islist* and *free_islist* functions take their parameters by address.

While some of these parameters might seem complicated, they are always the same for CAGL containers whose elements are CAGL containers.

Here's a first cut at how we could populate the list:

```C
static void populate_adj_slist(adj_slist *m, int num)
{
	int i, j;
	for (i = 0; i < num; ++i) {
		islist l;
		if (new_islist(&l) == NULL) {
			fprintf(stderr, "Out of memory %d", __LINE__);
			exit(1);
		}
		for (j = 0; j < num; ++j) {
			if (prepend_islist(&l, j) == NULL) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
			}
		}
		if (appendp_adj_slist(m, &l) == NULL) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
		}
		free_islist(&l);
	}
}
```

The problem with this code is that we first create the whole list in *l*. Then when we call *appendp_adj_slist*, it recopies the entire list into the element created in the array using the *new_from_islist* function. This is wasteful. We can save half the time by first appending an empty list into the array. The *new_from_islist* function now has very little work to do because it copies from an empty list into a new list. We then prepend elements directly into the list element that has been created in the array.

The following code shows how to populate the singly linked lists of the array efficiently.

```C
/*
	Example of how to populate an adjacency list represented by a matrix
	of singly-linked lists.

    Parameters:

	- m: the adjacency list

	- num the number of entries to put in the list
*/

static void populate_adj_slist_efficient(adj_slist *m, int num)
{
	int i, j;
	islist l;
	islist *pl;

	new_islist(&l);
	for (i = 0; i < num; ++i) {
		appendp_adj_slist(m, &l);
		/* Note we will populate the added list itself, not l.
		   We can populate l and then append it to the matrix,
		   but this would be inefficient and result in l's elements
		   being copied one-by-one into the element in the array.
		*/
		pl = back_adj_slist(m);
		for (j = 0; j < num; ++j)
			if (!prepend_islist(pl, j)) {
				fprintf(stderr, "Out of memory %d", __LINE__);
				exit(1);
			}
	}
	free_islist(&l);
}
```

For several examples of containers containing containers as elements, as well as a complicated example of a container within a container within a container, see *test_compound.c* in the *tests* sub-directory of the CAGL distribution.
