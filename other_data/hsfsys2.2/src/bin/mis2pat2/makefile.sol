# **********************************************************
# Project:		NIST HSF Recognition System
# SubTree:		./hsf/src/bin/mis2pat2
# Filename:		makefile.mak
# Programmer:		Michael D. Garris
# Organization:		NIST/ITL
# Host System:		SOLARIS SUN OS/5.2 (UNIX)
# Date Created:		9/04/96
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
EXECUTABLE	= mis2pat2
# Source files for EXECUTABLE
SRC	= mis2pat2.c
# LIBS needed to compile the EXECUTABLE
# (Full paths eg: /usr/local/image/lib/libimage.a)
# use $(LIBDIR) defined above for path extensions if appropriate
LIBS	= $(LIBDIR)/libhsf.a \
	$(LIBDIR)/libnn.a \
	$(LIBDIR)/libmis.a \
	$(LIBDIR)/libimage.a \
	$(LIBDIR)/libmfs.a \
	$(LIBDIR)/libihead.a \
	$(LIBDIR)/libutil.a

# How the libraries look when invoked on the compile line (eg: -limage)
LLIBS	= -lhsf -lnn -lmis -limage -lmfs -lihead -lutil -lm

# Local additions for CFLAG options (eg: -g)
LOCAL_CFLAGS	= -xO3

# *******************************************************
# THE REST OF THE MAKEFILE SHOULD NOT NEED TO BE MODIFIED
#       (EXCEPT UPON APPROVAL OF PROJECT MANAGER)
# *******************************************************

BIN	= $(PROJECT_ROOT)/bin
OBJ	= $(SRC:.c=.o)
INCLUDE = $(PROJECT_ROOT)/include

CFLAGS	= -I$(INCLUDE) -L$(LIBDIR) -s $(LOCAL_CFLAGS)
CC	= cc $(CFLAGS)

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
