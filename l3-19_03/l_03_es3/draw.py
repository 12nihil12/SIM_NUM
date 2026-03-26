import matplotlib.pyplot as plt
import numpy as np

M=100000 #numero estrazioni
N=100 #numero blocchi

titles={0:"Call option (metodo diretto)", 1: "Put option (metodo diretto)", 2:"Call option (con discretizzazione)", 3: "Put option (con discretizzazione)"}
mean={0: call, 1:put}

prices=np.loadtxt("results.csv", usecols=(0,2,4,6), delimiter=',', unpack='true',skiprows=1) #carica i dati, saltando la riga del titolo
errors=np.loadtxt("results.csv", usecols=(1,3,5,7), delimiter=',', unpack='true',skiprows=1) #carica i dati, saltando la riga del titolo

n=np.arange(1,N+1) #progressivo dei blocchi


fig,axs=plt.su
bplots(2,2,figsize=[20,12])

for i,ax in enumerate(axs.flatten()):
    ax.errorbar(n,prices[i][:],errors[i][:])
    ax.hlines(mean[i%2],0,N,color="red")
    ax.set_title(titles[i])
    ax.set_xlabel("Progressivo blocchi")
    ax.set_ylabel("Prezzo")
    ax.legend(["Valore atteso","Simulazione"])
    ax.grid(True)

plt.suptitle(f"Stima prezzo opzioni europee,con M={M} ")
plt.show()


