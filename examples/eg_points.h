#ifndef EG_POINTS_H
#define EG_POINTS_H

#include "cagl/dlist.h"

struct point {
	double x;
	double y;
};

CAG_DEC_DLIST(points, struct point);

double shortest_distance_using_index(points *);
double shortest_distance_using_iterators(points *);

#endif
