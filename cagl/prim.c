#include "cagl/prim.h"

CAG_DEF_CMP_DLIST(cag_char_dlist, char, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_uchar_dlist, unsigned char, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_schar_dlist, signed char, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_int_dlist, int, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_uint_dlist, unsigned, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_long_dlist, long, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_ulong_dlist, unsigned long, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_float_dlist, float, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_double_dlist, double, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_longdouble_dlist, long double, CAG_CMP_PRIMITIVE);

#if __STDC_VERSION__ >= 199901L
CAG_DEF_CMP_DLIST(cag_bool_dlist, _Bool, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_longlong_dlist, long long, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_DLIST(cag_ulonglong_dlist, unsigned long long, CAG_CMP_PRIMITIVE);
#endif


CAG_DEF_CMP_ARRAY(cag_char_array, char, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_uchar_array, unsigned char, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_schar_array, signed char, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_int_array, int, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_uint_array, unsigned, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_long_array, long, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_ulong_array, unsigned long, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_float_array, float, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_double_array, double, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_longdouble_array, long double, CAG_CMP_PRIMITIVE);

#if __STDC_VERSION__ >= 199901L
CAG_DEF_CMP_ARRAY(cag_bool_array, _Bool, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_longlong_array, long long, CAG_CMP_PRIMITIVE);
CAG_DEF_CMP_ARRAY(cag_ulonglong_array, unsigned long long, CAG_CMP_PRIMITIVE);
#endif
