import matplotlib.pyplot as plt
import numpy as np

M=10000 #numero estrazioni
N=100 #numero blocchi

I_u,error_u,I_is,error_is=np.loadtxt("results.csv", usecols=(0,1,2,3), delimiter=',', unpack='true',skiprows=1) #carica i dati, saltando la riga del titolo

n=np.arange(1,N+1) #progressivo dei blocchi
n*=int(M/N) #numero estrazioni

#traccia il grafico dello scostamento dell'integrale dal valore atteso in funzione del numero di blocchi, con le rispettive barre di errore
plt.errorbar(n,I_u-1,error_u,color="teal") #sampling uniforme
plt.errorbar(n,I_is-1,error_is,color="blue") #importance sampling


plt.title(fr"Scostamento di $I$ calcolato col metodo Montecarlo dal valore atteso (1)")
plt.xlabel("Numero lanci")
plt.ylabel(rf"$I-1$")
plt.legend(["Samplig uniforme su [0,1)", rf"Sampling con $d(x)=2(1-x) su [0,1)$"])
plt.grid(True)
plt.show()