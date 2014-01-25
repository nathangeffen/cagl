/*! \file Error handling for CAGL library.

    \copyright Copyright 2014 Nathan Geffen. All rights reserved.

    \license This code is licensed under the GNU LESSER GENERAL PUBLIC LICENSE.

    \sa COPYING for the license text.

    \sa howtodev.rst to learn how this code works and how to modify it.
*/

#ifndef CAG_ERROR_H
#define CAG_ERROR_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifndef CAG_ERROR_LABEL

#define CAG_ERROR_LABEL error

#endif

void cag_fprintf(const char *fmt, ...);
extern FILE *cag_log;

#define _CAG_LOG_ cag_log==NULL ? stderr : cag_log

#ifdef NDEBUG
#define CAG_DEBUG_C89(M)

#if __STDC_VERSION__ >= 199901L
#define CAG_DEBUG(M, ...)
#endif

#else
#define CAG_DEBUG_C89(M) \
    { \
        fprintf(_CAG_LOG_, \
                "DEBUG %s:%d: ",__FILE__, __LINE__); \
        cag_fprintf M; \
    }

#if __STDC_VERSION__ >= 199901L
#define CAG_DEBUG(M, ...) \
    fprintf(_CAG_LOG_, "DEBUG %s: %d %s: " M "\n", \
            __FILE__, __LINE__, __func__, __VA_ARGS__);

#endif


#endif

#define CAG_CLEAN_ERRNO() (errno == 0 ? "None" : strerror(errno))

#define CAG_LOG_ERR(M) fprintf(_CAG_LOG_, "[ERROR] (%s:%d: errno: %s) " \
                               M "\n", __FILE__, __LINE__, \
                               CAG_CLEAN_ERRNO())

#define CAG_LOG_WARN(M) fprintf(_CAG_LOG_, "[WARN] (%s:%d: errno: %s) " \
                                M "\n", __FILE__, __LINE__, \
                                CAG_CLEAN_ERRNO())

#define CAG_LOG_INFO(M) fprintf(_CAG_LOG_, "[INFO] (%s:%d) " M "\n", \
                                __FILE__, __LINE__)

#define CAG_CHECK(A, M) if(!(A)) { CAG_LOG_ERR(M); errno=0; \
        goto error; }

#define CAG_CHECKL(A, M, L) if(!(A)) { CAG_LOG_ERR(M); errno=0; \
        goto L; }

#define CAG_SENTINEL(M)  { \
        CAG_LOG_ERR(M); \
        errno=0; goto error; \
    }

#define CAG_CHECK_MEM(A) CAG_CHECK((A), "Out of memory.")

#define CAG_CHECK_DEBUG_C89(A, M) if(!(A)) { CAG_DEBUG_C89(M); \
        errno=0; goto error; }

#endif /* CAG_ERROR_H */
