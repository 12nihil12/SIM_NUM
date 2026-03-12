import matplotlib.pyplot as plt
import numpy as np


M=10000 #numero lanci 
R=100 #numero blocchi

I_u,error_u=np.loadtxt("results.csv", usecols=(0,1), delimiter=',', unpack='true',skiprows=1) #carica i dati, saltando la riga del titolo

n=np.arange(1,R+1) #progressivo dei blocchi

plt.errorbar(n,I_u-1,error_u) 


plt.title(fr"")
plt.xlabel("Numero passi")
plt.ylabel(r"$\sqrt{<|r_N|^2>}$")
plt.grid(True)
plt.show()