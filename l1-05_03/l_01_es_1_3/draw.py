import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import chi2

M=100 #numero sottointervalli
n=10000 #numero blocchi
R=100 #numero test


chi_square=np.loadtxt("results.csv",delimiter=",",unpack=True,skiprows=1)


fig,axs=plt.subplots(1,2)

#Mostra l'istogramma dei primi 100 valori di chi_quadro calcolati
axs[0].hist(chi_square[:100],density=False)
axs[0].set_title(rf"Istogramma dei {R} valori di $X^2$")
axs[0].set_xlabel("value")
axs[0].set_ylabel("counts")
axs[0].grid(True)



#mostra l'istogramma (con area normalizzata ad 1 ) di 10000 valori di chi_quadro per confronto con distribuzione teorica
min=chi_square.min()
max=chi_square.max()
x=np.linspace(min,max,1000)
axs[1].hist(chi_square,density=True,label="Dati") 
axs[1].plot(x,chi2.pdf(x,M-1),label="Distribuzione teorica") #distribuzione teorica (df=M-1)
axs[1].set_title(rf"Confronto con distribuzione teorica")
axs[1].legend()
axs[1].set_xlabel("value")
axs[1].set_ylabel("density")
axs[1].grid(True)

plt.show()