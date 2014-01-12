#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "cagl/array.h"

struct complex {
		long double real;
		long double imag;
};

static int cmp(const struct complex x, const struct complex y)
{
		if (x.real < y.real) return -1;
		if (x.real > y.real) return 1;
		return 0;
}

int main(int argc, char *argv[])
{
		size_t size, i;
		clock_t t;
		if (argc < 3) {
				printf("Usage: %s <size> <random|sorted|reversed>\n", argv[0]);
				return 1;
		}
		size = (size_t) atoi(argv[1]);

		struct complex *a = (struct complex *)
				malloc(size * sizeof(struct complex));
		if (!a) {
				printf("Out of mem.\n");
				return 1;
		}

		if (!strcmp("random", argv[2])) {
				for (i = 0; i < size; ++i) {
						a[i].real = rand() % size;
						a[i].imag = 0.0;
				}
		} else if (!strcmp("sorted", argv[2])) {
				for (i = 0; i < size; ++i) {
						a[i].real = i;
						a[i].imag = 0.0;
				}
		} else if (!strcmp("reversed", argv[2])) {
				for (i = 0; i < size; ++i) {
						a[i].real = size - i;
						a[i].imag = 0.0;
				}
		} else if (!strcmp("duplicates", argv[2])) {
				for (i = 0; i < size; ++i) {
						a[i].real = 0.0;
						a[i].imag = 0.0;
				}
		} else {
				printf("Unknown sort type: %s\n", argv[2]);
				return 1;
		}
		printf("Sorting a list that is %s.\n", argv[2]);
		t = clock();
		CAG_SORT_ARRAY_DEFAULT(struct complex, a, a+size, cmp);
		t = clock() - t;
		for (i = 1; i < size; ++i) {
				if (a[i].real < a[i-1].real) {
						printf("Problem: %zu %.0LF %.0Lf\n",
							   i, a[i-1].real, a[i].real);
						break;
				}
		}
		printf("Finished in %ld seconds.\n", t / CLOCKS_PER_SEC);
		free(a);
		return 0;
}
