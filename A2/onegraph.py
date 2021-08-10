import matplotlib.pyplot as plt
import numpy as np
index = 0
count = 0
ff = []
with open("outfile.txt", "r") as f:
    for ii in f.readlines():
        ff.append(float(ii[:-1]))
l = []
	
l = [j for j in range(3000)]
plt.plot(l, ff)
# naming the x axis
plt.xlabel('update number')
# naming the y axis
plt.ylabel('CW value')

# giving a title to my graph
plt.title("graph")

plt.savefig("example.png" , dpi =300 , bbox_inches = 'tight')

plt.close()

