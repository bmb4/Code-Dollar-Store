#Portions of this code taken from PA1_suboptimal.py by Dennis Murphy
#Code otherwise written by:
#Benjamin Brownstein
#bmb4@buffalo.edu
#My solution to the 0/1 knapsack problem.

import sys


def solver(Input):
	bw_cap = Input.pop(0)
	client_ct = Input.pop(0)
	val_matrix = []
	for i in range(client_ct):
		sublist = [0]*bw_cap
		val_matrix.append(sublist)
	keepers = []
	values = Input
	for i in values:
		value = i[0]/i[1]
		i.append(value)
	for client in range(client_ct):
		for bw_avail in range(bw_cap):
			if bw_avail < values[client][1]:
				val_matrix[client][bw_avail] = val_matrix[client-1][bw_avail]
			else:
				val_matrix[client][bw_avail] = max(val_matrix[client-1][bw_avail],values[client][2] + val_matrix[client-1][bw_avail-values[client][1]])
	client = client_ct-1
	bw = bw_cap-1
	while client >= 0:
		if val_matrix[client][bw] == 0:
			client = -1
		elif val_matrix[client][bw] == val_matrix[client-1][bw]:
			if client == 0:
				keepers.insert(0,client)
				client -=1
			else:
				client -=1
		elif val_matrix[client][bw] > val_matrix[client-1][bw]:
			keepers.insert(0,client)
			bw -= values[client][1]
			client -= 1
		elif client == 0:
			keepers.insert(0,client)
			client -=1
		else:
			print("uh-oh some edge case I didn't think of has occured")
			client -=1
	keepers.insert(0,len(keepers))
	return keepers

try:
	input_file = open(sys.argv[1], 'r')
except:
	print('Could not open input file\n')
	exit()
Input_Lines = input_file.readlines()
input_file.close()

Input = []
Input.append(int(Input_Lines[0]))
Input.append(int(Input_Lines[1]))
for i in range(Input[1]):
	 Client = Input_Lines[2 + i].strip('\n').split(',')
	 Input.append([int(Client[0]), int(Client[1])])

Output = solver(Input)

output_file = open('output.txt', 'w')
for i in range(len(Output)):
	output_file.write(str(Output[i])+'\n')
output_file.close()

