# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src/lib/clapck
# Filename:		makefile.mak
# Integrator:		Michael D. Garris
# Organization:		NIST/ITL
# Host System:		REDHAT LINUX 5.0
# Date Created:		5/31/2000
#
# **********************************************************
# Makefile for SRC Library Directory
# **********************************************************
SHELL=/bin/sh
# PROJDIR and INSTARCH Passed On Command Line ONLY
PROJECT_ROOT	= $(PROJDIR)
ARCHTYPE	= $(INSTARCH)
LIBDIR		= $(PROJECT_ROOT)/lib
SRCDIR		= $(PROJECT_ROOT)/src/lib/clapck

# **************************************************
# INDIVIDUAL PROGRAM MODIFICATIONS SHOULD BEGIN HERE
# **************************************************

# Library to make (library name)
LIBRARY	= libclapck.a
# Source files for LIBRARY
SRC	= ilaenv.c \
	lsame.c \
	slacpy.c \
	slae2.c \
	slaebz.c \
	slaev2.c \
	slagtf.c \
	slagts.c \
	slamc1.c \
	slamc2.c \
	slamc3.c \
	slamc4.c \
	slamc5.c \
	slamch.c \
	slanst.c \
	slansy.c \
	slapy2.c \
	slarf.c \
	slarfb.c \
	slarfg.c \
	slarft.c \
	slarnv.c \
	slartg.c \
	slaruv.c \
	slascl.c \
	slaset.c \
	slasr.c \
	slasrt.c \
	slassq.c \
	slatrd.c \
	sorg2l.c \
	sorg2r.c \
	sorgql.c \
	sorgqr.c \
	sorgtr.c \
	sorm2l.c \
	sorm2r.c \
	sormql.c \
	sormqr.c \
	sormtr.c \
	sstebz.c \
	sstein.c \
	ssteqr.c \
	ssterf.c \
	ssyevx.c \
	ssytd2.c \
	ssytrd.c


# Local additions for CFLAG options (eg: -g)
LOCAL_CFLAGS	= -O2

# *******************************************************
# THE REST OF THE MAKEFILE SHOULD NOT NEED TO BE MODIFIED
#       (EXCEPT UPON APPROVAL OF PROJECT MANAGER)
# *******************************************************

INCLUDE = $(PROJECT_ROOT)/include
OBJ     = $(SRC:.c=.o)
LIBRY   = $(LIBRARY)

CFLAGS	= -I$(INCLUDE) -L$(LIBDIR) $(LOCAL_CFLAGS)
CC	= gcc $(CFLAGS)

ARFLAGS = ru
AR	= ar $(ARFLAGS)

MAKEFILE	= makefile.mak
.PRECIOUS: $(MAKEFILE)

.c.o:
	$(CC) -c $<

it: $(LIBRY)

install: $(MAKEFILE) $(LIBDIR)/$(LIBRARY)
$(LIBDIR)/$(LIBRARY): $(LIBRY)
	cp $(LIBRY) $(LIBDIR)
	chmod 664 $(LIBDIR)/$(LIBRARY)
	ranlib $(LIBDIR)/$(LIBRARY)
	catalog.sh proc $(SRCDIR) c > /dev/null

$(LIBRY): $(OBJ)
	$(AR) $(LIBRY) $?
	ranlib $(LIBRY)

%.o: %.c
	$(CC) -c -o $@ $<

clean :
	rm -f $(OBJ) *.BAK *~ #*# dependlist;

bare: clean
	rm -f $(LIBRY)

$(MAKEFILE): $(SRC)
	$(CC) -M $(SRC) > dependlist
	@sed -e '1,/^# DO NOT DELETE/!d' $(MAKEFILE) > $(MAKEFILE).tmp.$$$$; \
	cat dependlist >> $(MAKEFILE).tmp.$$$$; \
	cp $(MAKEFILE) $(MAKEFILE).BAK; \
	mv $(MAKEFILE).tmp.$$$$ $(MAKEFILE); \
	rm -f dependlist

depend: $(SRC)
	$(CC) -M $(SRC) > dependlist
	@sed -e '1,/^# DO NOT DELETE/!d' $(MAKEFILE) > $(MAKEFILE).tmp.$$$$; \
	cat dependlist >> $(MAKEFILE).tmp.$$$$; \
	cp $(MAKEFILE) $(MAKEFILE).BAK; \
	mv $(MAKEFILE).tmp.$$$$ $(MAKEFILE); \
	rm -f dependlist; \
	echo make depend complete

instarch : bare
	@case "$(ARCHTYPE)" in \
	sol|sgi|aix|lnx|cyg) rm -f makefile.mak; \
	     cp makefile.$(ARCHTYPE) makefile.mak; \
	     chmod 640 makefile.mak; \
	     echo $(ARCHTYPE) files installed \
	;; \
	*) echo 'Tried to install uknown machine architecture: '$(ARCHTYPE); \
	   exit 1 \
	;; \
	esac

# DO NOT DELETE THIS LINE - make depend uses it
ilaenv.o: ilaenv.c /usr/local/hsfsys2/include/f2c.h
lsame.o: lsame.c /usr/local/hsfsys2/include/f2c.h
slacpy.o: slacpy.c /usr/local/hsfsys2/include/f2c.h
slae2.o: slae2.c /usr/local/hsfsys2/include/f2c.h
slaebz.o: slaebz.c /usr/local/hsfsys2/include/f2c.h
slaev2.o: slaev2.c /usr/local/hsfsys2/include/f2c.h
slagtf.o: slagtf.c /usr/local/hsfsys2/include/f2c.h
slagts.o: slagts.c /usr/local/hsfsys2/include/f2c.h
slamc1.o: slamc1.c /usr/local/hsfsys2/include/f2c.h
slamc2.o: slamc2.c /usr/local/hsfsys2/include/f2c.h
slamc3.o: slamc3.c /usr/local/hsfsys2/include/f2c.h
slamc4.o: slamc4.c /usr/local/hsfsys2/include/f2c.h
slamc5.o: slamc5.c /usr/local/hsfsys2/include/f2c.h
slamch.o: slamch.c /usr/local/hsfsys2/include/f2c.h
slanst.o: slanst.c /usr/local/hsfsys2/include/f2c.h
slansy.o: slansy.c /usr/local/hsfsys2/include/f2c.h
slapy2.o: slapy2.c /usr/local/hsfsys2/include/f2c.h
slarf.o: slarf.c /usr/local/hsfsys2/include/f2c.h
slarfb.o: slarfb.c /usr/local/hsfsys2/include/f2c.h
slarfg.o: slarfg.c /usr/local/hsfsys2/include/f2c.h
slarft.o: slarft.c /usr/local/hsfsys2/include/f2c.h
slarnv.o: slarnv.c /usr/local/hsfsys2/include/f2c.h
slartg.o: slartg.c /usr/local/hsfsys2/include/f2c.h
slaruv.o: slaruv.c /usr/local/hsfsys2/include/f2c.h
slascl.o: slascl.c /usr/local/hsfsys2/include/f2c.h
slaset.o: slaset.c /usr/local/hsfsys2/include/f2c.h
slasr.o: slasr.c /usr/local/hsfsys2/include/f2c.h
slasrt.o: slasrt.c /usr/local/hsfsys2/include/f2c.h
slassq.o: slassq.c /usr/local/hsfsys2/include/f2c.h
slatrd.o: slatrd.c /usr/local/hsfsys2/include/f2c.h
sorg2l.o: sorg2l.c /usr/local/hsfsys2/include/f2c.h
sorg2r.o: sorg2r.c /usr/local/hsfsys2/include/f2c.h
sorgql.o: sorgql.c /usr/local/hsfsys2/include/f2c.h
sorgqr.o: sorgqr.c /usr/local/hsfsys2/include/f2c.h
sorgtr.o: sorgtr.c /usr/local/hsfsys2/include/f2c.h
sorm2l.o: sorm2l.c /usr/local/hsfsys2/include/f2c.h
sorm2r.o: sorm2r.c /usr/local/hsfsys2/include/f2c.h
sormql.o: sormql.c /usr/local/hsfsys2/include/f2c.h
sormqr.o: sormqr.c /usr/local/hsfsys2/include/f2c.h
sormtr.o: sormtr.c /usr/local/hsfsys2/include/f2c.h
sstebz.o: sstebz.c /usr/local/hsfsys2/include/f2c.h
sstein.o: sstein.c /usr/local/hsfsys2/include/f2c.h
ssteqr.o: ssteqr.c /usr/local/hsfsys2/include/f2c.h
ssterf.o: ssterf.c /usr/local/hsfsys2/include/f2c.h
ssyevx.o: ssyevx.c /usr/local/hsfsys2/include/f2c.h
ssytd2.o: ssytd2.c /usr/local/hsfsys2/include/f2c.h
ssytrd.o: ssytrd.c /usr/local/hsfsys2/include/f2c.h
