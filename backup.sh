#! /bin/sh

echo Create Vect-Distribution 
echo ~~~~~~~~~~~~~~~~~~~~~~~~

if [ -f distr/vect$@.tar.gz ]; then
    if [ -f distr/vect$@.tar.gz~ ]; then
	echo Remove old backup distribution
	rm distr/vect$@.tar.gz~;
    fi
    echo Create new backup
    mv distr/vect$@.tar.gz distr/vect$@.tar.gz~
fi

echo Create new distribution: vect$@.tar.gz
cd ..
tar -czf vect/distr/vect$@.tar.gz vect/src/*.cc vect/src/*.hh vect/src/Makefile
cd vect


