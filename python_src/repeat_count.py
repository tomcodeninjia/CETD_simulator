#count the No. of distinct values in a file, store the result to a csv file
#count the No. of each distinct value
import csv

#@para srcDirName: the name of dir, ex "./short"
#@para srcFileName: the name of a src file (without extension), ex "test_txt"
#@para dstDirName
#@para dstFilename
def repeat_count(srcDirName,srcFileName, dstDirName, dstFileName):
	f = file(srcDirName+"/"+srcFileName,"r")
	data_list = []
	data_list = f.readlines()
#print len(data_list)
	
	#my_set contains the distinct values in data_list
	my_set = set(data_list)
	
	#static is a dict, the element is 'value:No. of value'
	static = {}
	for item in my_set:
		static[item]  = data_list.count(item)
	
	result_file = dstDirName+"/"+dstFileName
	csvFile = open(result_file,'wb')
	writer = csv.writer(csvFile, dialect='excel')
	
	#writer.writerow(['col1','col2'])#write a head for each column
	for key in static:
		writer.writerow([key,static[key]])




