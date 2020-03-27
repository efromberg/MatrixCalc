# -Matrix/Makefile    template library development
# Put all Matimp.h template functions into libmtrx1.so

MATRIX=/home/edgar/Programs/Matrix
LD_LIBRARY_PATH=${MATRIX}

STD=-std=c++11
FLAGS=-Wall

#CPP=g++-7
CPP=clang++-6.0

all : tstMatt tstnext
	echo "CPP="${CPP}" "${MATRIX}


libmtrx1.so : Mat.cc Mat.h Matimp.h matfuncs.h
	${CPP} -o libmtrx1.so -fPIC -shared Mat.cc


tstMatt : tstMatt.cc libmtrx1.so
	${CPP} -o tstMatt tstMatt.cc -L . -lmtrx1

tstnext : tstnext.cc libmtrx1.so
	${CPP} -o tstnext tstnext.cc -L . -lmtrx1

