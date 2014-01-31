/*********/
/* ARRAY */
/*********/

struct iterator_C {
    T value;
};
struct reverse_iterator_C {
    T value;
};
typedef struct iterator_C iterator_C;
typedef struct reverse_iterator_C reverse_iterator_C;
typedef iterator_C *it_C;
typedef reverse_iterator_C *rit_C;
struct C {
    iterator_C *begin;
    iterator_C *end;
    iterator_C *objects;
    size_t capacity;
};
typedef struct C C;
C *new_C(C * array);
C *new_with_capacity_C(C * array, const size_t reserve);
C *new_with_size_C(C * array, const size_t size);
it_C set_min_size_C(C * array, it_C it, const size_t size);
it_C set_exact_size_C(C * array, const size_t size);
it_C next_C(it_C const it);
it_C prev_C(it_C const it);
rit_C rnext_C(rit_C const it);
rit_C rprev_C(rit_C const it);
it_C at_C(it_C it, const size_t n);
rit_C rat_C(rit_C it, const size_t n);
size_t distance_C(const it_C from, const it_C to);
size_t rdistance_C(const rit_C from, const rit_C to);
int lt_it_C(const it_C it1, const it_C it2);
int lteq_it_C(const it_C it1, const it_C it2);
int rlt_it_C(const rit_C it1, const rit_C it2);
int rlteq_it_C(const rit_C it1, const rit_C it2);
it_C begin_C(const C * array);
it_C end_C(const C * array);
rit_C rbegin_C(const C * array);
rit_C rend_C(const C * array);
size_t size_C(const C * array);
it_C prepend_C(C * array, T const element);
it_C append_C(C * array, T const element);
it_C prependp_C(C * array, T const *element);
rit_C rprepend_C(C * array, T const element);
it_C appendp_C(C * array, T const *element);
rit_C rprependp_C(C * array, T const *element);
it_C insert_C(C * array, it_C position, T const element);
it_C put_C(C * array, it_C position, T const element);
it_C insertp_C(C * array, it_C position, T const *element);
it_C putp_C(C * array, it_C position, T const *element);
rit_C rinsert_C(C * array, rit_C position, T const element);
rit_C rappend_C(C * array, T const element);
T *front_C(const C * array);
T *back_C(const C * array);
T *rfront_C(const C * array);
T *rback_C(const C * array);
it_C erase_C(C * array, it_C it);
it_C erase_range_C(C * array, it_C from, it_C to);
void free_C(C * array);
it_C reverse_C(it_C first, it_C last);
it_C reverse_all_C(C * c);
it_C random_shuffle_C(const it_C from, it_C to);
it_C random_shuffle_all_C(C * c);
extern it_C(*const beg_C) (const C *);
C *new_from_C(C * to, const C * from);
int new_many_C(C * c, ...);
size_t distance_all_C(const C * c);
void swap_C(it_C a, it_C b);
it_C index_C(C * c, size_t n);
it_C erase_all_C(C * c);
void free_many_C(int max, C * c, ...);
C *copy_C(it_C first, it_C last, C * c);
C *copy_all_C(const C * c1, C * c2);
int copy_many_C(C * c, ...);
C *copy_if_C(it_C first, it_C last, C * c, int (*cond_func) (T *, void *),
	     void *data);
C *copy_if_all_C(const C * c1, C * c2, int (*cond_func) (T *, void *),
		 void *data);
it_C copy_over_C(it_C first, const it_C last, it_C result);
it_C find_C(it_C from, const it_C to, const T element,
	    int (*cmp_func) (const T *, const T *));
it_C findp_C(it_C from, const it_C to, const T * element,
	     int (*cmp_func) (const T *, const T *));
it_C find_all_C(const C * c, const T element,
		int (*cmp_func) (const T *, const T *));
it_C findp_all_C(C * c, const T * element,
		 int (*cmp_func) (const T *, const T *));
extern rit_C(*const rbeg_C) (const C *);
void rswap_C(rit_C a, rit_C b);
rit_C rfind_C(rit_C from, const rit_C to, const T element,
	      int (*cmp_func) (const T *, const T *));
rit_C rfindp_C(rit_C from, const rit_C to, const T * element,
	       int (*cmp_func) (const T *, const T *));
C *rcopy_C(rit_C first, rit_C last, C * c);
C *rcopy_all_C(const C * c1, C * c2);
it_C stable_sort_C(it_C from, it_C to);
rit_C rstable_sort_C(rit_C from, rit_C to);
it_C sort_C(it_C from, it_C to);
rit_C rsort_C(rit_C from, rit_C to);
it_C sort_all_C(C * c);
rit_C rsort_all_C(C * c);
it_C stable_sort_all_C(C * c);
it_C insert_gt_C(C * c, it_C position, T element);
it_C insert_gteq_C(C * c, it_C position, T element);
it_C insert_lt_C(C * c, it_C position, T element);
it_C insert_lteq_C(C * c, it_C position, T element);
it_C insertp_gt_C(C * c, it_C position, T * element);
it_C insertp_gteq_C(C * c, it_C position, T * element);
it_C insertp_lt_C(C * c, it_C position, T * element);
it_C insertp_lteq_C(C * c, it_C position, T * element);
int cmp_C(const it_C it1, const it_C it2);
int cmp_range_C(it_C from_1, const it_C to_1, it_C from_2,
		const it_C to_2);
int cmp_all_C(const C * c1, const C * c2);
int equal_range_C(it_C from_1, const it_C to_1, it_C from_2);
int equal_all_C(const C * c1, const C * c2);
it_C search_C(it_C first, const it_C last, T const key);
it_C searchp_C(it_C first, const it_C last, T const *key);
it_C search_all_C(C * c, T d);
it_C searchp_all_C(C * c, T * d);
int rcmp_C(const rit_C it1, const it_C it2);
int rcmp_range_C(rit_C from_1, const rit_C to_1, it_C from_2,
		 const it_C to_2);
int rcmp_all_C(const C * c1, const C * c2);
int requal_range_C(rit_C from_1, const rit_C to_1, it_C from_2);
int requal_all_C(const C * c1, const C * c2);
rit_C rsearch_C(rit_C first, const rit_C last, T const key);
rit_C rsearchp_C(rit_C first, const rit_C last, T const *key);
it_C lower_bound_C(it_C first, it_C last, T const key);
rit_C lower_rbound_C(rit_C first, rit_C last, T const key);
it_C lower_boundp_C(it_C first, it_C last, T const *key);
rit_C lower_rboundp_C(rit_C first, rit_C last, T const *key);
it_C lower_bound_all_C(C * c, T d);
it_C lower_boundp_all_C(C * c, T * d);
int binary_search_C(it_C first, it_C last, T const key);
int binary_rsearch_C(rit_C first, rit_C last, T const key);
int binary_searchp_C(it_C first, it_C last, T const *key);
int binary_rsearchp_C(rit_C first, rit_C last, T const *key);
int binary_search_all_C(C * c, T d);
int binary_searchp_all_C(C * c, T * d);


/*********/
/* DLIST */
/*********/

struct iterator_C {
    struct iterator_C *prev;
    struct iterator_C *next;
    T value;
};
struct reverse_iterator_C {
    struct reverse_iterator_C *next;
    struct reverse_iterator_C *prev;
    T value;
};
typedef struct iterator_C iterator_C;
typedef struct reverse_iterator_C reverse_iterator_C;
typedef iterator_C *it_C;
typedef reverse_iterator_C *rit_C;
struct C {
    it_C begin_;
    it_C end_;
};
typedef struct C C;
C *new_C(C * dlist);
it_C set_min_size_C(C * dlist, it_C it, size_t size);
it_C set_exact_size_C(C * dlist, const size_t size);
it_C next_C(it_C const it);
it_C prev_C(it_C const it);
rit_C rnext_C(rit_C const it);
rit_C rprev_C(rit_C const it);
it_C at_C(it_C iterator, size_t n);
rit_C rat_C(rit_C iterator, size_t n);
size_t distance_C(it_C from, const it_C to);
size_t rdistance_C(rit_C from, const rit_C to);
int lt_it_C(it_C it1, const it_C it2);
int lteq_it_C(it_C it1, const it_C it2);
int rlt_it_C(rit_C it1, const rit_C it2);
int rlteq_it_C(rit_C it1, const rit_C it2);
it_C begin_C(const C * dlist);
it_C end_C(const C * dlist);
rit_C rbegin_C(const C * dlist);
rit_C rend_C(const C * dlist);
it_C prepend_C(C * dlist, T const item);
it_C append_C(C * dlist, T const item);
it_C prependp_C(C * dlist, T const *element);
it_C appendp_C(C * dlist, T const *element);
T *front_C(const C * dlist);
T *back_C(const C * dlist);
it_C insert_C(it_C it, T const element);
it_C insertp_C(it_C it, T const *element);
it_C put_C(C * dlist, it_C it, T const element);
it_C putp_C(C * dlist, it_C it, T const *element);
it_C erase_C(C * dlist, it_C it);
it_C erase_range_C(C * c, it_C from, it_C to);
void free_C(C * list);
extern it_C(*const beg_C) (const C *);
C *new_from_C(C * to, const C * from);
int new_many_C(C * c, ...);
size_t distance_all_C(const C * c);
void swap_C(it_C a, it_C b);
it_C index_C(C * c, size_t n);
it_C erase_all_C(C * c);
void free_many_C(int max, C * c, ...);
C *copy_C(it_C first, it_C last, C * c);
C *copy_all_C(const C * c1, C * c2);
int copy_many_C(C * c, ...);
C *copy_if_C(it_C first, it_C last, C * c, int (*cond_func) (T *, void *),
	     void *data);
C *copy_if_all_C(const C * c1, C * c2, int (*cond_func) (T *, void *),
		 void *data);
it_C copy_over_C(it_C first, const it_C last, it_C result);
it_C find_C(it_C from, const it_C to, const T element,
	    int (*cmp_func) (const T *, const T *));
it_C findp_C(it_C from, const it_C to, const T * element,
	     int (*cmp_func) (const T *, const T *));
it_C find_all_C(const C * c, const T element,
		int (*cmp_func) (const T *, const T *));
it_C findp_all_C(C * c, const T * element,
		 int (*cmp_func) (const T *, const T *));
extern rit_C(*const rbeg_C) (const C *);
void rswap_C(rit_C a, rit_C b);
rit_C rfind_C(rit_C from, const rit_C to, const T element,
	      int (*cmp_func) (const T *, const T *));
rit_C rfindp_C(rit_C from, const rit_C to, const T * element,
	       int (*cmp_func) (const T *, const T *));
C *rcopy_C(rit_C first, rit_C last, C * c);
C *rcopy_all_C(const C * c1, C * c2);
it_C reverse_C(it_C first, it_C last);
it_C reverse_all_C(C * c);
it_C random_shuffle_C(const it_C from, it_C to);
it_C random_shuffle_all_C(C * c);
it_C stable_sort_C(it_C from, it_C to);
rit_C rstable_sort_C(rit_C from, rit_C to);
it_C sort_C(it_C from, it_C to);
rit_C rsort_C(rit_C from, rit_C to);
it_C sort_all_C(C * c);
rit_C rsort_all_C(C * c);
it_C stable_sort_all_C(C * c);
it_C insert_gt_C(C * c, it_C position, T element);
it_C insert_gteq_C(C * c, it_C position, T element);
it_C insert_lt_C(C * c, it_C position, T element);
it_C insert_lteq_C(C * c, it_C position, T element);
it_C insertp_gt_C(C * c, it_C position, T * element);
it_C insertp_gteq_C(C * c, it_C position, T * element);
it_C insertp_lt_C(C * c, it_C position, T * element);
it_C insertp_lteq_C(C * c, it_C position, T * element);
int cmp_C(const it_C it1, const it_C it2);
int cmp_range_C(it_C from_1, const it_C to_1, it_C from_2,
		const it_C to_2);
int cmp_all_C(const C * c1, const C * c2);
int equal_range_C(it_C from_1, const it_C to_1, it_C from_2);
int equal_all_C(const C * c1, const C * c2);
it_C search_C(it_C first, const it_C last, T const key);
it_C searchp_C(it_C first, const it_C last, T const *key);
it_C search_all_C(C * c, T d);
it_C searchp_all_C(C * c, T * d);
int rcmp_C(const rit_C it1, const it_C it2);
int rcmp_range_C(rit_C from_1, const rit_C to_1, it_C from_2,
		 const it_C to_2);
int rcmp_all_C(const C * c1, const C * c2);
int requal_range_C(rit_C from_1, const rit_C to_1, it_C from_2);
int requal_all_C(const C * c1, const C * c2);
rit_C rsearch_C(rit_C first, const rit_C last, T const key);
rit_C rsearchp_C(rit_C first, const rit_C last, T const *key);


/*********/
/* SLIST */
/*********/

struct iterator_C {
    struct iterator_C *next;
    T value;
};
typedef struct iterator_C iterator_C;
typedef iterator_C *it_C;
struct C {
    it_C header;
};
typedef struct C C;
C *new_C(C * slist);
it_C next_C(it_C const it);
it_C begin_C(const C * slist);
it_C end_C(const C * slist);
it_C last_C(const C * slist);
it_C prepend_C(C * slist, T element);
it_C prependp_C(C * slist, T * element);
it_C insert_after_C(C * slist, it_C it, T item);
it_C insertp_after_C(C * slist, it_C it, T * item);
it_C put_C(C * slist, it_C it, T const item);
size_t distance_C(it_C iterator_from, it_C iterator_to);
T *front_C(C * slist);
it_C set_min_size_C(C * slist, it_C it, size_t size);
it_C set_exact_size_C(C * slist, size_t size);
it_C erase_front_C(C * slist);
it_C erase_after_C(it_C it);
it_C erase_after_range_C(it_C from, it_C to);
it_C erase_range_C(C * slist, it_C from, it_C to);
it_C at_C(it_C iterator, size_t n);
C *reverse_all_C(C * slist);
void free_C(C * slist);
extern it_C(*const beg_C) (const C *);
C *new_from_C(C * to, const C * from);
int new_many_C(C * c, ...);
size_t distance_all_C(const C * c);
void swap_C(it_C a, it_C b);
it_C index_C(C * c, size_t n);
it_C erase_all_C(C * c);
void free_many_C(int max, C * c, ...);
C *copy_C(it_C first, it_C last, C * c);
C *copy_all_C(const C * c1, C * c2);
int copy_many_C(C * c, ...);
C *copy_if_C(it_C first, it_C last, C * c, int (*cond_func) (T *, void *),
	     void *data);
C *copy_if_all_C(const C * c1, C * c2, int (*cond_func) (T *, void *),
		 void *data);
it_C copy_over_C(it_C first, const it_C last, it_C result);
it_C find_C(it_C from, const it_C to, const T element,
	    int (*cmp_func) (const T *, const T *));
it_C findp_C(it_C from, const it_C to, const T * element,
	     int (*cmp_func) (const T *, const T *));
it_C find_all_C(const C * c, const T element,
		int (*cmp_func) (const T *, const T *));
it_C findp_all_C(C * c, const T * element,
		 int (*cmp_func) (const T *, const T *));
it_C stable_sort_all_C(C * list);
int cmp_C(const it_C it1, const it_C it2);
int cmp_range_C(it_C from_1, const it_C to_1, it_C from_2,
		const it_C to_2);
int cmp_all_C(const C * c1, const C * c2);
int equal_range_C(it_C from_1, const it_C to_1, it_C from_2);
int equal_all_C(const C * c1, const C * c2);
it_C search_C(it_C first, const it_C last, T const key);
it_C searchp_C(it_C first, const it_C last, T const *key);
it_C search_all_C(C * c, T d);
it_C searchp_all_C(C * c, T * d);


/*********/
/* TREE  */
/*********/

struct iterator_C {
    struct iterator_C *child[2];
    struct iterator_C *parent;
    unsigned char red;
    T value;
};
struct reverse_iterator_C {
    struct iterator_C *child[2];
    struct iterator_C *parent;
    unsigned char red;
    T value;
};
typedef struct value_C value_C;
typedef struct iterator_C iterator_C;
typedef struct reverse_iterator_C reverse_iterator_C;
typedef iterator_C *it_C;
typedef reverse_iterator_C *rit_C;
struct C {
    it_C root;
    it_C header;
};
typedef struct C C;
C *new_C(C * tree);
it_C begin_C(const C * tree);
rit_C rbegin_C(const C * tree);
it_C end_C(const C * tree);
rit_C rend_C(const C * tree);
T *front_C(const C * tree);
T *back_C(const C * tree);
it_C next_C(it_C it);
it_C prev_C(it_C it);
rit_C rnext_C(rit_C it);
rit_C rprev_C(rit_C it);
it_C at_C(it_C iterator, size_t n);
rit_C rat_C(rit_C iterator, size_t n);
size_t distance_C(const it_C from, const it_C to);
size_t rdistance_C(const rit_C from, const rit_C to);
size_t rdistance_all_C(C * c);
it_C insert_C(C * tree, const T element);
it_C insertp_C(C * tree, const T * element);
it_C put_C(C * tree, it_C it, T const element);
void preorder_C(it_C root, void *data, void (*action) (it_C, void *));
void inorder_C(it_C root, void *data, void (*action) (it_C, void *));
void reverseorder_C(rit_C root, void *data,
		    void (*action) (rit_C, void *));
void postorder_C(it_C root, void *data, void (*action) (it_C, void *));
void levelorder_C(C * tree, void *data,
		  void (*action) (it_C, size_t, int, int, void *));
it_C get_C(const it_C root, const T element);
it_C getp_C(const it_C root, const T * element);
it_C erase_C(C * tree, it_C it);
it_C erase_range_C(C * c, it_C from, it_C to);
it_C remove_C(C * tree, const T element);
void free_node_C(C * tree, it_C it);
void free_C(C * tree);
int check_integrity_C(const C * tree, it_C it);
extern it_C(*const beg_C) (const C *);
C *new_from_C(C * to, const C * from);
int new_many_C(C * c, ...);
size_t distance_all_C(const C * c);
void swap_C(it_C a, it_C b);
it_C index_C(C * c, size_t n);
it_C erase_all_C(C * c);
void free_many_C(int max, C * c, ...);
C *copy_C(it_C first, it_C last, C * c);
C *copy_all_C(const C * c1, C * c2);
int copy_many_C(C * c, ...);
C *copy_if_C(it_C first, it_C last, C * c, int (*cond_func) (T *, void *),
	     void *data);
C *copy_if_all_C(const C * c1, C * c2, int (*cond_func) (T *, void *),
		 void *data);
it_C copy_over_C(it_C first, const it_C last, it_C result);
it_C find_C(it_C from, const it_C to, const T element,
	    int (*cmp_func) (const T *, const T *));
it_C findp_C(it_C from, const it_C to, const T * element,
	     int (*cmp_func) (const T *, const T *));
it_C find_all_C(const C * c, const T element,
		int (*cmp_func) (const T *, const T *));
it_C findp_all_C(C * c, const T * element,
		 int (*cmp_func) (const T *, const T *));
extern rit_C(*const rbeg_C) (const C *);
void rswap_C(rit_C a, rit_C b);
rit_C rfind_C(rit_C from, const rit_C to, const T element,
	      int (*cmp_func) (const T *, const T *));
rit_C rfindp_C(rit_C from, const rit_C to, const T * element,
	       int (*cmp_func) (const T *, const T *));
C *rcopy_C(rit_C first, rit_C last, C * c);
C *rcopy_all_C(const C * c1, C * c2);
int cmp_C(const it_C it1, const it_C it2);
int cmp_range_C(it_C from_1, const it_C to_1, it_C from_2,
		const it_C to_2);
int cmp_all_C(const C * c1, const C * c2);
int equal_range_C(it_C from_1, const it_C to_1, it_C from_2);
int equal_all_C(const C * c1, const C * c2);
it_C search_C(it_C first, const it_C last, T const key);
it_C searchp_C(it_C first, const it_C last, T const *key);
it_C search_all_C(C * c, T d);
it_C searchp_all_C(C * c, T * d);
int rcmp_C(const rit_C it1, const it_C it2);
int rcmp_range_C(rit_C from_1, const rit_C to_1, it_C from_2,
		 const it_C to_2);
int rcmp_all_C(const C * c1, const C * c2);
int requal_range_C(rit_C from_1, const rit_C to_1, it_C from_2);
int requal_all_C(const C * c1, const C * c2);
rit_C rsearch_C(rit_C first, const rit_C last, T const key);
rit_C rsearchp_C(rit_C first, const rit_C last, T const *key);


/*********/
/* HASH */
/*********/

struct iterator_C {
    struct iterator_C *next;
    struct iterator_C **bucket;
    T value;
};
typedef struct iterator_C iterator_C;
typedef iterator_C *it_C;
struct C {
    it_C *objects;
    size_t buckets;
    size_t size;
    int rehash;
};
typedef struct C C;
C *new_with_buckets_C(C * hash, const size_t buckets);
C *new_C(C * hash);
C *new_with_buckets_C(C * hash, const size_t buckets);
it_C get_C(const C * hash, const T element);
it_C getp_C(const C * hash, const T * element);
it_C insert_C(C * hash, T const element);
it_C insertp_C(C * hash, T const *element);
it_C put_C(C * hash, it_C it, T const element);
it_C begin_C(const C * hash);
it_C end_C(const C * hash);
it_C next_C(it_C const it);
it_C at_C(it_C it, size_t n);
size_t distance_C(it_C from, const it_C to);
it_C remove_C(C * hash, const T element);
it_C removep_C(C * hash, const T * element);
it_C erase_C(C * hash, it_C it);
it_C erase_range_C(C * c, it_C from, it_C to);
C *rehash_C(C * hash, size_t buckets);
void free_C(C * hash);
extern it_C(*const beg_C) (const C *);
C *new_from_C(C * to, const C * from);
int new_many_C(C * c, ...);
size_t distance_all_C(const C * c);
void swap_C(it_C a, it_C b);
it_C index_C(C * c, size_t n);
it_C erase_all_C(C * c);
void free_many_C(int max, C * c, ...);
C *copy_C(it_C first, it_C last, C * c);
C *copy_all_C(const C * c1, C * c2);
int copy_many_C(C * c, ...);
C *copy_if_C(it_C first, it_C last, C * c, int (*cond_func) (T *, void *),
	     void *data);
C *copy_if_all_C(const C * c1, C * c2, int (*cond_func) (T *, void *),
		 void *data);
it_C copy_over_C(it_C first, const it_C last, it_C result);
it_C find_C(it_C from, const it_C to, const T element,
	    int (*cmp_func) (const T *, const T *));
it_C findp_C(it_C from, const it_C to, const T * element,
	     int (*cmp_func) (const T *, const T *));
it_C find_all_C(const C * c, const T element,
		int (*cmp_func) (const T *, const T *));
it_C findp_all_C(C * c, const T * element,
		 int (*cmp_func) (const T *, const T *));
