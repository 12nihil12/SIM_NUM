import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


data=pd.read_csv("../NSL_SIMULATOR/OUTPUT/pofv.dat",sep="\s+")



temp=1.1
N=10 # numero blocchi
M=1000 # numero step 
n_BINS=30 #numero bins

mask=data["#"]==N #seleziona l'ultimo blocco

values=data[mask]

bins=values["VELOCITY:"].to_numpy()
bin_size=bins[1]-bins[0]
print(bins)
print(len(bins))

#bins=np.append(bins,bins[len(bins)-1]+bin_size)
print(len(bins))
counts=values["AVE_POFV:"]
error=values["ERROR:"]
plt.bar(bins,counts,yerr=error)
print()
print(values)
plt.show()