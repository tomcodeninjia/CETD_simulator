#!/bin/bash

#filename="./tag_num.txt"
#./clean.sh
#cat $filename | while read LINE
#do
#	make
#	./run $LINE
#	python dir_operation.py
#	mv tmp/replay/*.csv results/${LINE}_flipGFcetd_gf1.csv
#	./clean.sh
#done 

##!/bin/bash
#
#cetd_dir="./CETD_simulator/"
#nist_dir="./NIST_suite/"
#cetd_para="cetd_para.txt"
#nist_para="nist_para.txt"

blk_len=4
filename="./cetd_para.txt"
i=""
line_num=$(cat ${filename} | wc -l)
./clean.sh
for k in $(seq 1 ${line_num})
do 
	make
	i=$(sed -n "${k}p" ${filename})	
	./run ${i} #results stored in NIST/short 
	python dir_operation.py
	mv tmp/replay/*.csv results/${k}gf_flipGFcetd${blk_len}.csv
	./clean.sh
done
#./run
#python dir_operation.py
