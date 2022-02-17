#!/bin/bash

cd $1

make clean

cd ..

tar cvfz $1.tar.gz $1/

mv *.gz ../turnIn
