export ProjDir=$(cd `dirname $0`; pwd)

export SRCDIR=$ProjDir/src
export OBJDIR=$ProjDir/build/objs

if [ $# -eq 0 ];then
    mkdir -p $OBJDIR
    make -f $ProjDir/build/Makefile
else
    rm -rf $OBJDIR test
fi


