import os
import pyResAna.repeat_count as repeat_count
import csv

src_dir = "./short" 
nonce_collide_test = True

#the filename is stored in the variable "filenames"
result_dir = "./tmp/"

name_list = os.listdir(src_dir)
name_list.sort()

if name_list[0][1] == "_":
	result_filename = "nonce_collide_n%d.csv"

	nonce_id = 0
	csvname = result_dir+"/"+result_filename % (nonce_id)
	csvFile = open(csvname,'wb')
	writer = csv.writer(csvFile, dialect='excel')

	for i in range(0, len(name_list)):
		if int(name_list[i][0]) != int(nonce_id):
			csvFile.close()
			nonce_id = nonce_id +1
			csvname = result_dir+"/"+result_filename % (nonce_id)
			csvFile = open(csvname,'wb')
			writer = csv.writer(csvFile, dialect='excel')

		f = file(src_dir+"/"+name_list[i],"r")
		data_list = []
		data_list = f.readlines()
	
		my_set = set(data_list)
	
		distinct_vn = len(my_set)
		writer.writerow([name_list[i],distinct_vn])

	csvFile.close()
else:
	result_filename = "frequency_new.csv"
	csvname = result_dir+"/"+result_filename 
	csvFile = open(csvname,'wb')
	writer = csv.writer(csvFile, dialect='excel')

	for i in range(0, len(name_list)):
		#repeat_count.repeat_count(src_dir,filenames,dst_dir,"count_"+filenames)
		
		f = file(src_dir+"/"+name_list[i],"r")
		data_list = []
		data_list = f.readlines()
	
		my_set = set(data_list)
	
		distinct_vn = len(my_set)
		writer.writerow([name_list[i],distinct_vn])

	
print "Finished"
#print filenames


