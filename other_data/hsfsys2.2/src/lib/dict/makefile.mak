# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src/lib/dict
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
SRCDIR		= $(PROJECT_ROOT)/src/lib/dict

# **************************************************
# INDIVIDUAL PROGRAM MODIFICATIONS SHOULD BEGIN HERE
# **************************************************

# Library to make (library name)
LIBRARY	= libdict.a
# Source files for LIBRARY
SRC	= alignchr.c \
	fanout.c \
	ldistv.c \
	levencmp.c \
	line.c \
	loaddict.c \
	reverse.c \
	stordict.c \
	word.c


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
alignchr.o: alignchr.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/local/hsfsys2/include/align.h /usr/include/values.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
fanout.o: fanout.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/local/hsfsys2/include/mfs.h /usr/local/hsfsys2/include/spell_ln.h
ldistv.o: ldistv.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h \
  /usr/include/endian.h /usr/include/bits/endian.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/bits/time.h \
  /usr/include/sys/sysmacros.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/bits/string.h \
  /usr/include/bits/string2.h /usr/include/malloc.h \
  /usr/include/sys/param.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
  /usr/include/asm/param.h /usr/local/hsfsys2/include/align.h \
  /usr/include/values.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h \
  /usr/local/hsfsys2/include/pointers.h /usr/local/hsfsys2/include/defs.h
levencmp.o: levencmp.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/local/hsfsys2/include/mfs.h /usr/local/hsfsys2/include/align.h \
  /usr/include/values.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/float.h
line.o: line.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/local/hsfsys2/include/mfs.h /usr/local/hsfsys2/include/spell_ln.h
loaddict.o: loaddict.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/local/hsfsys2/include/mfs.h
reverse.o: reverse.c
stordict.o: stordict.c /usr/local/hsfsys2/include/mfs.h
word.o: word.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/local/hsfsys2/include/spell_ln.h
