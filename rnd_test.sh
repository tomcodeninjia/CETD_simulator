#!/bin/bash

cetd_dir="./CETD_simulator/"
nist_dir="./NIST_suite/"
cetd_para="cetd_para.txt"
nist_para="nist_para.txt"

i=""
line_num=$(cat ${cetd_dir}${filename} | wc -l)
for k in $(seq 1 ${line_num})
do 
	cd ${cetd_dir}
	filename=${cetd_para}
	i=$(sed -n "${k}p" ${filename})	
	./clean.sh
	make
	./run ${i} 
	cd ../
	cd ${nist_dir}
	filename=${nist_para}
	i=$(sed -n "${k}p" ${filename})	
	./clean.sh
	make
	./assess ${i}
done

#cd ${cetd_dir}
#./run 2 2 1000 2
#cd ../
#cd ${nist_dir}
#./assess 8000
