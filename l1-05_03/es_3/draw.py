import matplotlib.pyplot as plt
import numpy as np

M=10000 #numero lanci per blocco
N=100 #numero blocchi

mean,error=np.loadtxt("results.csv", usecols=(0,1), delimiter=',', unpack='true',skiprows=1) #carica i dati, saltando la riga del titolo

n=np.arange(N) #progressivo dei blocchi
plt.errorbar(n,mean-np.pi,error) 

plt.title(fr"Scostamento di $\pi_B$ calcolato col metodo di Buffon dal valore atteso, con {M} lanci per blocco")
plt.xlabel("Numero blocchi considerati ")
plt.ylabel(rf"$\pi_B - \pi$")
plt.grid(True)
plt.show()