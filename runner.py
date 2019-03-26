import os
import subprocess

success = 0
n = 1e3
for i in range(0, int(n)):
    res = subprocess.call("./tic")
    if res==3:
        success+=1
    print i
    
print "Success rate = ", float(success)/n