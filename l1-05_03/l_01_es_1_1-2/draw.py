import matplotlib.pyplot as plt
import numpy as np

M=100000 #steps
N=100 #numero blocchi
L=M/N #grandezza blocco 



mean,mean_err,var,var_err=np.loadtxt("results.csv", delimiter=',', unpack='true',skiprows=1) #carica i dati, saltando la riga del titolo

n=np.arange(1,N-1)
n*=int(L)


#es 1.1: 

plt.errorbar(n,mean-1/2,mean_err) #plot delle oscillazioni della media (col loro errore) attorno al valore atteso (1/2)

plt.title("Valor medio")
plt.grid(True)
plt.xlabel("Counts")
plt.ylabel(r"$ <r> - 1/2$")
plt.show()


#es 1.2:


plt.errorbar(n,var-1/12,var_err)#plot delle oscillazioni della varianza (col loro errore) attorno al valore atteso (1/12)

plt.title("Varianza")
plt.grid(True)
plt.xlabel("Counts")
plt.ylabel(r"$ <(r-1/2)^2>-1/12$")
plt.show()