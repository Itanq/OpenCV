export ProjDir=$(cd `dirname $0`; pwd)

if [ -z $C_OUT_PATH ]; then
    export OutDir=$ProjDir/out
    export SrcDir=$ProjDir/src
    export HeaderDir=$ProjDir/include
    export ObjDir=$ProjDir/build/objs
else
    export OutDir=$C_OUT_PATH/out
    export SrcDir=$C_OUT_PATH/src
    export HeaderDir=$C_OUT_PATH/include
    export ObjDir=$C_OUT_PATH/build/objs
fi
FALSE=0
TRUE=1
CLEAN=$TRUE
BUILD=$FALSE

if [ $# -eq $FALSE ]; then
	BUILD=$TRUE
fi

if [ $CLEAN -eq $TRUE ]; then
	echo "Clean"
	rm -rf $TmpObjDir $OutDir
    make -f $ProjDir/build/Makefile clean
fi

if [ $BUILD -eq $TRUE ]; then
	echo "Build"
    echo $ObjDir
	mkdir -p $ObjDir $OutDir
    echo "Start make Makefile"
	make -f $ProjDir/build/Makefile
fi

