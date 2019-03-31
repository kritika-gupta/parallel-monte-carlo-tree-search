import os
import subprocess
import sys
import numpy

if(len(sys.argv)==1):
    print "Usage : python runner.py processors runs"
    exit
    
maxProc = int(sys.argv[1])
runs = int(sys.argv[2])

processors = [i for i in range(0, maxProc+1)]
problem_size = [10**i for i in range(2, 4)] + [5*(10**i) for i in range(1, 4)]
problem_size.sort()
base = os.getcwd()

serial_file = base + "/tictactoe.cpp"
leaf_file = base + "/tictactoe_leaf.cpp"
root_file = base + "/tictactoe_root.cpp"
serial_exec = base + "/tic"
leaf_exec = base + "/leaf"
root_exec = base + "/root"
os.system("g++ -g -fopenmp -std=c++11 " + serial_file + " -o " + serial_exec)
os.system("g++ -g -fopenmp -std=c++11 " + leaf_file + " -o " + leaf_exec)
os.system("g++ -g -fopenmp -std=c++11 " + root_file + " -o " + root_exec)


serial_success, leaf_success, root_success = {}, {}, {}

avg_serial_success, avg_leaf_success, avg_root_success = {}, {}, {}

serial_time, leaf_time, root_time = {}, {}, {}

avg_serial_time, avg_leaf_time, avg_root_time = {}, {}, {}

leaf_speedup, root_speedup = {}, {}

for p in processors:
    for n in problem_size:
        serial_success[(n, p)] = 0
        leaf_success[(n, p)] = 0
        root_success[(n, p)] = 0

        serial_time[(n, p)] = 0
        leaf_time[(n, p)] = 0
        root_time[(n, p)] = 0


for r in range(runs):
    print "Run : ", r

    for p in processors:
        print "cores : ", p
        
        for n in problem_size:
            print "Problem size : ", n

            serial_out = map(float, subprocess.check_output([serial_exec, str(n)]).split())
            print "serial done"  
            leaf_out = map(float, subprocess.check_output([leaf_exec, str(n/p), str(p)]).split())
            print "leaf done"
            root_out = map(float, subprocess.check_output([root_exec, str(n/p), str(p)]).split())
            print "root done"

            serial_result = serial_out[0]
            leaf_result = leaf_out[0]
            root_result = root_out[0]

            #print "parallel time  = ", leaf_out[1], " serial time = ", serial_out[1], "speedup = ", serial_out[1]/leaf_out[1], "\n"
            serial_time[(n, p)] += serial_out[1]
            leaf_time[(n, p)] += leaf_out[1]
            root_time[(n, p)] += root_out[1]
            
            if serial_result == 3:
                serial_success[(n, p)] += 1
            if leaf_result == 3:
                leaf_success[(n, p)] += 1
            if root_result == 3:
                root_success[(n, p)] += 1


for p in processors:
    
    for i in problem_size:
        
        avg_serial_success[(i, p)] = (serial_success[(i, p)])/float(runs)
        avg_leaf_success[(i, p)] = (leaf_success[(i, p)])/float(runs)
        avg_root_success[(i, p)] = (root_success[(i, p)])/float(runs)

        avg_serial_time[(i, p)] = (serial_time[(i, p)])/float(runs)
        avg_leaf_time[(i, p)] = (leaf_time[(i, p)])/float(runs)
        avg_root_time[(i, p)] = (root_time[(i, p)])/float(runs)

        leaf_speedup[(i, p)] = (avg_serial_time[(i, p)])/(avg_leaf_time[(i, p)])
        root_speedup[(i, p)] = (avg_serial_time[(i, p)])/(avg_root_time[(i, p)])


        print "\n\nProblem size : ", i, "cores = ", p
        print "Avg. serial success rate : ", avg_serial_success[(i, p)]
        print "Avg. leaf success rate : ", avg_leaf_success[(i, p)]
        print "Avg. root success rate : ", avg_root_success[(i, p)]


        print "Leaf Speedup = ", leaf_speedup[(i, p)]
        print "Root Speedup = ", root_speedup[(i, p)]    
    

