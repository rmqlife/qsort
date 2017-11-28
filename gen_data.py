from random import random
import sys
num = int(sys.argv[1])
data_int = int(sys.argv[2])
print num
for i in range(num):    
    if data_int:
        print int(random()*num)
    else:
        print random()
