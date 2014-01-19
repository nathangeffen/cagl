# Frequently Asked Questions about CAGL

# How do I reduce the size of executables that use CAGL?

CAGL macros generate many functions for each container type. You will likely only use a fraction of them. Yet these functions increase the size of executables, sometimes substantially.

The executable size can be reduced with the *gcc* compiler by using the *-flto* option. When this option is used, the linker will remove unused functions from the executable.

# Why do some functions return values when they could be void?

All CAGL functions return values except the *free_C*. For some, like *next_C*, a return value is the natural way of doing it. But for many functions, the return value appears redundant.

The rationale was that all CAGL functions (except *free_C* and a few others like swap) return values to support the possibility of function chaining. However function chaining is unlikely to be a much-desired feature of CAGL and so it is worth discussing whether functions in future versions of CAGL should return values.

In some cases functions return NULL to indicate an error and non-NULL to indicate success. The non-NULL value might as well be a container or iterator that has been modified. It's worth discussing whether in future versions of CAGL these functions should rather just return 0 or 1.
