import os
import pyResAna.repeat_count as repeat_count
import csv

src_dir = "./short" 
dst_dir = "./frequency"

#the filename is stored in the variable "filenames"
#for each filename, 
result_file = "./frequency_count.csv" 
csvFile = open(result_file,'wb')
writer = csv.writer(csvFile, dialect='excel')

for filenames in os.listdir(src_dir):
	#repeat_count.repeat_count(src_dir,filenames,dst_dir,"count_"+filenames)
	
	f = file(src_dir+"/"+filenames,"r")
	data_list = []
	data_list = f.readlines()

	my_set = set(data_list)

	distinct_vn = len(my_set)
	writer.writerow([filenames,distinct_vn])


	
print "Finished"
#print filenames


