#include <stdio.h>
#include <stdlib.h>

#include "eg_points.h"

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
