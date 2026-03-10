import matplotlib.pyplot as plt
import numpy as np
from scipy import stats 



distribution={0: "Uniforme",1: "Esponenziale", 2: "Lorentziana"}




N={0:1,1:2,2:10,3:100}

#Distribuzione uniforme

data=np.loadtxt("res_uni_dist.csv",delimiter=",",unpack=False, skiprows=1)

fig,axs=plt.subplots(2,2) 

for i,ax in enumerate(axs.flat):
    ax.hist(data[:,i],density=True) #plot della distribuzione, con area normalizzata a 1 
    ax.set_title(f"N={N[i]}")
    ax.set_xlabel("value")
    ax.set_ylabel("counts")

#per N=100, confronto con distribuzione gaussiana
mean=data[:,3].mean()
stdv=data[:,3].std()
max=data[:,3].max()
min=data[:,3].min()
x=np.linspace(min,max,1000) 
gauss=stats.norm.pdf(x,loc=mean,scale=stdv) #distribuzione gaussiana
axs[1,1].plot(x,gauss)
axs[1,1].legend(["Distribuzione gaussiana","Dati"])


fig.suptitle("Distribuzione delle medie di N elementi estratti da una distribuzione uniforme su [0,1]")
plt.tight_layout()
plt.show()






#Distribuzione esponenziale

data=np.loadtxt("res_exp_dist.csv",delimiter=",",unpack=False, skiprows=1)


fig,axs=plt.subplots(2,2) 

for i,ax in enumerate(axs.flat):
    ax.hist(data[:,i],density=True) #plot della distribuzione, con area normalizzata a 1 
    ax.set_title(f"N={N[i]}")
    ax.set_xlabel("value")
    ax.set_ylabel("counts")

#per N=100, confronto con distribuzione gaussiana
mean=data[:,3].mean()
stdv=data[:,3].std()
max=data[:,3].max()
min=data[:,3].min()
x=np.linspace(min,max,1000) 
gauss=stats.norm.pdf(x,loc=mean,scale=stdv) #distribuzione gaussiana
axs[1,1].plot(x,gauss)
axs[1,1].legend(["Distribuzione gaussiana","Dati"])

fig.suptitle("Distribuzione delle medie di N elementi estratti da una distribuzione esponenziale")
plt.tight_layout()
plt.show()




#Distribuzione lorentziana 

data=np.loadtxt("res_lor_dist.csv",delimiter=",",unpack=False, skiprows=1)


fig,axs=plt.subplots(2,2) 

for i,ax in enumerate(axs.flat):
    ax.hist(data[:,i],density=True,bins=200) #plot della distribuzione, con area normalizzata a 1
    ax.set_title(f"N={N[i]}")
    ax.set_xlabel("value")
    ax.set_ylabel("counts")
    ax.set_xlim([-3000,1000])
  

#per N=100, confronto con distribuzione lorentziana
x=np.linspace(-3000,1000,10000) 
lor=stats.cauchy.pdf(x,loc=0,scale=1)#distribuzione lorentziana
axs[1,1].plot(x,lor)
axs[1,1].legend(["Distribuzione lorentziana","Dati"])

fig.suptitle("Distribuzione delle medie di N elementi estratti da una distribuzione lorentziana")
plt.tight_layout()
plt.show()




