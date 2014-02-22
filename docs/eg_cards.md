## Example: Deck of cards {-}

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

To do this we simply use the *CAG_DEC_DEF_CMPP_ARRAY* macro. The extra *P* in *CMPP* means that the comparison function takes its parameters by address (or as pointers, hence the *P*).

```C
CAG_DEC_DEF_CMPP_ARRAY(deck, struct card, cmp_cards);
```

You could declare this in two macros as follows:

```C
CAG_DEC_CMP_ARRAY(deck, struct card);
CAG_DEF_CMPP_ARRAY(deck, struct card, cmp_cards);
```

It's identical except that you would use the two-macro version if *deck* was needed in multiple modules. Also *CAG_DEC_CMPP_ARRAY* is identical to *CAG_DEC_CMP_ARRAY*; you can use them interchangeably.

We want to populate our deck of cards so we write this function, which intentionally does not load the cards in order.

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

The *sort_deck* function takes two iterators pointing to elements in a *deck* and sorts the elements in that range. Many CAGL functions work this way. But operating on an entire container is so common, that CAGL also often provides a short-hand function so we don't have to type *beg_deck(&d), end_deck(&d)*. This does the trick:

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

The full code for this example is in *eg_cards.c* in the *examples* sub-directory.
