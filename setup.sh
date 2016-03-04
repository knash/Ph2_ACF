#!/bin/bash

#CACTUS
export CACTUSBIN=/opt/cactus/bin
export CACTUSLIB=/opt/cactus/lib
export CACTUSINCLUDE=/opt/cactus/include


# BOOST
export BOOST_LIB=/opt/cactus/lib
export BOOST_INCLUDE=/opt/cactus/include


#ROOT
#source /usr/local/bin/thisroot.sh

#Ph2_ACF
export BASE_DIR=$(pwd)

export PATH=$QTDIR/bin:$BASE_DIR/bin:$PATH
export LD_LIBRARY_PATH=$BASE_DIR/RootWeb/lib:/opt/cactus/lib:$QTROOTSYSDIR/lib:$QTDIR/lib:$BASE_DIR/lib:${LD_LIBRARY_PATH}
