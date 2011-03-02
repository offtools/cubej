## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" 

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = libjuce.la


libjuce_la_SOURCES = \
	juce_amalgamated.cpp      

libjuce_la_CXXFLAGS = \
	-I/usr/include/freetype2

libjuce_la_LIBADD = \
	-lfreetype \
	-lpthread \
	-lasound

include_HEADERS = \
	juce_amalgamated.h   \
	juce_amalgamated.mm   


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libjuce-1.52.pc

EXTRA_DIST = \
	libjuce-1.52.pc.in
