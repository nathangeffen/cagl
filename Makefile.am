## Makefile.am for libcagl
## Based on LibExample from
## http://www.openismus.com/documents/linux/building_libraries/building_libraries

## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.

AUTOMAKE_OPTIONS = subdir-objects

## ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libcagl-@CAGL_API_VERSION@.la

libcagl_@CAGL_API_VERSION@_la_SOURCES = cagl/common.c \
                                        cagl/error.c \
                                        cagl/prim.c

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.

libcagl_@CAGL_API_VERSION@_la_LDFLAGS = -version-info $(CAGL_SO_VERSION)

## Define the list of public header files and their install location.  The
## nobase_ prefix instructs Automake to not strip the directory part from each
## filename, in order to avoid the need to define separate file lists for each
## installation directory.  This only works if the directory hierarchy in the
## source tree matches the hierarchy at the install location, however.

cagl_includedir = $(includedir)/cagl-$(CAGL_API_VERSION)

nobase_cagl_include_HEADERS = 	cagl/common.h \
				cagl/array.h \
				cagl/concepts.h \
				cagl/dlist.h \
				cagl/error.h \
				cagl/hash.h \
				cagl/prim.h \
				cagl/test.h \
				cagl/slist.h \
				cagl/tree.h

## The generated configuration header is installed in its own subdirectory of
## $(libdir).  The reason for this is that the configuration information put
## into this header file describes the target platform the installed library
## has been built for.  Thus the file must not be installed into a location
## intended for architecture-independent files, as defined by the Filesystem
## Hierarchy Standard (FHS).
## The nodist_ prefix instructs Automake to not generate rules for including
## the listed files in the distribution on 'make dist'.  Files that are listed
## in _HEADERS variables are normally included in the distribution, but the
## configuration header file is generated at configure time and should not be
## shipped with the source tarball.

cagl_libincludedir = $(libdir)/cagl-$(CAGL_API_VERSION)/include

## Install the generated pkg-config file (.pc) into the expected location for
## architecture-dependent package configuration information.  Occasionally,
## pkg-config files are also used for architecture-independent data packages,
## in which case the correct install location would be $(datadir)/pkgconfig.

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = cagl-$(CAGL_API_VERSION).pc

## Define an independent executable script for inclusion in the distribution
## archive.  However, it will not be installed on an end user's system due to
## the noinst_ prefix.

dist_noinst_SCRIPTS = autogen.sh


## Set up tests
TESTS = bin/runtest1
