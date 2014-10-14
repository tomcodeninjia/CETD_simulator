#!/bin/bash

cetd_dir="./CETD_simulator/"
nist_dir="./NIST_suite/"
cetd_para="cetd_para.txt"
nist_para="nist_para.txt"
nist_result="data/short"
dist_result="short"
result_dir="dist_replay"
design="cetd"
input_len=4

filename="./CETD_simulator/cetd_para.txt"
i=""
line_num=$(cat ${filename} | wc -l)
for k in $(seq 1 ${line_num})
do 
	cd ${cetd_dir}
	filename=${cetd_para}
	i=$(sed -n "${k}p" ${filename})	
	./run ${i} #results stored in NIST/short 
	mv ${dist_result} ../${result_dir}/${design}$input_len-${k}
	mkdir $dist_result
	./clean.sh
	make
	cd ../
done

#cd ${cetd_dir}
#./run 2 2 1000 2
#cd ../
#cd ${nist_dir}
#./assess 8000
