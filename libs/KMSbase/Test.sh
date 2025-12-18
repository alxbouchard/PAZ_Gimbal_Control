#!/bin/sh

# Author / Auteur	:	KMS	-	Martin Dubois, ing.
# Product / Produit	:	KmsBase
# File / Fichier	:	Test.sh

echo  Executing Test.sh ....

# ===== Verification ========================================================

KMS_LIB_TEST=Binaries/KmsLib_Test
if test ! -f $KMS_LIB_TEST ; then
    echo FATAL ERROR : $KMS_LIB_TEST does not exist.
    exit 1 ;
fi

# ===== Execution ===========================================================

$KMS_LIB_TEST
if [ 0 != $? ] ; then
    echo  $KMS_LIB_TEST reported an error.
    exit 2 ;
fi

# ===== End =================================================================

echo  OK

