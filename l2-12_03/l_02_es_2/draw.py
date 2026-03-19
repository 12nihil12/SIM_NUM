import matplotlib.pyplot as plt
import numpy as np
import scipy.optimize as scipy


#funzione per il fit
def f(x,k): 
    return k*np.sqrt(x)

data_grid,error_grid,data_cont,error_cont=np.loadtxt("results.csv", usecols=(0,1,2,3), delimiter=',', unpack='true',skiprows=1) #carica i dati, saltando la riga del titolo


fig,axs=plt.subplots(1,2,figsize=[15,7])


M=10000 #numero random walk generati
N=100 #numero blocchi
R=100#numero step 


n=np.arange(1,R+1) #progressivo del numero di step 


#Random walk su griglia

axs[0].errorbar(n,data_grid,error_grid,marker=".",linestyle="none",color="blue") #plot dati

#fit e relativo plot
k_fit=scipy.curve_fit(f,n,data_grid)[0][0] #fitta i dati alla funzione k*sqrt(x) e restituisce il parametro k
x=np.linspace(0,R,R*10)
axs[0].plot(x,f(x,k_fit),color="black") 

axs[0].set_title(fr"RW su griglia discreta")
axs[0].set_xlabel("Numero passi")
axs[0].set_ylabel(r"$\sqrt{<|r_N|^2>}$")
axs[0].legend([rf"Fit: $f(x)=k \sqrt{{x}} $    k={k_fit:.3f} ","Dati generati"])
axs[0].grid(True)


#Random walk nel continuo 

axs[1].errorbar(n,data_cont,error_cont,marker=".",linestyle="none",color="blue") #plot dati

#fit e relativo plot
k_fit=scipy.curve_fit(f,n,data_cont)[0][0] #fitta i dati alla funzione k*sqrt(x) e restituisce il parametro k
x=np.linspace(0,R,R*10)
axs[1].plot(x,f(x,k_fit),color="black")

axs[1].set_title(fr"RW nel continuo")
axs[1].set_xlabel("Numero passi")
axs[1].set_ylabel(r"$\sqrt{<|r_N|^2>}$")
axs[1].legend([ rf"Fit: $f(x)=k \sqrt{{x}} $   k={k_fit:.3f}","Dati generati"])
axs[1].grid(True)


plt.suptitle(rf"Andamento di $\sqrt{{<|r_i|^2>}}$ al variare del numero di passi $i\in [1,{R}]$  con M={M} random walk generati e N={N} blocchi")
plt.show()