# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src/lib/adseg
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
SRCDIR		= $(PROJECT_ROOT)/src/lib/adseg

# **************************************************
# INDIVIDUAL PROGRAM MODIFICATIONS SHOULD BEGIN HERE
# **************************************************

# Library to make (library name)
LIBRARY	= libadseg.a
# Source files for LIBRARY
SRC	= cutpath.c \
	cutvert.c \
	dot.c \
	mergblob.c \
	multichr.c \
	noise.c \
	paste.c \
	segchars.c \
	segpara.c \
	shaplike.c \
	touching.c

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
cutpath.o: cutpath.c /usr/include/math.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h \
  /usr/include/bits/mathcalls.h /usr/include/bits/mathinline.h \
  /usr/local/hsfsys2/include/blobls.h /usr/include/stdio.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/local/hsfsys2/include/defs.h /usr/local/hsfsys2/include/segchars.h
cutvert.o: cutvert.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/local/hsfsys2/include/blobls.h \
  /usr/local/hsfsys2/include/segchars.h /usr/local/hsfsys2/include/defs.h
dot.o: dot.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  /usr/include/bits/mathinline.h /usr/local/hsfsys2/include/blobls.h \
  /usr/local/hsfsys2/include/segchars.h /usr/local/hsfsys2/include/defs.h
mergblob.o: mergblob.c /usr/include/math.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h \
  /usr/include/bits/mathcalls.h /usr/include/bits/mathinline.h \
  /usr/local/hsfsys2/include/blobls.h \
  /usr/local/hsfsys2/include/segchars.h /usr/local/hsfsys2/include/defs.h
multichr.o: multichr.c /usr/local/hsfsys2/include/blobls.h \
  /usr/local/hsfsys2/include/defs.h
noise.o: noise.c /usr/local/hsfsys2/include/blobls.h \
  /usr/local/hsfsys2/include/segchars.h /usr/local/hsfsys2/include/defs.h
paste.o: paste.c /usr/local/hsfsys2/include/blobls.h
segchars.o: segchars.c /usr/local/hsfsys2/include/blobls.h \
  /usr/local/hsfsys2/include/segchars.h \
  /usr/local/hsfsys2/include/multsort.h /usr/local/hsfsys2/include/defs.h
segpara.o: segpara.c /usr/local/hsfsys2/include/blobls.h \
  /usr/local/hsfsys2/include/segchars.h /usr/local/hsfsys2/include/defs.h
shaplike.o: shaplike.c /usr/include/math.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h \
  /usr/include/bits/mathcalls.h /usr/include/bits/mathinline.h \
  /usr/local/hsfsys2/include/segchars.h \
  /usr/local/hsfsys2/include/blobls.h /usr/local/hsfsys2/include/defs.h
touching.o: touching.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/local/hsfsys2/include/blobls.h \
  /usr/local/hsfsys2/include/segchars.h /usr/local/hsfsys2/include/defs.h
