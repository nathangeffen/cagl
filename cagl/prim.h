/*! GDA primitive type containers.

    \copyright Copyright 2013 University of Cape Town. All rights reserved.
    \license This code is licensed under the FreeBSD Copyright.
    See COPYING for the license text.

    This is code that is used throughout the GDA library.
*/

#ifndef CAG_PRIMITIVE_H
#define CAG_PRIMITIVE_H

#include "cagl/common.h"
#include "cagl/dlist.h"
#include "cagl/array.h"

#define CAG_AT_DEFAULT(a, i) a[i]

CAG_DEC_CMP_DLIST(cag_char_dlist, char);
CAG_DEC_CMP_DLIST(cag_uchar_dlist, unsigned char);
CAG_DEC_CMP_DLIST(cag_schar_dlist, signed char);
CAG_DEC_CMP_DLIST(cag_int_dlist, int);
CAG_DEC_CMP_DLIST(cag_uint_dlist, unsigned);
CAG_DEC_CMP_DLIST(cag_long_dlist, long);
CAG_DEC_CMP_DLIST(cag_ulong_dlist, unsigned long);
CAG_DEC_CMP_DLIST(cag_float_dlist, float);
CAG_DEC_CMP_DLIST(cag_double_dlist, double);
CAG_DEC_CMP_DLIST(cag_longdouble_dlist, long double);

#if __STDC_VERSION__ >= 199901L
CAG_DEC_CMP_DLIST(cag_bool_dlist, _Bool);
CAG_DEC_CMP_DLIST(cag_longlong_dlist, long long);
CAG_DEC_CMP_DLIST(cag_ulonglong_dlist, unsigned long long);
#endif

CAG_DEC_CMP_ARRAY(cag_char_array, char);
CAG_DEC_CMP_ARRAY(cag_uchar_array, unsigned char);
CAG_DEC_CMP_ARRAY(cag_schar_array, signed char);
CAG_DEC_CMP_ARRAY(cag_int_array, int);
CAG_DEC_CMP_ARRAY(cag_uint_array, unsigned);
CAG_DEC_CMP_ARRAY(cag_long_array, long);
CAG_DEC_CMP_ARRAY(cag_ulong_array, unsigned long);
CAG_DEC_CMP_ARRAY(cag_float_array, float);
CAG_DEC_CMP_ARRAY(cag_double_array, double);
CAG_DEC_CMP_ARRAY(cag_longdouble_array, long double);

#if __STDC_VERSION__ >= 199901L
CAG_DEC_CMP_ARRAY(cag_bool_array, _Bool);
CAG_DEC_CMP_ARRAY(cag_longlong_array, long long);
CAG_DEC_CMP_ARRAY(cag_ulonglong_array, unsigned long long);
#endif


#endif /* CAG_PRIMITIVE_H */
