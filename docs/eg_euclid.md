## Example: Euclidean points {-}

Let's generate a set of points, store them in a list and then find the two closest points. To demonstrate how you can write modular code with CAGL we're going to have three files:

- eg_points1.c which will contain our main function as well as functions to populate our points.
- eg_points2.c which will contain our functions that operate on points and list of points.
- eg_points.h which will contain our declarations common to eg_points1.c and eg_points2.c

In eg_points.h we'll first include our CAGL dlist header as follows:

```C
#include <cagl/dlist.h>
```

Then we declare our struct:

```C
struct point {
	double x;
	double y;
};
```

We also need a list to store the points:

```C
CAG_DEC_DLIST(points, struct point);
```

We've declared a type called *points* which is a list of *struct point*. We've also declared prototypes for functions that operate on *points*. In *eg_points2.c* we need to define our functions that operate on the *points* type:

```C
CAG_DEF_DLIST(points, struct point);
```
We'll implement two ways of populating our points. One will populate a list with points in an ordered way. Here is how we could do it in *eg_points1.c*:


```C
void populate_ordered(points *a, int numpoints,
                      double x_start, double x_step, double gradient)
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

We use the CAGL generated *append_points* function to put each new point at the back of the list. For doubly-linked lists and arrays, this is an efficient constant time operation.

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

*CAG_TRUE* and *CAG_FALSE* are macros equal to 1 and 0 respectively. You don't have to use them. (Why haven't we used the C99 bool type? We could, but unless otherwise specified all the examples here are C89 compatible.)

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

Here's a function to populate the list with randomly generated points. But notice that here we call *appendp_points* instead of *append_points*. In CAGL, if the verb of the function ends with a *p*, then the function takes its element parameter by address not by value. In this example it makes little difference, but if the elements of our container are large then the *appendp* version will be much more efficient because it won't copy the entire structure when calling the append function.

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

Note that *it* is iterated by the macro over all the elements in our container variable called *random*. Using a macro to abstract away a for loop is not every C programmer's cup of tea. In which case, feel free to ignore the idiom below and use the one above.

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

We'll implement two ways of calculating the shortest distance. One will use indices and the other will use iterators.

Here are the prototypes for our two functions in *eg_points.h*:

```C
double shortest_distance_using_index(points *);
double shortest_distance_using_iterators(points *);
```

Now in *eg_points2.c* we will implement our two shortest distance functions. They use a brute force algorithm which is simple but inefficient. For our purposes, the brute force method is sufficient. The algorithm steps through each point in the list except the last and calculates its Euclidean distance to every point in front of it, updating the smallest distance found if necessary.

Here's our implementation:

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

This method uses indices, *i* and *j*, to step through the list. It uses two CAGL functions, *distance_all_points*, which returns the number of elements in the list, and *index_points*, which returns an iterator to the *i*th or *j*th element in the container.

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
