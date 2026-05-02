import matplotlib.pyplot as plt
import numpy as np

M=100000 #steps
N=100 #numero blocchi
L=M/N #grandezza blocco 



x=np.loadtxt("x_values.dat") 


plt.hist(x,100)


plt.show()