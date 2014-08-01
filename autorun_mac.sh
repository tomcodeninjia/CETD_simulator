#!/bin/bash

filename="./tag_num.txt"
./clean.sh
cat $filename | while read LINE
do
	make
	./run $LINE
	python dir_operation.py
	mv tmp/replay/*.csv results/${LINE}_flipGFcetd_gf1.csv
	./clean.sh
done 

#./run
#python dir_operation.py
