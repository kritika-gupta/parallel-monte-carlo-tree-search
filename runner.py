import os
import subprocess

success = 0
n = 1
os.system("g++ -g -fopenmp -std=c++11 tictactoe.cpp -o tic")
os.system("g++ -g -fopenmp -std=c++11 tictactoe_leaf.cpp -o leaf")
for i in range(0, int(n)):

    serial = subprocess.call("./tic")
    parallel = subprocess.call("./leaf")
    if res==3:
        success+=1
    print i
    
#print "Success rate = ", float(success)/n