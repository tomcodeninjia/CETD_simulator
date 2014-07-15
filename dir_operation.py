import os
import pyResAna.repeat_count as repeat_count
import csv

src_dir = "./short" 
nonce_collide_test = True

#the filename is stored in the variable "filenames"
result_dir = "./tmp/"

name_list = os.listdir(src_dir)
name_list.sort()
nonce_num = len(name_list)/34

#statistic the No. of dsitinct tags for each proportion for each nonce_id
#nonce_id: 0 to x

if name_list[0][1] == "_":
	result_filename = "nonce_collide_n%d"
	test_type = "nonceCollide"

	nonce_id = 0
	csvname = result_dir+"/"+test_type+"/"+result_filename % (nonce_id)+".csv" 
	csvFile = open(csvname,'wb')
	writer = csv.writer(csvFile, dialect='excel')
	for i in range(0, len(name_list)):
		nonce_index = name_list[i].find('_')		
		nonce_id_tmp = name_list[i][0:nonce_index]
		if int(nonce_id_tmp) != nonce_id: #new nonce_id
			csvFile.close()
			nonce_id = int(nonce_id_tmp) 
			csvname = result_dir+"/"+test_type+"/"+result_filename % (nonce_id)+".csv" 
			csvFile = open(csvname,'wb')
			writer = csv.writer(csvFile, dialect='excel')
		f = file(src_dir+"/"+name_list[i],"r")
		data_list = []
		data_list = f.readlines()
		my_set = set(data_list)
		distinct_vn = len(my_set)
		index=name_list[i].find('_')
		writer.writerow([name_list[i][index+1:],distinct_vn])
	csvFile.close()


#static the aver of No. of distinct tags for each proportion
	result_filename = "nonce_collide_aver.csv"
	csvname = result_dir+"/"+result_filename 
	csvFile = open(csvname,'wb')
	writer = csv.writer(csvFile, dialect='excel')
	freq_list = {}
	name_list = os.listdir(result_dir+"/"+test_type)
	name_list.sort()
	f = file(result_dir+"/"+test_type+"/"+name_list[0])#read the result file of nonce_i
	for line in f: #read each line of file nonce_i
		  	index = line.find(',')
		  	key = line[0:index-4]
			freq_list[key] = int(line[index+1:])
	for i in range(1,len(name_list)):
	  	f = file(result_dir+"/"+test_type+"/"+name_list[i])#read the result file of nonce_i
		for line in f: #read each line of file nonce_i
		  	index = line.find(',')
		  	key = line[0:index-4]
			freq_list[key] = freq_list[key] + int(line[index+1:])
	print nonce_num
	print freq_list
	for ele in freq_list:
		writer.writerow([ele, freq_list[ele]/nonce_num])
	csvFile.close()
else:
#replay test: static the No. of distinct tags for each distinct input 
	test_type = "replay"
	result_filename = "frequency_new.csv"
	csvname = result_dir+"/"+test_type+"/"+result_filename 
	csvFile = open(csvname,'wb')
	writer = csv.writer(csvFile, dialect='excel')
	for i in range(0, len(name_list)):
		#repeat_count.repeat_count(src_dir,filenames,dst_dir,"count_"+filenames)
		f = file(src_dir+"/"+name_list[i],"r")
		data_list = []
		data_list = f.readlines()
		my_set = set(data_list)
		distinct_vn = len(my_set)
#		writer.writerow([name_list[i],distinct_vn])
		writer.writerow([i,distinct_vn])
#totally 65536 distinct inputs, then should be 65536 lines in the statistic csv file
	csvFile.close()
	
print "Finished"
#print filenames


