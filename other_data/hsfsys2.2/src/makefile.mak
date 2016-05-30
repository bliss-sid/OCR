# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src
# Filename:		makefile.mak
# Integrator:		Michael D. Garris
# Organization:		NIST/ITL
# Host System:		SUN OS/4.1.3 & 4.1.2 & 4.1.1 (UNIX)
# Date Created:		3/31/94
#
# **********************************************************
# Makefile for SRC Directory
# **********************************************************
SHELL=/bin/sh
#
SUBS = lib bin

it install instarch depend clean bare:
	@-X=`pwd`; \
	for i in $(SUBS); \
	do echo '<<' $$i '>>'; cd $$X/$$i; \
	make -f makefile.mak $@ PROJDIR=$(PROJDIR) INSTARCH=$(INSTARCH); done
