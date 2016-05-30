# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src/bin/mis2pat1
# Filename:		makefile.mak
# Programmer:		Michael D. Garris
# Organization:		NIST/ITL
# Host System:		REDHAT LINUX 5.0
# Date Created:		5/31/2000
#
# **********************************************************
# Makefile for SRC Binary Directory
# **********************************************************
SHELL=/bin/sh
# PROJDIR and ARCHTYPE Passed On Command Line ONLY
PROJECT_ROOT	= $(PROJDIR)
ARCHTYPE	= $(INSTARCH)
LIBDIR		= $(PROJECT_ROOT)/lib

# **************************************************
# INDIVIDUAL PROGRAM MODIFICATIONS SHOULD BEGIN HERE
# **************************************************

# Program to make (binary name)
EXECUTABLE	= mis2pat1
# Source files for EXECUTABLE
SRC	= mis2pat1.c
# LIBS needed to compile the EXECUTABLE
# (Full paths eg: /usr/local/image/lib/libimage.a)
# use $(LIBDIR) defined above for path extensions if appropriate
LIBS	= $(LIBDIR)/libhsf.a \
	$(LIBDIR)/libnn.a \
	$(LIBDIR)/libmis.a \
	$(LIBDIR)/libimage.a \
	$(LIBDIR)/libmfs.a \
	$(LIBDIR)/libihead.a \
	$(LIBDIR)/libstats.a \
	$(LIBDIR)/libutil.a

# How the libraries look when invoked on the compile line (eg: -limage)
LLIBS	= -lhsf -lnn -lmis -limage -lmfs -lihead -lstats -lutil -lm

# Local additions for CFLAG options (eg: -g)
LOCAL_CFLAGS	= -O2

# *******************************************************
# THE REST OF THE MAKEFILE SHOULD NOT NEED TO BE MODIFIED
#       (EXCEPT UPON APPROVAL OF PROJECT MANAGER)
# *******************************************************

BIN	= $(PROJECT_ROOT)/bin
OBJ	= $(SRC:.c=.o)
INCLUDE = $(PROJECT_ROOT)/include

CFLAGS	= -I$(INCLUDE) -L$(LIBDIR) $(LOCAL_CFLAGS)
CC	= gcc $(CFLAGS)

MAKEFILE	= makefile.mak
.PRECIOUS: $(MAKEFILE)

.c.o:
	$(CC) -c $<

it: $(EXECUTABLE)

install: $(MAKEFILE) $(BIN)/$(EXECUTABLE)
$(BIN)/$(EXECUTABLE): $(EXECUTABLE)
	cp $(EXECUTABLE) $(BIN)
	chmod 755 $(BIN)/$(EXECUTABLE)
	-strip $(BIN)/$(EXECUTABLE)

$(EXECUTABLE): $(OBJ) $(LIBS)
	$(CC) $(OBJ) $(LLIBS) -o $(EXECUTABLE)

# if there are other separate programs to compile, add the name to
#	SRC, OBJ, ... and the redo the last 3 lines EXPLICITLY
#	for each program.
#	Install will also have to be changed.

clean :
	rm -f $(OBJ) core a.out *.BAK *~ #*# dependlist

bare: clean
	rm -f $(EXECUTABLE)

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
mis2pat1.o: mis2pat1.c /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/stdio.h \
  /usr/include/sys/param.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/limits.h \
  /usr/lib/gcc-lib/i386-redhat-linux/2.96/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/linux/param.h \
  /usr/include/asm/param.h /usr/include/sys/types.h /usr/include/time.h \
  /usr/include/endian.h /usr/include/bits/endian.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/bits/time.h \
  /usr/include/sys/sysmacros.h /usr/include/string.h \
  /usr/include/bits/string.h /usr/include/bits/string2.h \
  /usr/include/stdlib.h /usr/include/alloca.h \
  /usr/local/hsfsys2/include/ihead.h /usr/local/hsfsys2/include/mis.h \
  /usr/local/hsfsys2/include/mfs.h /usr/local/hsfsys2/include/kdtree.h \
  /usr/local/hsfsys2/include/defs.h
