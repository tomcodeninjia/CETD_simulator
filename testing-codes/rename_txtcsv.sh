#rename 's/\.txt$/\.csv/' *.txt

#!/bin/bash

#http://blog.csdn.net/longxibendi
find . -name *.txt  | while read i
do
        echo "$i";
        mv $i.txt  $i.csv
done
