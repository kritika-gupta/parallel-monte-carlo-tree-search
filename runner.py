import os
import subprocess

p = 4
n = 1500
base = os.getcwd()
print base
serial_file = base + "/tictactoe.cpp"
parallel_file
# os.system("g++ -g -fopenmp -std=c++11  -o tic")
# os.system("g++ -g -fopenmp -std=c++11 tictactoe_leaf.cpp -o leaf")
# for i in range(0, int(n)):

#     serial_out = subprocess.check_output("./tic")
#     run = "./leaf "+str(n)+" "+str(p)
#     print run
#     parallel_out = subprocess.check_output(run)
#     print serial_out
#     print parallel_out
#     # if res==3:
#     #     success+=1
#     # print i
    
# #print "Success rate = ", float(success)/n