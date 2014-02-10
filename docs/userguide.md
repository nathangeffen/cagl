# CAGL's main macros and functions

Every CAGL macro and function is prefixed *CAG_* (for macros) or *cag_* (for functions). Please consider this the namespace for CAGL and do not prefix your own code with *CAG_*.

The CAGL container include files are:

cagl/array.h
  ~ For declaring and defining automatically sized arrays similar to a C++ STL *vector*.

cagl/dlist.h
  ~ For declaring and defining doubly-linked lists similar to a C++ STL *list*.

cagl/slist.h
  ~ For declaring and defining singly-linked lists similar to a C++11 STL *forward_list*.

cagl/tree.h
  ~ For declaring and defining balanced binary trees (implemented as red-black trees) similar to a C++ STL *map*.

cagl/hash.h
  ~ For declaring and defining hash tables similar to a C++11 *unordered_map*.

The simplest way to declare and define a container whose elements are a particular type is to do this:

```
CAG_DEC_DEF_[ARRAY/DLIST/SLIST/HASH/TREE](name of container type, element type, ... other parameters ...);
```

This declares a doubly-linked list of doubles type and forward and reverse iterators for it. It also defines its supporting functions:

```C
CAG_DEC_DEF_DLIST(dbl_list, double);
```

The first parameter is the name of the container type. You can give this any legal C type name. The second parameter is an already existing C pointer, primitive type or struct (or *typedef*ed version thereof). The *DEC* stands for *declare* and the *DEF* stands for *define*. In C you should declare functions you use in each module that uses it, but you may only define the functions once. Therefore if you need a list of doubles two C files, say file1.c and file2.c, you would declare the type in, say, file1.h as follows:

```C
CAG_DEC_DLIST(dbl_list, double);
```

You would need to include file1.h in both file1.c and file2. Then in one C source code file only, say file1.c, you would define the functions like this:

```C
CAG_DEF_DLIST(dbl_list, double);
```

Arrays and lists may be ordered. Balanced binary trees have to be ordered. To declare an ordered CAGL container, you need to pass the definition macro a comparison function. For example, this declares a binary tree of integers:

```C
CAG_DEC_DEF_CMP_TREE(int_tree, int, CAG_CMP_PRIMITIVE);
```

*CAG_CMP_PRIMITIVE* is a CAGL macro that correctly compares the C primitive types. You could also do it this way:

```C
CAG_DEC_CMP_TREE(int_tree, int);
CAG_DEF_CMP_TREE(int_tree, int, CAG_CMP_PRIMITIVE);
```

For C++ STL users, the *int_tree* type is similar to a std::map<int>.

The *CMP* part of the macro indicates that this is a container that requires a comparison function (or comparison macro such as CAG_CMP_PRIMITIVE), usually to maintain the container in a particular order. However, not all all containers that require a comparison function are ordered.


Hash tables require a comparison function but are not ordered. Hash table container types require a slightly more complicated macro to define them. This declares and defines a hash table of C strings type called *str_hash*.

```C
CAG_DEC_DEF_CMP_HASH(str_hash, char *, strcmp, CAG_STRING_HASH, strlen);
```

The first parameter is the name of the container type. The second parameter is the C string type, _char *_, which tells CAGL that the elements of this container are strings. The third parameter is the comparison function, which in this example is the C standard library function *strcmp*. The fourth parameter is the default CAGL string hash function, *CAG_STRING_HASH*. The final parameter is a function for calculating the length of each element, the C standard library function *strcmp*.

The following code snippet is an example of how to populate the hash table:

```C
static void populate_str_hash(str_hash *h)
{
	char *k0 = "k0";
	char *k1 = "k1";
	char *k2 = "k2";
	char *k3 = "k3";
	char *k4 = "k4";

	insert_str_hash(h, k0);
	insert_str_hash(h, k1);
	insert_str_hash(h, k2);
	insert_str_hash(h, k3);
	insert_str_hash(h, k4);
}
```

For primitive types and many types of structs, the default *CAG_DEC_DEF* or *CAG_DEF* macros are often sufficient. However, the string hash table type we declared has a limitation. The CAGL container manages the memory of the container itself, but it does no memory management of the elements of the container.

In the *str_hash* container k0 through k5 are assigned constants. If instead k0 to k5 were declared as string arrays of length 3 whose values were copied into them using strcpy, their values would be undefined as soon soon as *populate_str_hash* finished executing, even if the hash table remained alive. Attempting to access the elements from the function that called them would likely result in getting garbage values or a crash.

CAGL offers a way for its containers to manage the memory of its elements. The C++ STL containers manage the memory of their elements through the constructors and destructors of those elements. C does not have constructors and destructors, so CAGL needs to be told how to allocate and de-allocate its elements. CAGL macros that begin *CAG_DEF_ALL_* allow you to define the allocation and deallocation functions or macros for your container's elements.

Here is an example of a tree that manages the memory for C strings:

```C
CAG_DEC_TREE(string_tree, char *)
CAG_DEF_ALL_CMP_TREE(string_tree, char *, strcmp, CAG_BYVAL,
                     CAG_SIMPLE_ALLOC_STYLE, cag_strdup, free);
```

The parameters of the call to CAG_DEF_ALL_CMP_TREE are as follows:

1. *string_tree* is the name of the container type.

1. _char *_ is the C string type.

1. *strcmp* is the C standard function for comparing strings.

1. *CAG_BYVAL* is a CAGL macro which tells CAGL that the comparison function for this container takes its parameters by value and there is therefore no need to prefix the parameters with the & sign. See below for an alternative to this.

1. *CAG_SIMPLE_ALLOC_STYLE* is a CAGL macro for how the allocation function should be invoked. You should almost never need to define your own allocation style macro or function, but there are alternative styles which are discussed elsewhere.

1. *cag_strdup* is a function in the CAGL cag_common.c module that allocates a C string from the heap. Most C libraries provide a *strdup* function which you could also use, or you could write your own.

1. *free* is the standard C free function that will deallocate a string once it is no longer needed.

Now you can insert strings into a *string_tree* container without worrying about allocating or deallocating its memory. CAGL will handle this for you. When the container is freed, all the elements will be freed too.

# Examples

## Euclidean points {-}

There are numerous examples of how to use the CAGL in the test suite programs. Here is a simple example with Euclidean points.

Let's generate a set of points, store them in a list and then find the two closest points. To demonstrate how you can write modular code with CAGL we're going to have three files:

- eg_points1.c which will contain our main function as well as functions to populate our points.
- eg_points2.c which will contain our functions that operate on points and list of points.
- eg_points.h which will contain our declarations common to eg_points1.c and eg_points2.c

In eg_points.h we'll declare our struct as follows:

```C
struct point {
	double x;
	double y;
};
```

We also need a list to store the points:

```C
#include <cagl/dlist.h>
CAG_DEC_DLIST(points, struct point);
```

We've declared a type called *points* which is a list of *struct point*. We've also declared prototypes for functions that operate *points*. In *eg_points2.c* we need to define our functions that operate on the *points* type:

```C
CAG_DEF_DLIST(points, struct point);
```

We'll implement two ways of calculating the shortest distance. One will use indices and the other will use iterators.

Here are the prototypes for our two functions:

```C
double shortest_distance_using_index(points *);
double shortest_distance_using_iterators(points *);
```

We'll implement two ways of populating our points. One will populate a list with points in an ordered way. Here is how we could do it:


```C
void populate_ordered(points *a, int numpoints, double x_start,
		     double x_step, double gradient)
{
	int i;
	struct point p;

	for (i = 0; i < numpoints; ++i) {
		p.x = x_start + i * x_step;
		p.y = p.x * gradient;
		append_points(a, p);
	}
}
```

This function takes a list by address as its first parameter. It will populate it with *numpoints* points. The first *x* point will be *x_start* and each *y* point will be the corresponding *x* point multiplied by gradient.

We use the CAGL generated *append_points* function to put each new point at the back of the list. For doubly-linked lists and arrays, the efficiency of the *append* function is constant.

For minor or personal-use programs, the above is sufficient, but for a production ready system, there is a problem. The *append_points* function can fail. It might not be able to allocate memory for the new point. If the append was successful, *append_points* will return a pointer to the newly appended point, but if it fails it will return NULL.

C doesn't have exceptions, so we have to handle error checking at the call to *append_points*. If *append_points* fails, we want to return an error from populate_ordered, perhaps 1 for success and 0 for a failure.

Here's one way to do it:

```C
int populate_ordered(points *a, int numpoints, double x_start,
		     double x_step, double gradient)
{
	int i;
	struct point p;

	for (i = 0; i < numpoints; ++i) {
		p.x = x_start + i * x_step;
		p.y = p.x * gradient;
		if (append_points(a, p) == 0) {
			return CAG_FALSE;
		}
	}
	return CAG_TRUE;
}
```

*CAG_TRUE* and *CAG_FALSE* are simply macros equal to 1 and 0 respectively. You don't have to use them. (Why haven't we used the C99 bool type? We could, but unless otherwise specified all the examples here are C89 compatible.)

Now here's code in our *main()* function that declares a list of points and calls the *populate_ordered* function. It also prints out all the points.

```C
	points ordered;
	it_points it;

	if (new_points(&ordered) == CAG_FALSE) {
		fprintf(stderr, "Error initialising array.\n");
		exit(1);
	}
	if (populate_ordered(&ordered, 5, 0.0, 10.0, 2.0) == CAG_FALSE)  {
		fprintf(stderr, "Error populating array.\n");
		exit(1);
	}
	printf("Ordered points:\n");
	for (it = beg_points(&ordered); it != end_points(&ordered);
	     it = next_points(it))
		printf("(%0.1f, %0.1f)\n", it->value.x, it->value.y);
	free_points(&ordered);
```

We checked that the call to *new_points* didn't produce an error. All CAGL functions that can produce an error return a pointer. If the pointer is NULL, an error has occurred.

We also used an iterator *it* to step through the list. And we called *free_points* to return any memory we allocated to the heap.

Some C style guides explicitly recommend against hiding pointers in *typedef*s like CAGL does with iterators. If you're following such a style guide, instead of declaring *it* like this:

```C
it_points it;
```

you can instead do this

```C
struct iterator_points *it;
```

In all our examples and in the actual CAGL code, we use the *typedef* version. The two declarations are effectively identical. Which you chose to use is up to you.

Here's a function to populate the list with randomly generated points. But notice that here we call *appendp_points* instead of *append_points*. In CAGL, if the verb of the function ends with a *p*, then the function takes its element parameter by address -- as a pointer --  not by value. In this example it makes little difference, but if the elements of our container are large then the *appendp* version will be much more efficient because it won't copy the entire structure when calling the append function.

```C
int populate_random(points *a, int numpoints, int maxval)
{
	int i;
	struct point p;

	for (i = 0; i < numpoints; ++i) {
		/* Not a good way to generate random values but
		   sufficient for our purposes.
		 */
		p.x = rand() % maxval;
		p.y = rand() % maxval;
		if (appendp_points(a, &p) == 0) {
			return CAG_FALSE;
		}
	}
	return CAG_TRUE;
}
```

Now for the corresponding code in *main()*. Here too we do something different.

Instead of iterating using an explicitly defined for loop, we use the *CAG_FOR_ALL* macro. It takes four parameters: the name of the container type, *points*, the container variable, *random*, which must be a pointer hence we pass it by address, an iterator over the container, *it*, and code to execute on each iteration of the loop. Here we only execute one statement, a *printf*. But if we had multiple statements, we would have to enclose the code in curly brackets.

Note that *it* is iterated by the macro over all the elements in our container variable called *random*. Using a macro to abstract away a for loop is not every C programmer's cup of tea. In which case, feel free to ignore the idiom below and use the method above.

```C
	points random;
	it_points it;

	if (new_points(&random) == CAG_FALSE) {
		fprintf(stderr, "Error initialising array.\n");
		exit(1);
	}
	if (populate_random(&random, 5, 50) == CAG_FALSE)  {
		fprintf(stderr, "Error populating array.\n");
		exit(1);
	}
	printf("Random points:\n");
	CAG_FOR_ALL(points, &random, it,
		        printf("(%0.1f, %0.1f)\n", it->value.x, it->value.y));
	free_points(&random);
```

Now in file2.c we will implement our two shortest distance functions. They use a brute force algorithm which is simple but inefficient. For our purposes, the brute force method is sufficient. The algorithm steps through each point in the list except the last and calculates its Euclidean distance to every point in front of it, updating the smallest distance found if necessary.

Here's our implementation X:

```C
double shortest_distance_using_index(points *a)
{
	int i, j;
	int num_points;
	double min_dist = DBL_MAX;

	num_points = distance_all_points(a);
	for (i = 0; i < num_points - 1; ++i) {
		struct point p1 = index_points(a, i)->value;
		for (j = i + 1; j < num_points; ++j) {
			struct point p2 = index_points(a, j)->value;
			double d =
				(p1.x - p2.x) * (p1.x - p2.x) +
				(p1.y - p2.y) * (p1.y - p2.y);
			if (d < min_dist)
				min_dist = d;
		}
	}
	return sqrt(min_dist);
}
```

This method uses indices, *i* and *j*, to step through the list. It uses two CAGL functions, *distance_all_points*, which returns the number of elements in the list, and *index_points_points*, which returns an iterator to the *i*'th or *j*'th element in the container.

Here's another way of doing it using iterators:

```C

double shortest_distance_using_iterators(points *a)
{
	it_points i, j;
	double min_dist = DBL_MAX;

	for (i = beg_points(a); i != prev_points(end_points(a));
	     i = next_points(i)) {
		struct point p1 = i->value;
		for (j = next_points(i); j != end_points(a);
		     j = next_points(j)) {
			struct point p2 = j->value;
			double d =
				(p1.x - p2.x) * (p1.x - p2.x) +
				(p1.y - p2.y) * (p1.y - p2.y);
			if (d < min_dist)
				min_dist = d;
		}
	}
	return sqrt(min_dist);
}

```

Every CAGL container has functions called *beg_C*, *begin_C* (the same as *beg_C*), *next_C*, *distance_C*, *distance_all_C* and *end_C*, where *C* is the name of the container type.

So which is the better method to use, iterators or indices? For lists, *distance_all* is an O(n) operation and only used in the index version. The same goes for *index*. Therefore, for lists, the iterator version is more efficient. But for arrays these are both constant-time operations and there is little to separate the efficiency of the two implementations.

Here is our complete *main()* function which uses both of these shortest distance functions:

```C
int main(void)
{
	points ordered, random;
	it_points it;

	if (new_points(&ordered) == CAG_FALSE) {
		fprintf(stderr, "Error initialising array.\n");
		exit(1);
	}
	if (populate_ordered(&ordered, 5, 0.0, 10.0, 2.0) == CAG_FALSE)  {
		fprintf(stderr, "Error populating array.\n");
		exit(1);
	}
	printf("Ordered points:\n");
	for (it = beg_points(&ordered); it != end_points(&ordered);
	     it = next_points(it))
		printf("(%0.1f, %0.1f)\n", it->value.x, it->value.y);
	printf("Shortest distance using index is: %.1f\n",
	       shortest_distance_using_index(&ordered));
	printf("Shortest distance using iterators is: %.1f\n",
	       shortest_distance_using_iterators(&ordered));
	free_points(&ordered);

	if (new_points(&random) == CAG_FALSE) {
		fprintf(stderr, "Error initialising array.\n");
		exit(1);
	}
	if (populate_random(&random, 5, 50) == CAG_FALSE)  {
		fprintf(stderr, "Error populating array.\n");
		exit(1);
	}
	printf("Random points:\n");
	CAG_FOR_ALL(points, &random, it,
		    printf("(%0.1f, %0.1f)\n", it->value.x, it->value.y));
	printf("Shortest distance using index is: %.1f\n",
	       shortest_distance_using_index(&random));
	printf("Shortest distance using iterators is: %.1f\n",
	       shortest_distance_using_iterators(&random));
	free_points(&random);

	return 0;
}
```

The complete code can be found in the examples directory of the CAGL repository.

And here's sample output:

    Ordered points:
    (0.0, 0.0)
    (10.0, 20.0)
    (20.0, 40.0)
    (30.0, 60.0)
    (40.0, 80.0)
    Shortest distance using index is: 22.4
    Shortest distance using iterators is: 22.4
    Random points:
    (33.0, 36.0)
    (27.0, 15.0)
    (43.0, 35.0)
    (36.0, 42.0)
    (49.0, 21.0)
    Shortest distance using index is: 6.7
    Shortest distance using iterators is: 6.7


## Deck of cards {-}

Let's use CAGL to manage a deck of cards which we'll store in an array. To keep things simple, we'll write all the code in one file this time. We'll also order our cards according to the [high-card by suit](http://en.wikipedia.org/wiki/High_card_by_suit) convention. To compare the order of two cards, first compare their rank. If the ranks are the same, compare their suits alphabetically. So clubs is lowest, followed by diamonds, hearts and spades.

We create a file called *eg_cards.c*. Here are the first few lines:

```C
#include <stdio.h>
#include <stdlib.h>
#include <cagl/array.h>

struct card {
	int rank;
	char suit;
};
```

We now want to declare a CAGL array type called *deck*, but we want to use CAGL to order the deck if necessary, so we need to use the *CAG_DEC_CMP_ARRAY* macro. The *CMP* tells CAGL that this array has a comparison function.

As a first attempt we could write this comparison function:

```C
int cmp_cards(const struct card c1, const struct card c2)
{
	int r = c1.rank - c2.rank;
	return r == 0 ? c1.suit - c2.suit : r;
}
```

And then declare our deck like this:

```C
CAG_DEC_DEF_CMP_ARRAY(deck, struct card, cmp_cards);
```

If we were splitting this code over multiple files, we would use two macros, *CAG_DEC_CMP_ARRAY* and CAG_DEF_CMP_ARRAY

Note that our comparison function, *cmp_cards*, takes its parameters by value. For a small struct like *card* this is not serious. But it would be inefficient if we were comparing two large *struct*s. We would prefer to write our comparison function like this:

```C
int cmp_cards(const struct card *c1, const struct card *c2)
{
	int r = c1->rank - c2->rank;
	return r == 0 ? c1->suit - c2->suit : r;
}
```

CAGL supports this possibility but we have to use a slightly more complicated macro: *CAG_DEC_DEF_ALL_CMP_ARRAY*. The *ALL* means there are no default parameters to the macro.

The correct way to declare and define the *deck* array using the pass-by-address version of *cmp_cards* is:

```C
CAG_DEC_DEF_ALL_CMP_ARRAY(deck, struct card, cmp_card, CAG_BYADR,
			  CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
			  CAG_NO_FREE_FUNC);
```

That might look a bit intimidating, but usually for *struct*s the fourth parameter onwards are either the same as above, or one variation of it.

Incidentally, you could declare this in two macros as follows:

```C
CAG_DEC_CMP_ARRAY(deck, struct card);
CAG_DEF_ALL_CMP_ARRAY(deck, struct card, cmp_cards, CAG_BYADR,
			  CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
			  CAG_NO_FREE_FUNC);
```
It's identical except that you would use the two-macro version if *deck* was needed in multiple modules.

Here are what the parameters from the fourth one onwards mean:

- *CAG_BYADR* is a CAGL supplied macro telling CAGL that the comparison function takes its parameter by address. Actually *CAG_BYADR* is simply equal to the ampersand *&*, and each time CAGL calls the *cmp_cards* it will now put an ampersand before the parameters. The alternative is *CAG_BYVAL* which tells CAGL the comparison function is by value.

- *CAG_NO_ALLOC_STYLE* is a macro which tells CAGL not to do any special heap allocation for the elements of this array.

- *CAG_NO_ALLOC_FUNC* is a macro used by *CAG_NO_ALLOC_STYLE*.

- *CAG_NO_FREE_FUNC* is a macro telling CAGL not to deallocate (free) the elements of the array when its finished with them.

(Our next example looks at a variation of this where we do need the elements to be allocated and freed from the heap.)

Moving on, we want to populate our deck of cards so we write this function, which intentionally does not load the cards in order.

```C
int populate_deck(deck *d)
{
	int rank;
	struct card crd;

	for (rank = 0; rank < 13; ++rank) {
		crd.rank = rank;
		crd.suit = 'C';
        /* You might prefer to write the next line as:
		   if (appendp_deck(d, &crd) == NULL) return CAG_FALSE;
		*/
		if (!appendp_deck(d, &crd)) return CAG_FALSE;
		crd.suit = 'D';
		if (!appendp_deck(d, &crd)) return CAG_FALSE;
		crd.suit = 'H';
		if (!appendp_deck(d, &crd)) return CAG_FALSE;
		crd.suit = 'S';
		if (!appendp_deck(d, &crd)) return CAG_FALSE;
	}
	return CAG_TRUE;
}
```

We also need to pretty-print our cards:

```C
char *rank_names[] =
{
	"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
};

void print_card(const struct card *c)
{
	printf("%s%c", rank_names[c->rank], c->suit);
}

void print_deck(const deck *d)
{
	it_deck it;

	CAG_FOR_ALL(deck, d, it,
		    {
			    print_card(&it->value);
			    printf(" ");
		    });
	printf("\n");
}
```

Here's the first version of our *main()* function:

```C
int main(void)
{
	deck d;
	if (!new_deck(&d)) {
		fprintf(stderr, "Error creating deck\n");
		exit(1);
	};
	if (!populate_deck(&d)) {
		fprintf(stderr, "Error populating deck\n");
		exit(1);
	}
	print_deck(&d);
	free_deck(&d);
	return 0;
}
```

The output is:

    2S 2H 2D 2C 3S 3H 3D 3C 4S 4H 4D 4C 5S 5H 5D 5C 6S 6H 6D 6C 7S 7H 7D 7C 8S 8H 8D 8C 9S 9H 9D 9C 10S 10H 10D 10C JS JH JD JC QS QH QD QC KS KH KD KC AS AH AD AC

That's not the correct order of a pack though. We can use the CAGL generated sort function to fix that:

```C
sort_deck(beg_deck(&d), end_deck(&d));
```

The *sort_[container]* function takes two iterators over the same container as parameters and sorts the elements in that range. Many CAGL functions work this way. But operating on an entire container is so common, that the CAGL also often provides a short-hand function so we don't have to type *beg_deck(&d), end_deck(&d)*. This does the trick:

```C
sort_all_deck(&d);
```

We can sort the array from highest to lowest card instead of from lowest to highest. To do this we use reverse iterators. For containers that are bidirectional (arrays, doubly-linked lists and trees), many analogous functions that operate on reverse iterators are provided. These are prefixed *r*.

For example, for sorting, *rsort_deck* and *rsort_all_deck* are provided. This is how *rsort_deck* is invoked:

```C
rsort_deck(rbeg_deck(&d), rend_deck(&d));
```

Note the use of reverse versions of *beg_deck* and *end_deck*. Alternately we can be more succinct:

```C
rsort_all_deck(&d);
```

We can also shuffle our deck with a CAGL provided function:

```C
random_shuffle_deck(beg_deck(&d), end_deck(&d));
```

Or more succinctly:

```C
random_shuffle_all_deck(&d);
```

Here's our completed, admittedly rather contrived, *main()* function:

```C
int main(void)
{
	deck d;
	if (!new_deck(&d)) {
		fprintf(stderr, "Error creating deck\n");
		exit(1);
	};
	if (!populate_deck(&d)) {
		fprintf(stderr, "Error populating deck\n");
		exit(1);
	}
	printf("After populating the deck\n");
	print_deck(&d);
	sort_all_deck(&d);
	printf("After sorting the deck\n");
	print_deck(&d);
	rsort_all_deck(&d);
	printf("After reverse sorting the deck\n");
	print_deck(&d);
	random_shuffle_all_deck(&d);
	printf("After shuffling the deck\n");
	print_deck(&d);
	free_deck(&d);
	return 0;
}
```

The output might look like this:

    After populating the deck
    2S 2H 2D 2C 3S 3H 3D 3C 4S 4H 4D 4C 5S 5H 5D 5C 6S 6H 6D 6C 7S 7H 7D 7C 8S 8H 8D 8C 9S 9H 9D 9C 10S 10H 10D 10C JS JH JD JC QS QH QD QC KS KH KD KC AS AH AD AC
    After sorting the deck
    2C 2D 2H 2S 3C 3D 3H 3S 4C 4D 4H 4S 5C 5D 5H 5S 6C 6D 6H 6S 7C 7D 7H 7S 8C 8D 8H 8S 9C 9D 9H 9S 10C 10D 10H 10S JC JD JH JS QC QD QH QS KC KD KH KS AC AD AH AS
    After reverse sorting the deck
    AS AH AD AC KS KH KD KC QS QH QD QC JS JH JD JC 10S 10H 10D 10C 9S 9H 9D 9C 8S 8H 8D 8C 7S 7H 7D 7C 6S 6H 6D 6C 5S 5H 5D 5C 4S 4H 4D 4C 3S 3H 3D 3C 2S 2H 2D 2C
    After shuffling the deck
    AD 6D KC AH 4H 4S 10H 9C 5S 2C 3S 7C 7H JC 10C 3D 6C 6H 3H 10D QH QC 8S KD JD 9S 2S 6S 4D 7D AC JS 5D 3C KS QS AS 4C KH 8C 8H 2H 5C 9D 2D 7S 9H 8D JH 5H 10S QD

## Dictionary {-}

In this example, we want to create a dictionary for storing words and their meanings. In other words we want a balanced binary tree whose elements are this:

```C
struct entry {
	char *word;
	char *definition;
};
```

At the top of the program, *eg_dictionary.c*, we need some include files:

```C
#include <stdio.h>
#include <stdlib.h>

#include <cagl/tree.h>
```

We also need a comparison function so that our dictionary can be ordered.

```C
int cmp_entry(const struct entry *e1, const struct entry *e2)
{
	return strcmp(e1->word, e2->word);
}
```

We are lazy (or very cautious) and want the CAGL to manage the memory of our dictionary entries. But the *entry struct* members are pointers whose memory needs to be managed. In C++ you would do this by writing a constructor and destructor for *entry* and a *map<entry>* would manage the allocation and deletion of its members by calling these implicitly whenever you added or erased an element in the map. Although C doesn't have constructors or destructors, you can pass two functions to the CAGL definition which effectively serve the same purpose. Actually a copy constructor is the more appropriate analogy. When we insert an entry into the tree we will copy it from an existing *struct entry*.

Here is a function to allocate (or copy-construct if you like) a new entry:

```C
int alloc_entry(struct entry *to, const struct entry *from)
{
	to->word = cag_strdup(from->word);
	if (!to->word)
		return CAG_FALSE;
	to->definition = cag_strdup(from->definition);
	if (!to->definition) {
		free(to->word);
		return CAG_FALSE;
	}
	return CAG_TRUE;
}
```

Note the use of the *cag_strdup* function. You could easily write your own one or, if you use GNU/Linux, use the *strdup* function. (But strdup, despite its ubiquity is not part of the standard ISO C library.)

We also need to free entries once they're erased or no longer used:

```C
void free_entry(struct entry *e)
{
	free(e->word);
	free(e->definition);
}
```

Now we can create our CAGL dictionary type:

```C
CAG_DEC_DEF_ALL_CMP_TREE(dictionary, struct entry, cmp_entry, CAG_BYADR,
			             CAG_STRUCT_ALLOC_STYLE, alloc_entry, free_entry);
```

Here's what the fourth parameter onwards mean:

- *CAG_BYADR*: means our comparison function, *cmp_entry* in this case, takes its parameters by address.
- *CAG_STRUCT_ALLOC_STYLE*: is a CAGL macro that allocates the memory for *struct*s. Since *entry* is a struct and we want CAGL to manage its memory, this is the correct allocation style parameter to use.
- *alloc_entry*: is our function for allocating the memory of an entry. It will be called by the code in *CAG_STRUCT_ALLOC_STYLE*.
- *free_entry*: is our function to return an entry's memory to the heap.

Again, we could have declared and defined the dictionary in two steps like this instead with the same results:

```C
CAG_DEC_CMP_TREE(dictionary, struct entry);
CAG_DEF_ALL_CMP_TREE(dictionary, struct entry, cmp_entry, CAG_BYADR,
                     CAG_STRUCT_ALLOC_STYLE, alloc_entry, free_entry);
```

The above method what we'd use if *dictionary* was needed across multiple modules.

However, if we'd declared our dictionary like this:

```C
CAG_DEC_DEF_CMP_TREE(dictionary, struct entry, cmp_entry)
```

Then we'd have to rewrite our comparison function to accept its parameters by value and we'd have to manage the memory of all our entries.

And if we did it like this, specifying no allocation or free functions:

```C
CAG_DEC_DEF_ALL_CMP_ARRAY(deck, struct card, cmp_cards, CAG_BYADR,
                          CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
			              CAG_NO_FREE_FUNC);
```

Then we would not have to rewrite our comparison function, but we would have to manage the memory of each entry. By now you should be understanding the differences between the container declaration and definition macros as well as the parameters they accept.

Here's our code to populate the dictionary:

```C
int populate_dictionary(dictionary *d)
{
	struct entry e;

	e.word = "lion";
	e.definition = "Dangerous, lazy and lovable wild animal";
	if (!insertp_dictionary(d, &e)) return 0;
	e.word = "cat";
	e.definition = "Fat, lazy and lovable domestic pet";
	if (!insertp_dictionary(d, &e)) return 0;
	e.word = "springbok";
	e.definition = "type of lion food";
	if (!insertp_dictionary(d, &e)) return 0;
	e.word = "mouse";
	e.definition = "type of cat food";
	if (!insertp_dictionary(d, &e)) return 0;
	e.word = "giraffe";
	e.definition = "strange animal with absurdly long neck";
	if (!insertp_dictionary(d, &e)) return 0;
	return 1;
}
```

And here's our *main()* function, which populates the dictionary and then prints it out.

```C
int main(void)
{
	dictionary d;
	it_dictionary it;

	if (!new_dictionary(&d)) {
		fprintf(stderr, "Error initializing dictionary.\n");
		exit(1);
	};
	if (!populate_dictionary(&d)) {
		fprintf(stderr, "Error populating dictionary.\n");
		exit(1);
	}
	CAG_FOR_ALL(dictionary, &d, it,
		    printf("%s: %s\n", it->value.word, it->value.definition));
	free_dictionary(&d);
	return 0;
}
```

This is the output:

    cat: Fat, lazy and lovable domestic pet
    giraffe: strange animal with absurdly long neck
    lion: Dangerous, lazy and lovable wild animal
    mouse: type of cat food
    springbok: type of lion food

Note that it's in alphabetical order even though we inserted the entries arbitrarily. The CAGL binary tree container modification functions maintain entries in alphabetical order and unless you intentionally invalidate the order of the tree, it will remain ordered. This is in contrast to arrays and lists. It's valid to scramble the order of any CAGL array or list, but the tree container is always ordered.

This example is ridiculously contrived because in practice you'd typically get the words and definitions from standard input. So let's rewrite our *populate_dictionary* function to do just that:

```C
int populate_dictionary(dictionary *d)
{
	char word[20];
	char definition[200];
	struct entry e;

	while(!feof(stdin) && strcmp(fgets(word, 20, stdin), "\n")) {
		word[strlen(word) - 1] = '\0'; /* remove newline */
		fgets(definition, 200, stdin);
		definition[strlen(definition) - 1] = '\0'; /* remove newline */
		e.word = word;
		e.definition = definition;
		if ((!insertp_dictionary(d, &e))) return CAG_FALSE;
	}
	return CAG_TRUE;
}
```

Running the program through a memory tester like *Valgrind* will show that there are no leaks.

What if you want to manually allocate your own container elements, but when the container is freed, or when elements are erased, you want CAGL to handle that? Easily done. The code below is inelegant but it'll do the trick:

```C
CAG_DEC_DEF_ALL_CMP_TREE(dictionary, struct entry, cmp_entry, CAG_BYADR,
			 CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, free_entry);

int populate_dictionary(dictionary *d)
{
	char *word;
	char *definition;
	struct entry e;

	word = malloc(sizeof(char) * 20);
	if (!word) return CAG_FALSE;
	while(!feof(stdin) && strcmp(fgets(word, 20, stdin), "\n")) {
		word[strlen(word) - 1] = '\0'; /* remove newline */
		definition = malloc(sizeof(char) * 200);
		if (!definition) return CAG_FALSE;
		fgets(definition, 200, stdin);
		definition[strlen(definition) - 1] = '\0'; /* remove newline */
		e.word = word;
		e.definition = definition;
		if ((!insertp_dictionary(d, &e))) return CAG_FALSE;
		word = malloc(sizeof(char) * 20);
		if (!word) return CAG_FALSE;
	}
	free(word); /* free the last unused word */
	return CAG_TRUE;
}
```

Because C strings are often needed in containers, and because hash tables and binary trees frequently store dictionaries, CAGL makes it easier by supplying macros that are abbreviations for the declarations and definitions we've used above. Here's a quick description of each of them. They all maintain the memory of their elements, similarly to the way C++ STL containers manage the memory of strings.

CAG_DEC_STR_HASH(string_hash)
  ~ Declares a hash table called *string_hash* whose elements are _char *_.
CAG_DEF_STR_HASH(string_hash)
  ~ Defines a hash table called *string_hash* whose elements are _char *_.
CAG_DEC_DEF_STR_HASH(string_hash)
  ~ Combines the above two macros in one.
- CAG_DEC_STR_TREE(string_tree), CAG_DEF_STR_TREE(string_tree) and CAG_DEC_DEF_STR_TREE(string_tree) which are the tree equivalents of the above hash table declarations and definitions.

There's only one way to define a C string, _char *_. But there are many ways to define a struct that stores dictionaries, so you have to do a bit more work. First make sure you have declared a struct that holds a dictionary. This must be a struct that consists of two _char *_ elements.

For example:

struct dictionary {
	char *w;
	char *d;
};

Then we can do this:

- CAG_DEC_STR_STR_HASH(dict_hash, struct dictionary);
- CAG_DEF_STR_STR_HASH(dict_hash, struct dictionary);
- CAG_DEC_DEF_STR_STR_HASH(dict_hash, struct dictionary);

- CAG_DEC_STR_STR_TREE(dict_tree, struct dictionary );
- CAG_DEF_STR_STR_TREE(dict_tree, struct dictionary);
- CAG_DEC_DEF_STR_STR_TREE(dict_tree, struct dictionary);

## Adjacency list {-}

This example shows how to declare and define a container whose elements are containers. An adjacency list is used to represent graphs in computer science. It is typically implemented as an array of lists. In our example, the list elements are integers. This is simple to do.

We first declare and define our list.

```C
#include <cagl/array.h>
#include <cagl/slist.h>

CAG_DEC_DEF_SLIST(islist, int);
```



Then we declare and define our array of lists:

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

For several examples of containers containing containers as elements, as well as a complicated example of a container within a container within a container, see *test_compound.c* in the tests directory of the CAGL distribution.


# Understanding CAGL macro, struct, type and function names {#names}

CAGL is easier to use if you understand the naming conventions:

- CAGL macros are prefixed *CAG_*.

- CAGL functions (not generated ones) are prefixed *cag_*.

- If you look at the CAGL source code, you'll notice some macros are prefixed *CAG_P_*. These are macros for internal use (the *P* is for private). Don't call them and don't depend on them to be unchanged or even continuing to exist between CAGL versions. The same goes for functions prefixed *cag_p_*.

- This is obvious but for the sake of completion:

	- Macros with ARRAY in their names deal with array container types.

	- Macros with DLIST in their names deal with doubly-linked list container types.

	- Macros with HASH in their names deal with hash table container types.

	- Macros with SLIST in their names deal with singly-linked list container types.

	- Macros with TREE in their names deal with balanced binary tree container types.


- The container declaration and definition macros are the foundation of CAGL. Their naming conventions are used consistently across the different types of containers.

    - 	These declare the container and iterator struct, and function prototypes. Any module that uses a CAGL container must declare its structs and prototypes. \
	\
	CAG_DEC_ARRAY \
	CAG_DEC_DLIST  \
	CAG_DEC_SLIST

	- 	These declare the container and iterator struct, and function prototypes for containers that require a comparison function. \
	\
	CAG_DEC_CMP_ARRAY \
	CAG_DEC_CMP_DLIST \
	CAG_DEC_CMP_SLIST \
	CAG_DEC_CMP_TREE \
	CAG_DEC_CMP_HASH

	- These are identical to the corresponding CAG_DEC_CMP_ macros but for legibility purposes it is recommended that these be paired with the corresponding CAG_DEF_CMPP_ macros described below. \
	\
	CAG_DEC_CMPP_ARRAY \
	CAG_DEC_CMPP_DLIST \
	CAG_DEC_CMPP_SLIST \
	CAG_DEC_CMPP_TREE \
	CAG_DEC_CMPP_HASH

	- These define the functions for container types. These macros should only be used once per container type. They call corresponding CAG_DEF_ALL_  prefixed macros using default parameters and are intended to cover the most common cases. \
	\
	CAG_DEF_ARRAY \
	CAG_DEF_DLIST \
	CAG_DEF_SLIST

	- These define the functions for container types that require a comparison function. They call corresponding CAG_DEF_ALL_CMP_ prefixed macros using default parameters and are intended to cover the most common cases. The comparison function takes two elements by value as parameters. \
	\
	CAG_DEF_CMP_ARRAY \
	CAG_DEF_CMP_DLIST \
	CAG_DEF_CMP_SLIST \
	CAG_DEF_CMP_TREE \
	CAG_DEF_CMP_HASH

	- These are identical to the corresponding CAG_DEF_CMP_ macros except the comparison function takes its parameters by address. \
	\
	CAG_DEF_CMPP_ARRAY \
	CAG_DEF_CMPP_DLIST \
	CAG_DEF_CMPP_SLIST \
	CAG_DEF_CMPP_TREE \
	CAG_DEF_CMPP_HASH

	- These combine declarations and definitions in one call. \
	\
	CAG_DEC_DEF_ARRAY \
	CAG_DEC_DEF_DLIST \
	CAG_DEC_DEF_SLIST

	- These combine declarations and definitions in one call for container that require comparison functions that take two elements by value. \
	\
	CAG_DEC_DEF_CMP_ARRAY \
	CAG_DEC_DEF_CMP_DLIST \
	CAG_DEC_DEF_CMP_SLIST \
	CAG_DEC_DEF_CMP_HASH \
	CAG_DEC_DEF_CMP_TREE

	- These are identical to the corresponding CAG_DEC_DEF_CMP_ macros except that the comparison functions takes its two elements by address.\
	\
	CAG_DEC_DEF_CMPP_ARRAY \
	CAG_DEC_DEF_CMPP_DLIST \
	CAG_DEC_DEF_CMPP_SLIST \
	CAG_DEC_DEF_CMPP_HASH \
	CAG_DEC_DEF_CMPP_TREE \

	- These define the functions for container types. These macros should only be used once per container type. These expect more parameters than their like-named counterparts without *ALL* in them. \
	\
	CAG_DEF_ALL_ARRAY \
	CAG_DEF_ALL_DLIST \
	CAG_DEF_ALL_SLIST

	- These definition macros are the most flexible and have the most number of parameters, including a comparison function. \
	\
	CAG_DEF_ALL_CMP_ARRAY \
	CAG_DEF_ALL_CMP_DLIST \
	CAG_DEF_ALL_CMP_SLIST \
	CAG_DEF_ALL_CMP_TREE \
	CAG_DEF_ALL_CMP_HASH

- The generated functions for a container type are suffixed with the name of the container type. E.g. if you declare an array type of integers called *int_array* then all the generated structs, types and functions are suffixed *int_array*.

- Functions that take an element as a parameter often come in two forms: by value or by address. The verb of the by address version will be suffixed with a *p*. E.g. *append_[container](&container variable, element)* and *appendp_[container](&container variable, &element)* are by value and by address functions respectively.

- Every container type has an iterator type defined for it: *it_[container]* or _struct iterator_[container] *_.

- Bidirectional containers (arrays, doubly-linked lists and trees) also have a reverse iterator type defined: *rit_[container]* or *struct iterator_[container] **.

- Bidirectional containers have reverse iterator versions defined for several functions. Reverse functions are prefixed with an *r*, e.g. *rsort_all_[container]*.

- Some functions have variations that can operate on a single element and a range of elements. The version that operates on a range will have its verb suffixed with *range_*. E.g. *erase_[container]*, *erase_range_[container]*. However, if a function only sensibly can operate on a range, then the *range_* suffix is omitted. E.g. there is a *sort_* function but not a *sort_range_* one.

- Functions that operate on a range typically can also operate on the entire container and a *_all* version will be defined. E.g. *sort_all_[container]* and *random_shuffle_all_[container]*.

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

# Portability

CAGL is compatible with ANSI C 1989 compliant compilers, as well as subsequent standard versions of C. The 1989 C definition specifies an archaic limitation that external variable names that share the first six characters do not have to be treated as unique. This is not a limitation for any modern C compiler and since adhering to it would have rendered CAGL impractical, it is ignored, as it is by most useful C libraries.

The test suite currently compiles without warnings and executes 100% successfully using GNU C and Clang with these options:

*-Wall -pedantic -Wstrict-prototypes -Wextra*

It is intended that the test suite of future versions of CAGL will also be compiled, without generating warnings, and executed with Microsoft's and Intel's C compilers.

Currently CAGL is tested under GNU/Linux. Future tests should also be carried out under Windows and OS X.
