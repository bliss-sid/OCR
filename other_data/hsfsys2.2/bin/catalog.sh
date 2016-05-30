#!/bin/sh

if [ $# -lt 3 ]
then
   program=`basename $0`
   echo Usage\: $program \<pgrm\|proc\> \<dir\> \<extensions\>
   exit 1
fi

key=$1
dpath=$2
shift 2
pd=`pwd`
cd $dpath
ofile=catalog.txt
rm -f $ofile

case $key
in
   pgrm)
     for ext in $* `echo $* | tr 'a-z' 'A-Z'`
     do
        grep -h "# ${key}:" */*.${ext} | awk -F\: '{print $2}' | tee -a $ofile
        grep -h "#cat:" */*.${ext} | awk -F\: '{print $2}' | tee -a $ofile
     done
     ;;
   proc)
     for ext in $* `echo $* | tr 'a-z' 'A-Z'`
     do
        grep -h "# ${key}:" *.${ext} | awk -F\: '{print $2}' | tee -a $ofile
        grep -h "#cat:" *.${ext} | awk -F\: '{print $2}' | tee -a $ofile
     done
     ;;
   *)
     echo Illegal option\: $key
     ;;
esac

cd $pd
