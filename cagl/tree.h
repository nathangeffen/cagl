/*! \file CAGL balanced binary tree, implemented as a red-black tree.

  \copyright Copyright 2014 Nathan Geffen. All rights reserved.

  \license This code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE.

  \sa COPYING for the license text.

  \sa howtodev.rst to learn how this code works and how to modify it.

  Implementation of Red-Black tree helped a lot by:

  - Wikipedia:
    http://en.wikipedia.org/wiki/Red%E2%80%93black_tree

  - Eternally Confuzzled:
    http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx

  - Source code of the C++ STL

  - And especially by this excellent piece of scholarship:
    An Introduction to Binary Search Trees and Balanced Trees by Ben Pfaff:
    ftp://ftp.gnu.org/pub/gnu/avl/avl-2.0.2.pdf.gz

  One complication is the use of a field called header to manage finding the
  *begin* and *end* of the tree. Cf. the somewhat different approaches used for
  the implementations of the doubly linked list and the singly linked list.  In
  time, perhaps both implementations should be made consistent, though these
  details are irrelevant to users.

  Insertion and deletion are non-recursive, but traversal functions use
  recursion.  So is *free* recursive. Perhaps, to be consistent with the rest of
  of CAGL, these should be converted to non-recursive functions, though this is
  irrelevant to users.

*/


#ifndef CAG_TREE_H
#define CAG_TREE_H

#include <stdlib.h>
#include "cagl/concepts.h"

/*! \brief Macro to check if an iterator points to an element and is therefore
    not NULL and not one of the special nodes used to signify the beginning or
    end of the tree.
*/

#define CAG_IS_IT_TREE(it) (it != NULL && (void *) it->parent != (void *) it)

/*! \brief Macro and function declaration and definition to initialize
    doubly linked list.
*/

#define CAG_DEC_NEW_TREE(function, container) \
    container *function(container *tree)

#define CAG_DEF_NEW_TREE(function, container) \
    CAG_DEC_NEW_TREE(function, container) \
    { \
        tree->root = NULL; \
        tree->header = malloc(sizeof(* tree->header)); \
        if (!tree->header) return NULL; \
        tree->header->parent = tree->header; \
        tree->header->child[0] = tree->header; \
        tree->header->child[1] = tree->header; \
        tree->header->red = 0; \
        return tree; \
    }

/*! \brief Macro to check if a node is red. */

#define CAG_IS_RED_TREE(node) (node && node->red)

/*! \brief Function declaration and definition for *begin* and *end*. */

#define CAG_DEC_BEGIN_TREE(function, container, iterator_type) \
    iterator_type function(const container *tree)

#define CAG_DEF_BEGIN_TREE(function, container, iterator_type, dir) \
    CAG_DEC_BEGIN_TREE(function, container, iterator_type) \
    { \
        return (iterator_type) tree->header->child[dir]; \
    }

#define CAG_DEC_END_TREE(function, container, iterator_type) \
    iterator_type function(const container *tree)

#define CAG_DEF_END_TREE(function, container, iterator_type) \
    CAG_DEC_END_TREE(function, container, iterator_type) \
    { \
        return (iterator_type) tree->header; \
    }

/*! \brief Function declaration and definition to iterate one element forward or
    backward in the tree.
*/

#define CAG_DEC_STEP_TREE(function, iterator_type) \
    iterator_type function(iterator_type it)

#define CAG_DEF_STEP_TREE(function, dir, iterator_type) \
    CAG_DEC_STEP_TREE(function, iterator_type) \
    { \
        if (it->child[dir])  { \
            it = (iterator_type) it->child[dir]; \
            while(it->child[!dir] && (iterator_type) it->parent != it) \
                it = (iterator_type) it->child[!dir]; \
            return it; \
        } \
        if (it->parent && it == (iterator_type) it->parent->child[!dir]) \
            return (iterator_type) it->parent; \
        while(it->parent && it == (iterator_type) it->parent->child[dir]) \
            it = (iterator_type) it->parent; \
        it = (iterator_type) it->parent; \
        return it; \
    }

/*! \brief Macros and function declarations and definitions to find the
    front and back elements of the tree.
*/

#define CAG_BOUNDARY_TREE(tree, dir) \
    &tree->header->child[dir]->value

#define CAG_DEC_BOUNDARY_TREE(function, container, type) \
    type *function(const container *tree)

#define CAG_DEF_BOUNDARY_TREE(function, container, type, dir) \
    CAG_DEC_BOUNDARY_TREE(function, container, type) \
    { \
        return CAG_BOUNDARY_TREE(tree, dir); \
    }

/*! \brief Macros and function declaration and definition to binary search the
    tree. By value and address versions implemented. Attempted to use a generic
    binary search in cag_common, but ultimately decided on a customised
    implementation out of frustration. Would be good to make the cag/common.h
    binary search cover this case as well.
*/

#define CAG_GET_TREE(iterator_type, root, cmp_func, val_adr, val) \
    do { \
        iterator_type p; \
        int result; \
        p = root; \
        while(CAG_IS_IT_TREE(p)) { \
            result = cmp_func(val_adr (p->value), val_adr (val)); \
            if (result > 0) \
                p = (iterator_type) p->child[0]; \
            else if (result < 0) \
                p = (iterator_type) p->child[1]; \
            else \
                return p; \
        } \
        return NULL; \
    } while(0)


#define CAG_DEC_GET_TREE(function, container, type)	\
    it_ ## container function(const container *tree, const type element)

#define CAG_DEF_GET_TREE(function, container, type, cmp_func, val_adr) \
    CAG_DEC_GET_TREE(function, container, type) \
    { \
        CAG_GET_TREE(it_ ## container, tree->root, cmp_func, val_adr, element); \
    }


#define CAG_DEC_GETP_TREE(function, container, type) \
    it_ ## container function(const container *tree, const type *element)

#define CAG_DEF_GETP_TREE(function, container, type, cmp_func, val_adr) \
    CAG_DEC_GETP_TREE(function, container, type) \
    { \
        CAG_GET_TREE(it_ ## container, tree->root, cmp_func, val_adr, *element);\
    }



/*! \brief Insert function adapted from: An Introduction to Binary Search Trees
   and Balanced Trees by Ben Pfaff
   ftp://ftp.gnu.org/pub/gnu/avl/avl-2.0.2.pdf.gz and the constcorresponding code in
   libavl.
 */

#define CAG_INSERT_TREE(container, iterator_type, type, \
                        cmp_func, val_adr, alloc_style, alloc_func, item) \
{ \
    iterator_type p;  /* Traverses tree looking for insertion point. */ \
    iterator_type q; /* Parent of p; node at which we are rebalancing. */ \
    iterator_type n; /* Newly inserted node. */ \
    int dir; /* Side of q on which n is inserted. */ \
    int leftmost = 0, rightmost = 0; \
    int result; \
    assert (tree != NULL); \
    for (q = NULL, p = tree->root; CAG_IS_IT_TREE(p); \
            q = p, p = p->child[dir]) { \
        result = cmp_func(val_adr (item), val_adr (p->value)); \
        if (result == 0) \
            return p; \
        dir = result > 0; \
    } \
    n = CAG_MALLOC(sizeof(*p)); \
    if (!n) \
        return NULL; \
    alloc_style(n->value, (item), alloc_func, \
                { \
                        CAG_FREE(n); \
                        return NULL; \
                }); \
    n->child[0] = n->child[1] = NULL; \
    n->parent = q; \
    if (q != NULL) { \
        if (q->child[0] && dir == 0 && q->child[0] == tree->header) \
            leftmost = 1; \
        else if (q->child[1] && dir == 1 && \
                 q->child[1] == tree->header) \
            rightmost = 1; \
        q->child[dir] = n; \
    } else { \
        tree->root = n; \
        tree->root->child[0] = tree->header; \
        tree->root->child[1] = tree->header; \
        tree->header->child[0] = tree->root; \
        tree->header->child[1] = tree->root; \
    } \
    n->red = 1; \
    q = n; \
    for (;;) { \
        iterator_type f; /* parent of q */ \
        iterator_type g; /* Grandparent of q. */ \
        f = q->parent; \
        if (f == NULL || f->red == 0) \
            break; \
        g = f->parent; \
        if (g == NULL) \
            break; \
        if (g->child[0] == f) { \
            iterator_type y = g->child[1]; \
            if (CAG_IS_IT_TREE(y) && y->red) { \
                /* case 1: q's uncle is red */ \
                f->red = y->red = 0; \
                g->red = 1; \
                q = g; \
            } else { \
                iterator_type h; /* Great-grandparent of q. */ \
                h = g->parent; \
                if (h == NULL) \
                    h = (iterator_type) &tree->root; \
                if (f->child [1] == q) { \
                    /* Case 3: */ \
                    /* q is the right child of its parent */ \
                    f->child[1] = q->child[0]; \
                    q->child[0] = f ; \
                    g->child[0] = q; \
                    f->parent = q; \
                    if (CAG_IS_IT_TREE(f->child[1])) \
                        f->child[1]->parent = f; \
                    f = q; \
                } \
                /* case 2: q is the left child of its parent */ \
                g->red = 1; \
                f->red = 0; \
                g->child[0] = f->child[1]; \
                f->child[1] = g; \
                h->child[h->child[0] != g] = f; \
                f->parent = g->parent; \
                g->parent = f; \
                if (CAG_IS_IT_TREE(g->child[0])) \
                    g->child[0]->parent = g; \
                break; \
            } \
        } \
        else { \
            /* Right side rebalancing */ \
            iterator_type y = g->child[0]; \
            if (CAG_IS_IT_TREE(y) && y->red == 1) { \
                /* Case 1 in right side rebalancing */ \
                f->red = y->red = 0; \
                g->red = 1; \
                q = g; \
            } else { \
                iterator_type h; /* Great-grandparent of q. */ \
                h = g->parent; \
                if (h == NULL) \
                    h = (iterator_type) &tree->root; \
                if (f->child[0] == q) { \
                    /* Case 3 in right side rebalancing */ \
                    f->child[0] = q->child[1]; \
                    q->child[1] = f; \
                    g->child[1] = q; \
                    f->parent = q; \
                    if (CAG_IS_IT_TREE(f->child[0])) \
                        f->child[0]->parent = f; \
                    f = q; \
                } \
                /* Case 2 in right-side rebalancing */ \
                g->red = 1; \
                f->red = 0; \
                g->child[1] = f->child[0]; \
                f->child[0] = g; \
                h->child[h->child[0] != g] = f; \
                f->parent = g->parent; \
                g->parent = f; \
                if (CAG_IS_IT_TREE(g->child[1])) \
                    g->child[1]->parent = g; \
                break; \
            } \
        } \
    } \
    tree->root->red = 0; \
    if (leftmost || rightmost) { \
        dir = leftmost ? 0 : 1; \
        tree->header->child[dir] = n; \
        n->child[dir] = tree->header; \
    } \
    return n; \
}

/*! \brief Function declaration and definition for inserting into red-black
    tree. By value and by address versions supplied.
*/

#define CAG_DEC_INSERT_TREE(function, container, iterator_type, type) \
    iterator_type function(container *tree, const type element)


#define CAG_DEF_INSERT_TREE(function, container, iterator_type, type, \
                            cmp_func, val_adr, alloc_style, alloc_func) \
CAG_DEC_INSERT_TREE(function, container, iterator_type, type) \
{ \
    CAG_INSERT_TREE(container, iterator_type, type, \
                    cmp_func, val_adr, alloc_style, alloc_func, element); \
}


#define CAG_DEC_INSERTP_TREE(function, container, iterator_type, type) \
    iterator_type function(container *tree, type const *element)


#define CAG_DEF_INSERTP_TREE(function, container, iterator_type, type, \
                             cmp_func, val_adr, alloc_style, alloc_func) \
CAG_DEC_INSERTP_TREE(function, container, iterator_type, type) \
{ \
    CAG_INSERT_TREE(container, iterator_type, type, \
                    cmp_func, val_adr, alloc_style, alloc_func, *element); \
}

/*! \brief Function declaration and definition for *put* and *putp*. Every
    container needs a put function to have a uniform insertion mechanism
    used by some of the generic functions.
*/

#define CAG_DEC_PUT_TREE(function, container, iterator_type, type) \
    iterator_type function(container *tree, iterator_type it, \
                           type const element)


#define CAG_DEF_PUT_TREE(function, container, iterator_type, type) \
    CAG_DEC_PUT_TREE(function, container, iterator_type, type) \
    { \
        assert(it); \
        return insert_ ## container(tree, element); \
    }


#define CAG_DEC_PUTP_TREE(function, container, iterator_type, type) \
    iterator_type function(container *tree, iterator_type it, \
                           type const *element)


#define CAG_DEF_PUTP_TREE(function, container, iterator_type, type) \
    CAG_DEC_PUTP_TREE(function, container, iterator_type, type) \
    { \
        assert(it); \
        return insertp_ ## container(tree, element); \
    }


/*! \brief Erase macro and function adapted from: An Introduction to Binary
   Search Trees and Balanced Trees by Ben Pfaff
   ftp://ftp.gnu.org/pub/gnu/avl/avl-2.0.2.pdf.gz and the corresponding code in
   libavl.
 */

#define CAG_P_ERASE_TREE(container, tree, p, iterator_type, \
                         cmp_func, val_adr, next) \
do { \
    iterator_type q; /* Parent of |p|. */ \
    iterator_type f; /* Node at which we are rebalancing. */ \
    iterator_type n; /* Next node - the one to return. */ \
    int dir;            /* Side of |q| on which |p| is a child; */ \
    /*   side of |f| from which node was deleted. */ \
    int leftmost  = 0; \
    int rightmost = 0; \
    assert(tree != NULL); \
    assert(p != NULL); \
    n = next(p); \
    if (p->parent && p == p->parent->child[1]) \
        dir = 1; \
    else \
        dir = 0; \
    if (p->child[0] == tree->header) { \
        leftmost = 1; \
    } \
    if (p->child[1] == tree->header) { \
        rightmost = 1; \
    } \
    q = p->parent; \
    if (q == NULL) { \
        q = (iterator_type) &tree->root; \
        dir = 0; \
    } \
    /* First insert by finding successor */ \
    if (CAG_IS_IT_TREE(p->child[1]) == 0) { \
        /* no right child */ \
        q->child[dir] = p->child[0]; \
        if (CAG_IS_IT_TREE(q->child[dir])) \
            q->child[dir]->parent = p->parent; \
        f = q; \
    } else { \
        unsigned char t; \
        iterator_type r = p->child[1]; \
        if (r->child[0] == NULL) { \
            /* Right child has no left child. */ \
            r->child[0] = p->child[0]; \
            q->child[dir] = r; \
            r->parent = p->parent; \
            if (CAG_IS_IT_TREE(r->child[0])) \
                r->child[0]->parent = r; \
            t = p->red; \
            p->red = r->red; \
            r->red = t; \
            f = r; \
            dir = 1; \
        } else { \
            /* Right child has left child */ \
            iterator_type s = r->child[0]; \
            while (s->child[0]) \
                s = s->child[0]; \
            r = s->parent; \
            r->child[0] = s->child[1]; \
            s->child[0] = p->child[0]; \
            s->child[1] = p->child[1]; \
            q->child[dir] = s; \
            if (s->child[0] != NULL) \
                s->child[0]->parent = s; \
            s->child[1]->parent = s; \
            s->parent = p->parent; \
            if (r->child[0]) \
                r->child[0]->parent = r; \
            t = p->red; \
            p->red = s->red; \
            s->red = t; \
            f = r; \
            dir = 0; \
        } \
    } \
    /* Rebalancing */ \
    /* We only have to rebalance if p was a black node. */ \
    if (p->red == 0) { \
        for (;;) { \
            /* Node we want to recolor black if possible. */ \
            iterator_type x; \
            /* Parent of |f|. */ \
            iterator_type g; \
            /* Temporary for use in finding parent. */ \
            iterator_type t; \
            x = f->child[dir]; \
            if (CAG_IS_IT_TREE(x) && x->red)  { \
                x->red = 0; \
                break; \
            } \
            if (f == (iterator_type) &tree->root) \
                break; \
            g = f->parent; \
            if (g == NULL) \
                g = (iterator_type) &tree->root; \
            if (dir == 0) { \
                /* left rebalancing of tree */ \
                iterator_type w = f->child[1]; \
                if (w->red) { \
                    w->red = 0; \
                    f->red = 1; \
                    f->child[1] = w->child[0]; \
                    w->child[0] = f; \
                    g->child[g->child[0] != f] = w; \
                    w->parent = f->parent; \
                    f->parent = w; \
                    g = w; \
                    w = f->child[1]; \
                    w->parent = f; \
                } \
                if ((w->child[0] == NULL || \
                        w->child[0]->red == 0) \
                        && (w->child[1] == NULL || \
                            w->child[1]->red == 0)) { \
                    w->red = 1; \
                } else { \
                    if (CAG_IS_IT_TREE(w->child[1]) == 0 || \
                            w->child[1]->red == 0) { \
                        iterator_type y = w->child[0]; \
                        y->red = 0; \
                        w->red = 1; \
                        w->child[0] = y->child[1]; \
                        y->child[1] = w; \
                        if (w->child[0]) \
                            w->child[0]->parent = w; \
                        w = f->child[1] = y; \
                        w->child[1]->parent = w; \
                    } \
                    w->red = f->red; \
                    f->red = 0; \
                    w->child[1]->red = 0; \
                    f->child[1] = w->child[0]; \
                    w->child[0] = f; \
                    g->child[g->child[0] != f] = w; \
                    w->parent = f->parent; \
                    f->parent = w; \
                    if (f->child[1] != NULL) \
                        f->child[1]->parent = f; \
                    break; \
                } \
            } else { \
                iterator_type w = f->child[0]; \
                if (w->red == 1) { \
                    w->red = 0; \
                    f->red = 1; \
                    f->child[0] = w->child[1]; \
                    w->child[1] = f; \
                    g->child[g->child[0] != f] = w; \
                    w->parent = f->parent; \
                    f->parent = w; \
                    g = w; \
                    w = f->child[0]; \
                    w->parent = f; \
                } \
                if ((CAG_IS_IT_TREE(w->child[0]) == 0 || \
                        w->child[0]->red == 0) \
                        && (w->child[1] == NULL || \
                            w->child[1]->red == 0)) { \
                    w->red = 1; \
                } else { \
                    if (CAG_IS_IT_TREE(w->child[0]) == 0 || \
                            w->child[0]->red == 0) { \
                        iterator_type y = w->child[1]; \
                        y->red = 0; \
                        w->red = 1; \
                        w->child[1] = y->child[0]; \
                        y->child[0] = w; \
                        if (w->child[1] != NULL) \
                            w->child[1]->parent = w; \
                        w = f->child[0] = y; \
                        w->child[0]->parent = w; \
                    } \
                    w->red = f->red; \
                    f->red = 0; \
                    w->child[0]->red = 0; \
                    f->child[0] = w->child[1]; \
                    w->child[1] = f; \
                    g->child[g->child[0] != f] = w; \
                    w->parent = f->parent; \
                    f->parent = w; \
                    if (f->child[0] != NULL) \
                        f->child[0]->parent = f; \
                    break; \
                } \
            } \
            t = f; \
            f = f->parent; \
            if (f == NULL) \
                f = (iterator_type) &tree->root; \
            dir = f->child[0] != t; \
        } \
    } \
    CAG_FREE(p); \
    if (leftmost && rightmost) { \
        tree->root = NULL; \
        tree->header->parent = tree->header; \
        tree->header->child[0] = tree->header; \
        tree->header->child[1] = tree->header; \
        tree->header->red = 0; \
    } \
    else if (leftmost || rightmost) { \
        dir = leftmost ? 0 : 1; \
        p = tree->root; \
        while(CAG_IS_IT_TREE(p->child[dir])) \
            p = p->child[dir]; \
        tree->header->child[dir] = p; \
        p->child[dir] = tree->header; \
    } \
    return n; \
} while(0)

#define CAG_DEC_ERASE_TREE(function, container, iterator_type, type) \
    iterator_type function(container *tree, iterator_type it)

#define CAG_DEF_ERASE_TREE(function, container, iterator_type, type, \
                           cmp_func, val_adr, next) \
CAG_DEC_ERASE_TREE(function, container, iterator_type, type) \
{ \
    CAG_P_ERASE_TREE(container, tree, it, iterator_type, cmp_func, \
                     val_adr, next); \
}

/*! \brief Function declaration and definition for removing an element from the
    tree. This differs from *erase* in that it takes an element, not an iterator
    as its parameter. It then searches for the element and calls *erase* with
    the iterator that has been found.
*/

#define CAG_DEC_REMOVE_TREE(function, container, iterator_type, type) \
    iterator_type function(container *tree, const type element)

#define CAG_DEF_REMOVE_TREE(function, container, iterator_type, type) \
    CAG_DEC_REMOVE_TREE(function, container, iterator_type, type) \
    { \
        iterator_type n = get_ ## container(tree, element); \
        if (n) \
            return erase_ ## container(tree, n); \
        else \
            return NULL; \
    }

#define CAG_DEC_REMOVEP_TREE(function, container, iterator_type, type) \
    iterator_type function(container *tree, const type *element)

#define CAG_DEF_REMOVEP_TREE(function, container, iterator_type, type) \
    CAG_DEC_REMOVEP_TREE(function, container, iterator_type, type) \
    { \
        iterator_type n = getp_ ## container(tree, element); \
        if (n) \
            return erase_ ## container(tree, n); \
        else \
            return NULL; \
    }


/*! \brief Function declaration and definition to calculate distance between two
   iterators in a list.
*/

#define CAG_DEC_DISTANCE_TREE(function, iterator_type) \
    size_t function(const iterator_type from, \
                    const iterator_type to)

#define CAG_DEF_DISTANCE_TREE(function, iterator_type, next) \
    CAG_DEC_DISTANCE_TREE(function, iterator_type) \
    { \
        iterator_type it; \
        size_t count = 0; \
        for (it = from; it != to; it = next(it)) \
            ++count; \
        return count; \
    }

/*! \brief Function declaration and definitions for returning tree to heap.
    Uses a recursive implementation. Should possibly be changed to a
    non-recursive one to be consistent with rest of CAGL, but irrelevant to
    users.
*/

#define CAG_DEC_FREE_TREE(function, container) \
    void function(container *tree)

#define CAG_P_DEC_FREE_NODE(function, container, iterator_type) \
    void function(container *tree, iterator_type it)

#define CAG_P_DEF_FREE_NODE(function, container, iterator_type, \
                          free_func, val_adr) \
CAG_P_DEC_FREE_NODE(function, container, iterator_type) \
{ \
    if (it->child[0] && it->child[0] != tree->header) \
        free_node_p_ ## container(tree, it->child[0]); \
    if (it->child[1] && it->child[1] != tree->header) \
        free_node_p_ ##container(tree, it->child[1]); \
    free_func(val_adr it->value); \
    CAG_FREE(it); \
}

#define CAG_DEF_FREE_TREE(function, container) \
    CAG_DEC_FREE_TREE(function, container) \
    { \
        if (tree->root) \
            free_node_p_ ## container(tree, tree->root); \
        CAG_FREE(tree->header); \
    }


/* \brief Integrity checker adapted from
 http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
 This is just for testing purposes.
*/

#define CAG_DEC_CHECK_INTEGRITY_TREE(function, container, iterator_type) \
    int function(const container *tree, iterator_type it)

#define CAG_DEF_CHECK_INTEGRITY_TREE(function, container, iterator_type, \
                                     cmp_func, val_adr) \
CAG_DEC_CHECK_INTEGRITY_TREE(function, container, iterator_type) \
{ \
    int lh, rh; \
    if (it == NULL || it == tree->header) \
        return 1; \
    else { \
        const iterator_type ln = it->child[0]; \
        const iterator_type rn = it->child[1]; \
        /* Consecutive red links */ \
        if ( CAG_IS_RED_TREE(it)) { \
            if (CAG_IS_RED_TREE(ln)  || CAG_IS_RED_TREE(rn)) { \
                puts ( "Red violation" ); \
                return 0; \
            } \
        } \
        lh = function (tree, ln); \
        rh = function (tree, rn); \
        /* Invalid binary search tree */ \
        if (ln != NULL && ln != tree->header && \
                cmp_func(val_adr (ln->value), val_adr (it->value)) >= 0) \
        { \
            puts ( "Binary tree violation on left" ); \
            printf("Keys: it %p, ln %p\n", \
                   (void *) it, (void *) ln); \
            return 0; \
        } \
        if (rn != NULL && rn != tree->header && \
                cmp_func(val_adr (rn->value), val_adr (it->value)) <= 0) \
        { \
            puts ( "Binary tree violation on right" ); \
            printf("Keys: it %p, ln %p\n", \
                   (void *) it, (void *) rn); \
            return 0; \
        } \
        /* Black height mismatch */ \
        if ( lh != 0 && rh != 0 && lh != rh ) { \
            printf("Black violation %d %d\n", lh, rh); \
            return 0; \
        } \
        /* Only count black links */ \
        if ( lh != 0 && rh != 0 ) \
            return it->red ? lh : lh + 1; \
        else \
            return 0; \
    } \
}

/*! \brief Function declaration and definitions for *inorder*, *preorder*,
    *post-order* and *levelorder* traversals of the tree. All implemented
    recursively except *levelorder*.
*/

#define CAG_DEC_INORDER_TREE(function, iterator_type) \
    void function(iterator_type root, \
                  void *data, void (*action)(iterator_type, void *))

#define CAG_DEF_INORDER_TREE(function, iterator_type, dir) \
    CAG_DEC_INORDER_TREE(function, iterator_type) \
    { \
        if (root == NULL || (iterator_type) root->parent == root) \
            return; \
        function((iterator_type) root->child[dir], data, action); \
        action(root, data); \
        function((iterator_type) root->child[!dir], data, action); \
    }

#define CAG_DEC_PREORDER_TREE(function, iterator_type) \
    void function(iterator_type root, \
                  void *data, void (*action)(iterator_type, void *))

#define CAG_DEF_PREORDER_TREE(function, iterator_type) \
    CAG_DEC_PREORDER_TREE(function, iterator_type) \
    { \
        if (CAG_IS_IT_TREE(root) == 0) \
            return; \
        action(root, data); \
        function(root->child[0], data, action); \
        function(root->child[1], data, action); \
    }

#define CAG_DEC_POSTORDER_TREE(function, iterator_type) \
    void function(iterator_type root, \
                  void *data, void (*action)(iterator_type, void *))

#define CAG_DEF_POSTORDER_TREE(function, iterator_type) \
    CAG_DEC_POSTORDER_TREE(function, iterator_type) \
    { \
        if (CAG_IS_IT_TREE(root) == 0) \
            return; \
        function(root->child[0], data, action); \
        function(root->child[1], data, action); \
        action(root, data); \
    }

#define CAG_DEC_LEVELORDER_TREE(function, container, iterator_type) \
    void function(container *tree, \
                  void *data, \
                  void (*action)(iterator_type, size_t, int, int, void *))

#define CAG_DEF_LEVELORDER_TREE(function, container, iterator_type) \
    CAG_DEC_LEVELORDER_TREE(function, container, iterator_type) \
    { \
        struct nodelevel { \
            iterator_type node; \
            size_t level; \
        }; \
        struct nodelevel *queue; \
        struct nodelevel entry; \
        iterator_type prev_parent = NULL; \
        size_t front = 0, back = 0, prev_level; \
        int level_changed = 0, parent_changed = 0; \
        if (!tree->root) return; \
        queue = CAG_MALLOC(distance_ ## container( \
                           begin_ ## container(tree), \
                           end_ ## container(tree)) * sizeof(*queue)); \
        if (!queue) return; \
        entry.node = tree->root; \
        entry.level = prev_level = 0; \
        queue[back++] = entry; \
        while(front != back) { \
            iterator_type node; \
            size_t level; \
            entry = queue[front++]; \
            node = entry.node; \
            level = entry.level; \
            if (level != prev_level) \
                level_changed = 1; \
            else \
                level_changed = 0; \
            if (node->parent != prev_parent) \
                parent_changed = 1; \
            else \
                parent_changed = 0; \
            prev_parent = node->parent; \
            prev_level = level; \
            action(node, level, level_changed, parent_changed, data); \
            if(CAG_IS_IT_TREE(node->child[0])) { \
                entry.node = node->child[0]; \
                entry.level = level + 1; \
                queue[back++] = entry; \
            } \
            if(CAG_IS_IT_TREE(node->child[1])) { \
                entry.node = node->child[1]; \
                entry.level = level + 1; \
                queue[back++] = entry; \
            } \
        } \
        CAG_FREE(queue); \
    }

/*! \brief Definitions of tree iterator and functions. */

#define CAG_DEC_CMP_TREE(container, type) \
    struct iterator_ ## container { \
        struct iterator_ ## container *child[2]; \
        struct iterator_ ## container *parent; \
        unsigned char red; \
        type value; \
    }; \
    struct reverse_iterator_ ## container { \
        struct iterator_ ## container *child[2]; \
        struct iterator_ ## container *parent; \
        unsigned char red; \
        type value; \
    }; \
    typedef struct value_ ## container value_ ## container; \
    typedef struct iterator_ ## container iterator_ ## container; \
    typedef struct reverse_iterator_ ## container \
            reverse_iterator_ ## container; \
    typedef iterator_ ## container * it_ ## container; \
    typedef reverse_iterator_ ## container * rit_ ## container; \
    struct container { \
        it_ ## container root; \
        it_ ## container header; \
    }; \
    typedef struct container container; \
    CAG_DEC_NEW_TREE(new_ ## container, container); \
    CAG_DEC_BEGIN_TREE(begin_ ## container, container, it_ ## container); \
    CAG_DEC_BEGIN_TREE(rbegin_ ## container, container, rit_ ## container); \
    CAG_DEC_END_TREE(end_ ## container, container, it_ ## container); \
    CAG_DEC_END_TREE(rend_ ## container, container, rit_ ## container); \
    CAG_DEC_BOUNDARY_TREE(front_ ## container, container, type); \
    CAG_DEC_BOUNDARY_TREE(back_ ## container, container, type); \
    CAG_DEC_STEP_TREE(next_ ## container, it_ ## container); \
    CAG_DEC_STEP_TREE(prev_ ## container, it_ ## container); \
    CAG_DEC_STEP_TREE(rnext_ ## container, rit_ ## container); \
    CAG_DEC_STEP_TREE(rprev_ ## container, rit_ ## container); \
    CAG_DEC_AT(at_ ## container, container, it_ ## container); \
    CAG_DEC_AT(rat_ ## container, container, rit_ ## container); \
    CAG_DEC_DISTANCE_TREE(distance_ ## container, it_ ## container); \
    CAG_DEC_DISTANCE_TREE(rdistance_ ## container, rit_ ## container); \
    CAG_DEC_INSERT_TREE(insert_ ## container, container, \
                        it_ ## container, type); \
    CAG_DEC_INSERTP_TREE(insertp_ ## container, container, \
                         it_ ## container, type); \
    CAG_DEC_PUT_TREE(put_ ## container, container, \
                     it_ ## container, type); \
    CAG_DEC_PUTP_TREE(putp_ ## container, container, \
                      it_ ## container, type); \
    CAG_DEC_PREORDER_TREE(preorder_ ## container, it_ ## container); \
    CAG_DEC_INORDER_TREE(inorder_ ## container, it_ ## container); \
    CAG_DEC_INORDER_TREE(reverseorder_ ## container, rit_ ## container); \
    CAG_DEC_POSTORDER_TREE(postorder_ ## container, it_ ## container); \
    CAG_DEC_LEVELORDER_TREE(levelorder_ ## container, container, \
                            it_ ## container); \
    CAG_DEC_GET_TREE(get_ ## container, container, type); \
    CAG_DEC_GETP_TREE(getp_ ## container, container, type); \
    CAG_DEC_ERASE_TREE(erase_ ## container, container, \
                       it_ ## container, type); \
    CAG_DEC_ERASE_RANGE(erase_range_ ## container, container, \
                        it_ ## container); \
    CAG_DEC_REMOVE_TREE(remove_ ## container, container, \
                        it_ ## container, type); \
    CAG_DEC_REMOVEP_TREE(removep_ ## container, container, \
                         it_ ## container, type); \
    CAG_P_DEC_FREE_NODE(free_node_p_## container, container, it_ ## container); \
    CAG_DEC_FREE_TREE(free_## container, container); \
    CAG_DEC_CHECK_INTEGRITY_TREE(check_integrity_ ## container, container, \
                                 it_ ## container); \
    CAG_DEC_BIDIRECTIONAL(container, type); \
    CAG_DEC_CMP_BIDIRECTIONAL(container, type)


/*! \brief Definitions of tree functions. */

#define CAG_DEF_ALL_CMP_TREE(container, type, cmp_func, val_adr, \
                             alloc_style, alloc_func, free_func) \
CAG_DEF_NEW_TREE(new_ ## container, container) \
CAG_DEF_BEGIN_TREE(begin_ ## container, container, it_ ## container, 0) \
CAG_DEF_BEGIN_TREE(rbegin_ ## container, container, rit_ ## container, 1) \
CAG_DEF_END_TREE(end_ ## container, container, it_ ## container) \
CAG_DEF_END_TREE(rend_ ## container, container, rit_ ## container) \
CAG_DEF_STEP_TREE(next_ ## container, 1, it_ ## container) \
CAG_DEF_STEP_TREE(rnext_ ## container, 0, rit_ ## container) \
CAG_DEF_STEP_TREE(prev_ ## container, 0, it_ ## container) \
CAG_DEF_STEP_TREE(rprev_ ## container, 1, rit_ ## container) \
CAG_DEF_BOUNDARY_TREE(front_ ## container, container, type, 0) \
CAG_DEF_BOUNDARY_TREE(back_ ## container, container, type, 1) \
CAG_DEF_AT(at_ ## container, container, \
           it_ ## container, next_ ## container) \
CAG_DEF_AT(rat_ ## container, container, \
           rit_ ## container, rnext_ ## container) \
CAG_DEF_DISTANCE_TREE(distance_ ## container, it_ ## container, \
                      next_ ## container) \
CAG_DEF_DISTANCE_TREE(rdistance_ ## container, rit_ ## container, \
                      rnext_ ## container) \
CAG_DEF_INSERT_TREE(insert_ ## container, container, it_ ## container, \
                    type, cmp_func, val_adr, alloc_style, alloc_func) \
CAG_DEF_INSERTP_TREE(insertp_ ## container, container, it_ ## container, \
                     type, cmp_func, val_adr, alloc_style, alloc_func) \
CAG_DEF_PUT_TREE(put_ ## container, container, \
                 it_ ## container, type) \
CAG_DEF_PUTP_TREE(putp_ ## container, container, \
                 it_ ## container, type) \
CAG_DEF_PREORDER_TREE(preorder_ ## container, it_ ## container) \
CAG_DEF_INORDER_TREE(inorder_ ## container, it_ ## container, 0) \
CAG_DEF_INORDER_TREE(reverseorder_ ## container, rit_ ## container, 1) \
CAG_DEF_POSTORDER_TREE(postorder_ ## container, it_ ## container) \
CAG_DEF_LEVELORDER_TREE(levelorder_ ## container, container, \
                        it_ ## container) \
CAG_DEF_GET_TREE(get_ ## container, container, \
                 type, cmp_func, val_adr) \
CAG_DEF_GETP_TREE(getp_ ## container, container, \
                  type, cmp_func, val_adr) \
CAG_DEF_ERASE_TREE(erase_ ## container, container, it_ ## container, \
                   type, cmp_func, val_adr, next_ ## container) \
CAG_DEF_ERASE_RANGE(erase_range_ ## container, container, \
                    it_ ## container, erase_ ## container, CAG_NO_OP_3) \
CAG_DEF_REMOVE_TREE(remove_ ## container, container, \
                    it_ ## container, type) \
CAG_DEF_REMOVEP_TREE(removep_ ## container, container, \
                     it_ ## container, type) \
CAG_P_DEF_FREE_NODE(free_node_p_## container, container, it_ ## container, \
                  free_func, val_adr) \
CAG_DEF_FREE_TREE(free_ ## container, container) \
CAG_DEF_CHECK_INTEGRITY_TREE(check_integrity_ ## container, container, \
                             it_ ## container, cmp_func, val_adr) \
CAG_DEF_BIDIRECTIONAL(container, type) \
CAG_DEF_CMP_BIDIRECTIONAL(container, type, cmp_func, val_adr) \
typedef container CAG_P_CMB(container,  __LINE__)


#define CAG_DEC_DEF_ALL_CMP_TREE(container, type, cmp_func, val_adr, \
                                 alloc_style, alloc_func, free_func) \
CAG_DEC_CMP_TREE(container, type); \
CAG_DEF_ALL_CMP_TREE(container, type, cmp_func, val_adr, \
                     alloc_style, alloc_func, free_func)

/*! \brief Definition of common case tree, in which memory of elements is not
 *  managed by CAGL.
*/

#define CAG_DEF_CMP_TREE(container, type, cmp_func) \
    CAG_DEF_ALL_CMP_TREE(container, type, cmp_func, CAG_BYVAL, \
                         CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC)


/*! \brief Identical to CAG_DEC_CMP_TREE but provided for users who
    want consistent names.
*/

#define CAG_DEC_CMPP_TREE CAG_DEC_CMP_TREE


/*! \brief Same as CAG_DEF_CMP_TREE but cmp_fun takes parameters by address. */

#define CAG_DEF_CMPP_TREE(container, type, cmp_func) \
    CAG_DEF_ALL_CMP_TREE(container, type, cmp_func, CAG_BYADR, \
                         CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC)

/*! \brief Used to declare and define a tree in one macro.  Useful
   usually for small programs where the container will only be used in one
   module.
*/

#define CAG_DEC_DEF_CMP_TREE(container, type, cmp_func) \
    CAG_DEC_CMP_TREE(container, type); \
    CAG_DEF_CMP_TREE(container, type, cmp_func)

#define CAG_DEC_DEF_CMPP_TREE(container, type, cmp_func) \
    CAG_DEC_CMPP_TREE(container, type); \
    CAG_DEF_CMPP_TREE(container, type, cmp_func)


/*! \brief Declare and define macros for a tree  whose elements are structs
   composed of two strings.  This is a common use-case, e.g. for a dictionary
   made up of words (the keys) and definitions.
*/


#define CAG_DEC_STR_STR_TREE(container, type) \
    CAG_DEC_CMP_TREE(container, type)

#define CAG_DEF_STR_STR_TREE(container, type) \
    CAG_DEF_ALL_CMP_TREE(container, type, CAG_STRCMP_STRUCT_WITH_STR_KEY, \
                         CAG_BYVAL, CAG_STRUCT_ALLOC_STYLE, \
                         cag_alloc_str_str, CAG_FREE_STRUCT_STR_STR)

#define CAG_DEC_DEF_STR_STR_TREE(container, type) \
    CAG_DEC_STR_STR_TREE(container, type); \
    CAG_DEF_STR_STR_TREE(container, type)


/*! \brief Declare and define macros for a tree whose elements are C
   strings.  This is a common use-case, e.g. a list of words.
*/


#define CAG_DEC_STR_TREE(container) \
    CAG_DEC_CMP_TREE(container, char *)

#define CAG_DEF_STR_TREE(container) \
    CAG_DEF_ALL_CMP_TREE(container, char *, strcmp, CAG_BYVAL, \
                         CAG_SIMPLE_ALLOC_STYLE, cag_strdup, free)

#define CAG_DEC_DEF_STR_TREE(container) \
    CAG_DEC_STR_TREE(container); \
    CAG_DEF_STR_TREE(container)



#endif /* CAG_TREE_H */
