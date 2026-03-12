import matplotlib.pyplot as plt
import numpy as np

M=10000 #numero lanci 
N=100 #numero blocchi

I,error=np.loadtxt("results.csv", usecols=(0,1), delimiter=',', unpack='true',skiprows=1) #carica i dati, saltando la riga del titolo

n=np.arange(1,N+1) #progressivo dei blocchi
n*=int(M/N) #numero lanci

plt.errorbar(n,I-1,error) 

plt.title(fr"Scostamento di $I$ calcolato col metodo Montecarlo dal valore atteso (1)")
plt.xlabel("Numero lanci")
plt.ylabel(rf"$I-1$")
plt.grid(True)
plt.show()