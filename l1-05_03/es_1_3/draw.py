import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from scipy.stats import chi2

M=100 #numero sottointervalli
n=10000 #numero blocchi
R=100 #numero test



data=pd.read_csv("results.csv",usecols=["chi_squared"])

counts,bins=np.histogram(data["chi_squared"])

fig,axs=plt.subplots(1,2)

axs[0].hist(data["chi_squared"],density=False)
axs[0].set_title(rf"Istogramma dei {R} valori di $X^2$")
axs[0].set_xlabel("value")
axs[0].set_ylabel("counts")

axs[0].grid(True)

x=np.linspace(60,150,1000)

axs[1].hist(data["chi_squared"],density=True)
axs[1].plot(x,chi2.pdf(x,M-2))
axs[1].set_xlabel("value")
axs[1].set_ylabel("density")


axs[1].grid(True)

plt.show()