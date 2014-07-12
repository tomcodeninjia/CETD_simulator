#!/bin/bash

filename="./tagNum_test.txt"
./clean.sh
cat $filename | while read LINE
do
	make
	./run $LINE
	python dir_operation.py
	mv tmp/replay/*.csv results/replay_flipGFcetd_gf1_${LINE}.csv
	./clean.sh
done 

#./run
#python dir_operation.py
