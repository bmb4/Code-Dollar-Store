#Benjamin Brownstein, bmb4@buffalo.edu
#This code is meant to optimize multiplication efficiency by recursively splitting two numbers in binary form
#until they are at a base value and then summing up all those components to reach the correct answer.
#It was a assignment given to us during the summer 2021 offering of CSE 331, Algorithm analysis and Design at
#SUNY University at Buffalo
#The output is to demonstrate proper splitting and final product of the numbers.
#Portions of this code take from pa_1_suboptimal.py by Prof. Dennis Murphy
import sys



def recursive_multiplication(a,b):
    x = max(a,b)
    y = min(a,b)
    biny = bin(y)
    biny = biny[2:]
    binx = bin(x)
    binx = binx[2:]
    ylen = len(biny)
    xlen = len(binx)
    ns = ylen//2
    if ns <= 1:
        product = x * y
        return product
    xh = binx[:(xlen-ns)]
    xl = binx[(xlen-ns):]
    yh = biny[:(ylen-ns)]
    yl = biny[(ylen-ns):]
    xh = int(xh,2)
    xl = int(xl,2)
    yh = int(yh,2)
    yl = int(yl,2)
    zh = recursive_multiplication(xh,yh)
    cx = (xh+xl)
    cy = (yh+yl)
    zc = recursive_multiplication(cx,cy)
    zl = recursive_multiplication(xl,yl)
    Line =(ns, xh, xl, yh, yl, zh, zc, zl)
    Output.append(Line)
    first = zh << (ns*2)
    mid = zc-zh-zl
    mid = mid << ns
    last = zl
    product = first+mid+last
    return product

    
print(sys.argv[1])
try:
	input_file = open(sys.argv[1], 'r')
except:
	print('Could not open input file\n')
	exit()
Input_Lines = input_file.readlines()
input_file.close()

x = Input_Lines[0]
y = Input_Lines[1]
x = int(x,10)
y = int(y,10)
Output = []
recursive_multiplication(x,y)


output_file = open('output.txt', 'w')
length = len(Output)
for i in range(length):
    output_file.write(str(Output[i][0]) + ',' + str(Output[i][1]) + ',' + str(Output[i][2]) + ',' + str(Output[i][3]) + ',' + str(Output[i][4]) + ',' + str(Output[i][5]) + ',' + str(Output[i][6]) + ',' + str(Output[i][7]) + '\n')
    
output_file.close()



