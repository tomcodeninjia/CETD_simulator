filename="./CETD_simulator/cetd_para.txt"

i=""
line_num=$(cat ${filename} | wc -l)
for k in $(seq 1 ${line_num})
do 
	i=$(sed -n "${k}p" ${filename})	
	echo ${i}
done
