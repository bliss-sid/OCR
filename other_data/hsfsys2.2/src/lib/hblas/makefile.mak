# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src/lib/hblas
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
SRCDIR		= $(PROJECT_ROOT)/src/lib/hblas

# **************************************************
# INDIVIDUAL PROGRAM MODIFICATIONS SHOULD BEGIN HERE
# **************************************************

# Library to make (library name)
LIBRARY	= libhblas.a
# Source files for LIBRARY
SRC	= isamax.c \
	lsame.c \
	sasum.c \
	saxpy.c \
	scopy.c \
	sdot.c \
	sgemm.c \
	sgemv.c \
	sger.c \
	snrm2.c \
	sscal.c \
	sswap.c \
	ssymv.c \
	ssyr2.c \
	ssyr2k.c \
	strmm.c \
	strmv.c \
	xerbla.c

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
isamax.o: isamax.c /usr/local/hsfsys2/include/f2c.h
lsame.o: lsame.c /usr/local/hsfsys2/include/f2c.h
sasum.o: sasum.c /usr/local/hsfsys2/include/f2c.h
saxpy.o: saxpy.c /usr/local/hsfsys2/include/f2c.h
scopy.o: scopy.c /usr/local/hsfsys2/include/f2c.h
sdot.o: sdot.c /usr/local/hsfsys2/include/f2c.h
sgemm.o: sgemm.c /usr/local/hsfsys2/include/f2c.h
sgemv.o: sgemv.c /usr/local/hsfsys2/include/f2c.h
sger.o: sger.c /usr/local/hsfsys2/include/f2c.h
snrm2.o: snrm2.c /usr/local/hsfsys2/include/f2c.h
sscal.o: sscal.c /usr/local/hsfsys2/include/f2c.h
sswap.o: sswap.c /usr/local/hsfsys2/include/f2c.h
ssymv.o: ssymv.c /usr/local/hsfsys2/include/f2c.h
ssyr2.o: ssyr2.c /usr/local/hsfsys2/include/f2c.h
ssyr2k.o: ssyr2k.c /usr/local/hsfsys2/include/f2c.h
strmm.o: strmm.c /usr/local/hsfsys2/include/f2c.h
strmv.o: strmv.c /usr/local/hsfsys2/include/f2c.h
xerbla.o: xerbla.c /usr/local/hsfsys2/include/f2c.h
