#!/bin/sh

# Author   KMS - Martin Dubois, ing.
# product  KmsBase
# File     RunDoxygen.sh
# Usage    ./RunDoxygen.sh

# CODE REVIEW  2019-07-19  KMS - Martin Dubois, ing.

echo Executing  RunDoxygen.sh  ...

# ===== Execution ===========================================================

doxygen DoxyFile_en.txt
if [ 0 != $? ] ; then
    echo ERROR  doxygen DoxyFile_en.txt  failed
    exit 10
fi

doxygen DoxyFile_fr.txt
if [ 0 != $? ] ; then
    echo ERROR  doxygen DoxyFile_fr.txt  failed
    exit 10
fi

# ===== End =================================================================

echo OK
