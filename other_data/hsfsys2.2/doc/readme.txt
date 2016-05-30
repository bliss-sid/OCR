         NIST Form-Based Handprint Recognition System
                         (Release 2.2)
                           April 2003
                       Michael D. Garris
                       mgarris@nist.gov

This CD-ROM distribution contains software and data supporting Release 2.2
of the NIST Form-Based Handprint Recognition System.  This release is has
been packaged for internet distribution and represents what is intended to
be the final release of software from this project.  The contents of
this release should be installed in its entirety.  The documentation for
Release 2.0 (NISTIR 5959, doc/hsfsys2.pdf) applies to this release with
the following exceptions.


1. RELEASE 2.2 INCORPORATES ALL CAPABILITIES AND UPDATES ASSOCIATED
   WITH RELEASE 2.1

1.1 Release 2.1 was necessitated by the fact that Release 2.0 would not
successfully compile and execute using the GNU gcc compiler.  More
specifically, the implementation of numerical recipes used to perform
eigenvector-based feature extraction were not as portable as desired.  In
this new release, eigenvector computation is supported by a set of portable
and more robust routines provided from the CLAPACK mathematical library
(www.netlib.org/clapack).

As a result, compilation scripts are provided that support GNU gcc and
gmake (www.gnu.org) and are targeted, but not limited, to the Linux operating
system (www.linux.org).  Release 2.1 supported the following UNIX Operating
Systems: Linux, SGI IRIX, Sun Solaris, and IBM AIX.

1.2 Release 2.1 was also designed to be compiled to run on computers running
the family of Win32 operating systems by first installing the Cygwin library
and associated port of GNU tools (www.cygwin.com).  The abbreviations used to
install these target architectures is listed in the table below. Compilation
scripts specifically supporting Open System Foundation (OSF), HP UNIX, and
SunOS are no longer provided as these operating systems were not available
for testing.  However, compilation is now generally possible on those systems
that support GNU gcc and GNU gmake by installing the Linux compilation
scripts.  The porting of the software to other operating systems, compilers,
or compilation environments is the responsibility of the recipient.


   Operating System   Installation      Assumed
                      Abbreviation     Byte Order
   ------------------------------------------------
     Linux                lin         little endian
     Win32/Cygwin         cyg         little endian
     SGI IRIX             sgi          big endian
     Sun Solaris          sol          big endian
     IBM AIX              aix          big endian


1.3 Another change in Release 2.1 had to do with the byte order of binary
application data files.  Release 2.1 was modified to consistently
read and write binary files in big endian byte order.  A small number of
alternative files are provided in the source code libraries for little
endian architectures, in which case data units are swapped so that these
computers can operate on, and generate application files in, big endian
byte order.  The software installation scripts assume the byte orders as
listed in the table above and install these alternative files accordingly.
Alternative source code files supporting big endian byte order have
extension ".big" and those supporting little endian byte order have
extension ".lit".  If your computer has byte order opposite that listed
in the table above, you will have to manually copy the version of these
source code files appropriate for your particular computer to files having
extension ".c".

1.4 Finally, Release 2.2 incorporates all bug fixes generated for
Release 2.1.  As this project is no longer active, there will be no
further postings of updates.  If needed, the entire internet distribution
will be updated and the date of last modification posted.


2. RELEASE 2.2 HAS BEEN DOWNSIZED FOR INTERNET DISTRIBUTION

As mentioned, this release has been configured for internet distribution.
Up to this time, this package was distributed exclusively on CD-ROM
and required over 370 Mb.  This included all training files containing
images of segmented characters along with pattern files containing
feature vectors derived from these images.  Most of the 370 Mb of
storage required on CD-ROM belonged to these files.

To make the internet distribution of reasonable size (the gzipped tarball
is under 5.5 Mb), all training image files and pattern files have been
removed.  This has the following implications:

   * The PNN-based system "hsfsys1" will not execute until its
     pattern files are computed locally by the recipient.  See
     the "readme.txt" files under "$PROJDIR/weights/pnn/".
     (Note that the MLP-based system "hsfsys2" will execute using
      the precomputed weights provided in the internet distribution.)

   * The MLP classifier used in "hsfsys2" cannot be retrained until
     its pattern files are computed locally by the recipient.  See
     the "readme.txt" files under "$PROJDIR/weights/mlp/".

   * Pattern files cannot be computed locally without the files
     of segmented character images distributed in NIST Special
     Database 19 (SD19).  This database is available for a nominal
     fee of $150.00 from our distributor:

        Standard Reference Data
        srdata@nist.gov

     Prior to computing pattern files, training files from SD19 must
     be linked or copied into "$PROJDIR/train/" according to the
     "readme.txt" file in "$PROJDIR/train/".


In conclusion, CD-ROM distribution of this package will no longer
continue.  The software may now be freely downloaded from the
internet via anonymous FTP at sequoyah.nist.gov.  Any portion of
this package may be used without restrictions.  Recipients assume
all responsibilities associated with its operation, modification,
and maintenance.


Much success!
Mike Garris
mgarris@nist.gov
