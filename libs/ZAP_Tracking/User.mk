
# Author  KMS - Martin Dubois, P.Eng.
# Client  ZAP
# Product Tracking
# File    User.mk

IMPORT_FOLDER = ../Import

# COMPILE_FLAGS = -DDEBUG -fpic -ggdb -O2
COMPILE_FLAGS = -DNDEBUG -fpic -ggdb -O2

INCLUDE_IMPORT = -I $(IMPORT_FOLDER)/Includes

EthCAN_LIB_A = $(IMPORT_FOLDER)/Libraries/EthCAN_Lib.a