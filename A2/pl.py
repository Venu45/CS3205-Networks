import matplotlib.pyplot as plt
import numpy as np
index = 0
count = 0

for i in range(1,33):
	ff = []
	count =0
	file_name = "venu_out" + str(i) + ".txt"
	with open(file_name, "r") as f:
		for ii in f.readlines():
			if count==0:
				param = ii[:-1]
				count = count + 1
			else :
				ff.append(float(ii[:-1]))

	l = []
	
	l = [j for j in range(3000)]
	plt.plot(l, ff)
	# naming the x axis
	plt.xlabel('update number')
	# naming the y axis
	plt.ylabel('CW value')

	# giving a title to my graph
	plt.title(param)

	name = "venu_plot" + str(i) + ".png" 

	plt.savefig(name , dpi =300 , bbox_inches = 'tight')

	plt.close()






