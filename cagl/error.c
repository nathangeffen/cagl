#include <stdarg.h>

#include "cagl/error.h"

FILE *cag_log = NULL;

void cag_fprintf(const char *fmt, ...)
{
        va_list ap;

        va_start(ap, fmt);
	vfprintf(_CAG_LOG_, fmt, ap);
	fprintf(_CAG_LOG_, "\n");
        va_end(ap);
}
