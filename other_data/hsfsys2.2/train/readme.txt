The training directories {hsf_4, hsf_6, hsf_7} have been
removed from this release for internet distribution.

The files in these directories are required to build
pattern files, which are  needed to run the PNN version
of the system "hsfsys1" and to retrain the weights for
the MLP version of the system "hsfsys2".  Without these
pattern files, "hsfsys2" is able to execute with the
precomputed weights provided, but "hsfsys1" will NOT
execute.

The training directories {hsf_4, hsf_6, hsf_7} are
distributed as part of NIST Special Database 19 (SD19),
which can be purchased for a nominal fee of $150.00 from
our distributor:

   Standard Reference Data
   srdata@nist.gov

To generate the pattern files in this release, the
contents of SD19 under the directories:

   ./data/by_write/{hsf_4,hsf_6,hsf_7}

 should be linked or copied to this release's directory

   $PROJDIR/train/

The patterns files used in this release can be computed
locally once this training data is made available.  The
commands for generating patterns files can be found in:

   $PROJDIR/weights/*/*/readme.txt
