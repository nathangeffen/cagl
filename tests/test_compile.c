#include <cagl/array.h>
#include <cagl/dlist.h>
#include <cagl/slist.h>
#include <cagl/hash.h>
#include <cagl/tree.h>

CAG_DEC_ARRAY(int_a, int);
CAG_DEF_ARRAY(int_a, int);

CAG_DEC_DLIST(int_d, int);
CAG_DEF_DLIST(int_d, int);

CAG_DEC_SLIST(int_s, int);
CAG_DEF_SLIST(int_s, int);

CAG_DEC_DEF_ARRAY(int_dd_a, int);
CAG_DEC_DEF_DLIST(int_dd_d, int);
CAG_DEC_DEF_SLIST(int_dd_s, int);

CAG_DEC_CMP_ARRAY(int_ca, int);
CAG_DEF_CMP_ARRAY(int_ca, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMP_DLIST(int_cd, int);
CAG_DEF_CMP_DLIST(int_cd, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMP_SLIST(int_cs, int);
CAG_DEF_CMP_SLIST(int_cs, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMP_HASH(int_ch, int);
CAG_DEF_CMP_HASH(int_ch, int, CAG_CMP_PRIMITIVE, cag_int_hash, sizeof);

CAG_DEC_CMP_TREE(int_ct, int);
CAG_DEF_CMP_TREE(int_ct, int, CAG_CMP_PRIMITIVE);

CAG_DEC_DEF_CMP_ARRAY(int_dd_ca, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMP_DLIST(int_dd_cd, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMP_SLIST(int_dd_cs, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMP_HASH(int_dd_ch, int, CAG_CMP_PRIMITIVE, cag_int_hash, sizeof);
CAG_DEC_DEF_CMP_TREE(int_dd_ct, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMPP_ARRAY(int_pca, int);
CAG_DEF_CMPP_ARRAY(int_pca, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMPP_DLIST(int_pcd, int);
CAG_DEF_CMPP_DLIST(int_pcd, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMPP_SLIST(int_pcs, int);
CAG_DEF_CMPP_SLIST(int_pcs, int, CAG_CMP_PRIMITIVE);

CAG_DEC_CMPP_HASH(int_pch, int);
CAG_DEF_CMPP_HASH(int_pch, int, CAG_CMP_PRIMITIVE, cag_int_hash, sizeof);

CAG_DEC_CMPP_TREE(int_pct, int);
CAG_DEF_CMPP_TREE(int_pct, int, CAG_CMP_PRIMITIVE);

CAG_DEC_DEF_CMPP_ARRAY(int_pdd_ca, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMPP_DLIST(int_pdd_cd, int, CAG_CMP_PRIMITIVE);

CAG_DEC_DEF_CMPP_SLIST(int_pdd_cs, int, CAG_CMP_PRIMITIVE);
CAG_DEC_DEF_CMPP_HASH(int_pdd_ch, int, CAG_CMP_PRIMITIVE, cag_int_hash, sizeof);

CAG_DEC_DEF_CMPP_TREE(int_pdd_ct, int, CAG_CMP_PRIMITIVE);

CAG_DEC_ARRAY(int_aa, int);
CAG_DEF_ALL_ARRAY(int_aa, int, CAG_NO_ALLOC_STYLE,
		  CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);

CAG_DEC_DLIST(int_ad, int);
CAG_DEF_ALL_DLIST(int_ad, int, CAG_NO_ALLOC_STYLE,
		  CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);

CAG_DEC_SLIST(int_as, int);
CAG_DEF_ALL_SLIST(int_as, int, CAG_NO_ALLOC_STYLE,
		  CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);

CAG_DEC_DEF_ALL_ARRAY(int_dd_aa, int, CAG_NO_ALLOC_STYLE,
		      CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);
CAG_DEC_DEF_ALL_DLIST(int_dd_ad, int, CAG_NO_ALLOC_STYLE,
		      CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);
CAG_DEC_DEF_ALL_SLIST(int_dd_as, int, CAG_NO_ALLOC_STYLE,
		      CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC, CAG_BYVAL);

CAG_DEC_CMP_ARRAY(int_caa, int);
CAG_DEF_ALL_CMP_ARRAY(int_caa, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
		      CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
		      CAG_NO_FREE_FUNC);

CAG_DEC_CMP_DLIST(int_cad, int);
CAG_DEF_ALL_CMP_DLIST(int_cad, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
		      CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
		      CAG_NO_FREE_FUNC);

CAG_DEC_SLIST(int_cas, int);
CAG_DEF_ALL_CMP_SLIST(int_cas, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
		      CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
		      CAG_NO_FREE_FUNC);

CAG_DEC_CMP_HASH(int_cas_ah, int);
CAG_DEF_ALL_CMP_HASH(int_cas_ah, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				 cag_int_hash, sizeof,
		     CAG_NO_ALLOC_STYLE,
		     CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC);

CAG_DEC_CMP_TREE(int_cas_at, int);
CAG_DEF_ALL_CMP_TREE(int_cas_at, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
		     CAG_NO_ALLOC_STYLE,
		     CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC);

CAG_DEC_DEF_ALL_CMP_ARRAY(int_cdd_aa, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				  CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
				  CAG_NO_FREE_FUNC);
CAG_DEC_DEF_ALL_CMP_DLIST(int_cdd_ad, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				  CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
				  CAG_NO_FREE_FUNC);
CAG_DEC_DEF_ALL_CMP_SLIST(int_cdd_as, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				  CAG_NO_ALLOC_STYLE, CAG_NO_ALLOC_FUNC,
				  CAG_NO_FREE_FUNC);
CAG_DEC_DEF_ALL_CMP_HASH(int_cdd_ah, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				 cag_int_hash, sizeof,
				 CAG_NO_ALLOC_STYLE,
				 CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC);
CAG_DEC_DEF_ALL_CMP_TREE(int_cdd_at, int, CAG_CMP_PRIMITIVE, CAG_BYVAL,
				 CAG_NO_ALLOC_STYLE,
				 CAG_NO_ALLOC_FUNC, CAG_NO_FREE_FUNC);

CAG_DEC_STR_HASH(sh);
CAG_DEF_STR_HASH(sh);
CAG_DEC_DEF_STR_HASH(ddsh);

CAG_DEC_STR_TREE(st);
CAG_DEF_STR_TREE(st);
CAG_DEC_DEF_STR_TREE(ddst);

struct dictionary {
	char *w;
	char *d;
};

CAG_DEC_STR_STR_HASH(dh, struct dictionary);
CAG_DEF_STR_STR_HASH(dh, struct dictionary);
CAG_DEC_DEF_STR_STR_HASH(dddh, struct dictionary);

CAG_DEC_STR_STR_TREE(dt, struct dictionary );
CAG_DEF_STR_STR_TREE(dt, struct dictionary);
CAG_DEC_DEF_STR_STR_TREE(dddt, struct dictionary);



int main(void)
{
return 0;
}
