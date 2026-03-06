import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

M=100000 #steps
N=100 #numero blocchi
L=M/N #grandezza blocco 



data=pd.read_csv("results.csv",usecols=["cum_avg","error"])

n=np.arange(N)
n+=1
n*=int(L)


plt.errorbar(n,data["cum_avg"]-1/2,data["error"])
plt.grid(True)
plt.xlabel("Counts")
plt.ylabel(r"$ <r> - 1/2$")
plt.show()