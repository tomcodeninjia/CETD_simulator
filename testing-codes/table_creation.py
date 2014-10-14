#!/usr/bin/python
import os
import csv
import numpy as np

"""
creat a table for data in files in a dir
"""
#data_dir = "./flipgf4/flipgf4-1"
data_dir_root = "./"
data_dir_name = "flipgf4"

analysis_dir = "./"

#filename_list = os.listdir(data_dir+"/"+sub_dir)
data_dir = os.path.join(data_dir_root, data_dir_name)
dir_list = os.listdir(data_dir)
dir_list.sort()
#print dir_list
dir_num = len(dir_list)
#print dir_num
data_table = {}
tmp_path=0
for i in range(0,dir_num):# check each sub-dir of data_dir
	path = os.path.join(data_dir,dir_list[i])
	print path
	tmp_path = os.path.isdir(path)
	data_table = {}#a table for each sub-dir
	if tmp_path: #data_dir has sub-dir
		filename_list = os.listdir(path)
		filename_num = len(filename_list)
		for j in range(0,filename_num):
			fn = os.path.join(path,filename_list[j])
			f = file(fn,"r")
			csvreader = csv.reader(f)	
			for line in csvreader:
				tmp_data = float(line[2])
				if j==0:
					data_table[line[0]]=[[line[1]],[tmp_data]]
				else:
					data_table[line[0]][0].append(line[1]) 
					data_table[line[0]][1].append(tmp_data)
#   data_table for dir[i] is ready, to statistic then
		csvname = os.path.join(data_dir_root,dir_list[i])+".csv"
		print csvname
		csvFile = open(csvname,'wb')
		writer = csv.writer(csvFile,dialect='excel')
		writer.writerow(['nist test name','pvalue min','pvalue max','pvalue variance','pvalue mean','pass rate'])
		for key in data_table:
			pr = float(data_table[key][0].count('SUCCESS'))/len(data_table[key][0])
			writer.writerow([key,min(data_table[key][1]), max(data_table[key][1]),np.var(data_table[key][1]),np.mean(data_table[key][1]),pr])
		csvFile.close()
	else: #data_dir is the root dir
	  	fn = path
		f = file(fn, "r")
		csvreader = csv.reader(f)	
		for line in csvreader:
			tmp_data = float(line[2])
			if i==0:
#				data_table[line[0]]=[(line[1],tmp_data)]
				data_table[line[0]]=[[line[1]],[tmp_data]]
			else:
				data_table[line[0]][0].append(line[1]) 
				data_table[line[0]][1].append(tmp_data)
#				data_table[line[0]].append((line[1],tmp_data)) 
if tmp_path==False:
# data_table ready, do statistic work then
	csvname = os.path.join(analysis_dir,data_dir_name)+".csv"
	csvFile = open(csvname,'wb')
	writer = csv.writer(csvFile,dialect='excel')
	writer.writerow(['nist test name','pvalue min','pvalue max','pvalue variance','pvalue mean','pass rate'])
	for key in data_table:
		pr = float(data_table[key][0].count('SUCCESS'))/len(data_table[key][0])
		writer.writerow([key,min(data_table[key][1]), max(data_table[key][1]),np.var(data_table[key][1]),np.mean(data_table[key][1]),pr])
	csvFile.close()
print "finished"
#		print fn
"""
		f = file()
		tup = ()
"""
#	print filename_num

"""
data_table = {}
key1 = 'liang_bra'
data_table[key1]=[(1,1),(2,2)]
"""
