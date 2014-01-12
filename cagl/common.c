/*! CAG supporting functions

  \copyright Copyright 2013 University of Cape Town. All rights reserved.
  \license GNU Lesser General Public License Copyright.
  See COPYING for the license text.
*/


#include <stdlib.h>
#include <string.h>

/* Simple hash function from K&R, 2nd ed. p144 */

size_t cag_kr_hash(const void *key, const size_t len)
{
	size_t hashval = 0;
	size_t i;
	const unsigned char *p = key;
	for (i = 0; i < len; ++i)
		hashval = p[i] + 31 * hashval;
	return hashval;
}


/* One at a time hash adapted from Eternally Confuzzled.
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 */

size_t cag_oat_hash (const void *key, const size_t len )
{
	const unsigned char *p = key;
	unsigned h = 0;
	size_t i;

	for ( i = 0; i < len; i++ ) {
		h += p[i];
		h += ( h << 10 );
		h ^= ( h >> 6 );
	}
	h += ( h << 3 );
	h ^= ( h >> 11 );
	h += ( h << 15 );

	return h;
}

size_t cag_int_hash(const int key, const size_t len)
{
	(void) len;
	return key;
}

/* Implementation of strdup which some C library implementations might not
   include.
*/

char *cag_strdup(const char *s)
{
	size_t len = strlen(s) + 1;
	char *output = malloc(sizeof(char) * len);
	strcpy(output, s);
	return output;
}


/* Allocator for struct {char *, char *}, i.e. a typical dictionary. */

int cag_alloc_str_str(void *to, const void *from)
{
	static struct str_str {
		char *key;
		char *data;
	} *x;

	const struct str_str *y;

	x = to;
	y = from;

	x->key = cag_strdup(y->key);
	if (!x->key)
		return 0;
	x->data = cag_strdup(y->data);
	if (!x->data) {
		free(x->key);
		return 0;
	}
	return 1;
}

/* De-allocator for struct {char *, char *}, i.e. a typical dictionary. */

void cag_free_str_str(void *x)
{
	static struct str_str {
		char *key;
		char *data;
	} *y;

	y = x;
	free(y->key);
	free(y->data);
}
