# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src/lib/clapck
# Filename:		makefile.mak
# Integrator:		Michael D. Garris
# Organization:		NIST/ITL
# Host System:		SOLARIS SUN OS/5.2 (UNIX)
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
LOCAL_CFLAGS	= -xO3

# *******************************************************
# THE REST OF THE MAKEFILE SHOULD NOT NEED TO BE MODIFIED
#       (EXCEPT UPON APPROVAL OF PROJECT MANAGER)
# *******************************************************

INCLUDE = $(PROJECT_ROOT)/include
OBJ     = $(SRC:.c=.o)
LIBRY   = $(LIBRARY)

CFLAGS	= -I$(INCLUDE) -L$(LIBDIR) -s $(LOCAL_CFLAGS)
CC	= cc $(CFLAGS)

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
	catalog.sh proc $(SRCDIR) c > /dev/null

$(LIBRY): $(OBJ)
	$(AR) $(LIBRY) $?

%.o: %.c
	$(CC) -c -o $@ $<

clean :
	rm -f $(OBJ) *.BAK *~ #*# dependlist;

bare: clean
	rm -f $(LIBRY)

$(MAKEFILE): $(SRC)
	$(CC) -xM $(SRC) > dependlist
	@sed -e '1,/^# DO NOT DELETE/!d' $(MAKEFILE) > $(MAKEFILE).tmp.$$$$; \
	cat dependlist >> $(MAKEFILE).tmp.$$$$; \
	cp $(MAKEFILE) $(MAKEFILE).BAK; \
	mv $(MAKEFILE).tmp.$$$$ $(MAKEFILE); \
	rm -f dependlist

depend: $(SRC)
	$(CC) -xM $(SRC) > dependlist
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
