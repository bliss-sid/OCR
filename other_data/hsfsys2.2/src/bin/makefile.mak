# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src/bin
# Filename:		makefile.mak
# Integrator:		Michael D. Garris
# Organization:		NIST/ITL
# Host System:		SUN OS/4.1.3 & 4.1.2 & 4.1.1 (UNIX)
# Date Created:		3/31/94
#
# **********************************************************
# Makefile for SRC Subdirectory
# **********************************************************
SHELL=/bin/sh
#
SUBS = hsfsys1 \
	hsfsys2 \
	mis2evt \
	mis2pat1 \
	mis2pat2 \
	mlp \
	trainreg

it depend clean bare:
	@-X=`pwd`; \
	for i in $(SUBS); \
	do echo '<' $$i '>'; cd $$X/$$i; \
	make -f makefile.mak $@ PROJDIR=$(PROJDIR) INSTARCH=$(INSTARCH); done

instarch:
	@-X=`pwd`; \
	for i in $(SUBS); \
	do echo '<' $$i '>'; cd $$X/$$i; \
	make -f makefile.$(INSTARCH) $@ PROJDIR=$(PROJDIR) INSTARCH=$(INSTARCH); done

install:
	@-X=`pwd`; \
	for i in $(SUBS); \
	do echo '<' $$i '>'; cd $$X/$$i; \
	make -f makefile.mak $@ PROJDIR=$(PROJDIR); done; \
	catalog.sh pgrm $(PROJDIR)/src/bin c > /dev/null
