#!/bin/bash

filename="./tag_num.txt"
./clean.sh
cat $filename | while read LINE
do
	mkdir long short
	make
	./run $LINE
	python dir_operation.py
	mv tmp/replay/*.csv results/${LINE}_flipGFcetd_gf1.csv
	rm -rf long short
	./clean.sh
done 

#./run
#python dir_operation.py
