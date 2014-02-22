## Dictionary {-}

In this example, we want to create a dictionary for storing words and their meanings. The obvious implementation is a balanced binary tree whose elements are this:

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

We are lazy (or very cautious) and want CAGL to manage the memory of our dictionary entries. But the *struct entry* members are pointers whose memory needs to be managed. In C++ you would do this by writing a constructor and destructor for *entry* and a *std::map\<entry\>* would manage the allocation and deletion of its members by calling these implicitly whenever you added or erased an element in the map. Although C doesn't have constructors or destructors, you can pass two functions to the CAGL definition which effectively serve the same purpose (actually a copy constructor is the more appropriate analogy with C++). When we insert an entry into the tree we will copy it from an existing *struct entry*.

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

We need to use a definition macro with more parameters than in our previous examples.


```C
CAG_DEC_DEF_ALL_CMP_TREE(dictionary, struct entry, cmp_entry, CAG_BYADR,
			             CAG_STRUCT_ALLOC_STYLE, alloc_entry, free_entry);
```

The definition macros we've used until now actually call one like this with default parameters from the fourth one onwards.

Here's what the parameters mean:

- *dictionary* is the name of our tree container type.

- *struct entry* is the element type of *dictionary*.

- *CAG_BYADR* means our comparison function, *cmp_entry* in this case, takes its parameters by address. If it took its parameters by value, we'd set this to *CAG_BYVAL*.

- *CAG_STRUCT_ALLOC_STYLE*: is a CAGL macro that allocates the memory for *struct*s. Since *entry* is a struct and we want CAGL to manage its memory, this is the correct allocation style parameter to use. If we didn't need CAGL to manage the memory of the elements, we'd set this to *CAG_NO_ALLOC_STYLE*. For more information on the different values this parameter can be, see [Allocation style macros](#allocation-style).

- *alloc_entry* is the function we wrote for allocating the memory of an entry. It will be called by the code in *CAG_STRUCT_ALLOC_STYLE*. If we didn't want CAGL to manage the memory of our elements, we'd instead use *CAG_NO_ALLOC_FUNC*.

- *free_entry* is the function we wrote to return an entry's memory to the heap. If we didn't want CAGL to manage the memory of our elements, we'd instead use *CAG_NO_FREE_FUNC*.

Again, we could have declared and defined the dictionary in two steps like this instead with the same results:

```C
CAG_DEC_CMP_TREE(dictionary, struct entry);
CAG_DEF_ALL_CMP_TREE(dictionary, struct entry, cmp_entry, CAG_BYADR,
                     CAG_STRUCT_ALLOC_STYLE, alloc_entry, free_entry);
```

The above method is what we'd use if *dictionary* was needed across multiple modules.

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

Then we would not have to rewrite our comparison function, but we would have to manage the memory of each entry. By now you are hopefully understanding the differences between the container declaration and definition macros as well as the parameters they accept.

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

Note that it's in alphabetical order even though we inserted the entries arbitrarily. The CAGL binary tree container modification functions maintain entries in alphabetical order and unless you intentionally invalidate the order of the tree (in which case results are undefined), it will remain ordered. This is in contrast to arrays and lists, even ones defined with *CMP* macros. It's valid to scramble the order of any CAGL array or list, but the tree container is always ordered.

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

The code for this example is in *eg_dictionary.c* in the *examples* sub-directory. The *input.txt* file in the same directory can be used as example input redirected from *stdin*.

Because C strings are often needed in containers, and because hash tables and binary trees frequently store dictionaries, CAGL makes it easier by supplying macros that are abbreviations for the declarations and definitions we've used above. Here's a quick description of each of them. They all maintain the memory of their elements, similarly to the way C++ STL containers manage the memory of strings.

- CAG_DEC_STR_HASH(string_hash): Declares a hash table called *string_hash* whose elements are _char *_.
- CAG_DEF_STR_HASH(string_hash): Defines a hash table called *string_hash* whose elements are _char *_.
- CAG_DEC_DEF_STR_HASH(string_hash): Combines the above two macros in one.
- CAG_DEC_STR_TREE(string_tree), CAG_DEF_STR_TREE(string_tree) and CAG_DEC_DEF_STR_TREE(string_tree) which are the tree equivalents of the above hash table declarations and definitions.
- CAG_DEC_STR_ARRAY(string_array), CAG_DEF_STR_ARRAY(string_array) and CAG_DEC_DEF_STR_ARRAY(string_array) which are the array equivalents of the above hash table declarations and definitions.
- CAG_DEC_STR_DLIST(string_dlist), CAG_DEF_STR_DLIST(string_dlist) and CAG_DEC_DEF_STR_DLIST(string_dlist) which are the doubly linked list equivalents of the above hash table declarations and definitions.
- CAG_DEC_STR_SLIST(string_slist), CAG_DEF_STR_SLIST(string_slist) and CAG_DEC_DEF_STR_SLIST(string_slist) which are the singly linked list equivalents of the above hash table declarations and definitions.

There's only one way to define a C string, _char *_. But there are many ways to define a struct that stores dictionaries, so you have to do a bit more work. First make sure you have declared a struct that holds a dictionary. This must be a struct that consists of two _char *_ elements.

For example:

```C
struct dictionary {
	char *w;
	char *d;
};
```

Then we can do this:

- CAG_DEC_STR_STR_HASH(dict_hash, struct dictionary);
- CAG_DEF_STR_STR_HASH(dict_hash, struct dictionary);
- CAG_DEC_DEF_STR_STR_HASH(dict_hash, struct dictionary);

- CAG_DEC_STR_STR_TREE(dict_tree, struct dictionary );
- CAG_DEF_STR_STR_TREE(dict_tree, struct dictionary);
- CAG_DEC_DEF_STR_STR_TREE(dict_tree, struct dictionary);

Default dictionary declaration and definition macros are only available for trees and hash tables.
