export ProDir=$(cd `dirname $0`; pwd)

export OBJ=$ProDir/obj

FALSE=0

if [ $# -eq $FALSE ]; then
    mkdir -p $OBJ
    make -f ./build/Makefile
else
    rm -rf $OBJ
    make -f ./build/Makefile clean
fi

