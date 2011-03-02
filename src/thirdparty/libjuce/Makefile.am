## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

libjucedocdir = ${prefix}/doc/libjuce
libjucedoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(libjucedoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(libjucedocdir)
