# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf
# Filename:		makefile.mak
# Integrator:		Michael D. Garris
# Organization:		NIST/ITL
# Host System:		SUN OS/4.1.3 & 4.1.2 & 4.1.1 (UNIX)
# Date Created:		6/12/2000
#
# **********************************************************
# Makefile for Root Project Directory
# **********************************************************
SHELL=/bin/sh
#
PROJDIR = /usr/local/hsfsys2
# INSTARCH Passed On Command Line ONLY
SUBS = src

it install depend clean bare:
	@-X=`pwd`; \
	for i in $(SUBS); \
	do echo '<<<' $$i '>>>'; cd $$X/$$i; \
	make -f makefile.mak $@ PROJDIR=$(PROJDIR) INSTARCH=$(INSTARCH); done

instarch:
	@-X=`pwd`; \
	rm -f bin/*.exe lib/*.a ;\
	if [ "$(INSTARCH)" = "cyg" ] ; \
	then echo '<<<' include '>>>'; \
	rm -f include/values.h; \
	cp include/values.lnx include/values.h; \
	chmod 444 include/values.h; fi;\
	for i in $(SUBS); \
	do echo '<<<' $$i '>>>'; cd $$X/$$i; \
	make -f makefile.mak $@ PROJDIR=$(PROJDIR) INSTARCH=$(INSTARCH); done

