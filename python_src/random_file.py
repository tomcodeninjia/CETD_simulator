import random
#store random numbers to a file

#dirName: "./short"
def randomNumFile(dirName, fileNum, lineNum, startN, endN):
	for i in range(fileNum):
		dirname = dirName 
		filename = dirname+"/test_txt"+str(i)+".txt"
		f = open(filename,"w")
		for j in range(lineNum):
			f.write(str(random.randint(startN,endN))+"\n")	
		f.close()


