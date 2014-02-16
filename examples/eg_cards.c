#include <stdio.h>
#include <stdlib.h>
#include "cagl/array.h"

struct card {
	int rank;
	char suit;
};

int cmp_cards(const struct card *c1, const struct card *c2)
{
	int r = c1->rank - c2->rank;
	return r == 0 ? c1->suit - c2->suit : r;
}


CAG_DEC_DEF_CMPP_ARRAY(deck, struct card, cmp_cards);

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

/* Deck purposefully not loaded in order. */

int populate_deck(deck *d)
{
	int rank;
	struct card crd;

	for (rank = 0; rank < 13; ++rank) {
		crd.rank = rank;
		crd.suit = 'S';
		if (!appendp_deck(d, &crd)) return CAG_FALSE;
		crd.suit = 'H';
		if (!appendp_deck(d, &crd)) return CAG_FALSE;
		crd.suit = 'D';
		if (!appendp_deck(d, &crd)) return CAG_FALSE;
		crd.suit = 'C';
		if (!appendp_deck(d, &crd)) return CAG_FALSE;
	}
	return CAG_TRUE;
}

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
