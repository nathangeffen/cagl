#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "eg_points.h"

CAG_DEF_DLIST(points, struct point);

/* This functions use an inefficient brute force method to calculate the
   all-pairs shortest distance in a set of Euclidean points. Their time
   complexity is O(n^2) but there is a better, albeit much more complicated
   (and beautiful) algorithm that is O(n log n).
*/

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

double shortest_distance_using_iterators(points *a)
{
	/* If you don't like pointers or structs to be typedef'd
           you can instead do:

           struct iterator_points *i, *j;
	*/
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
