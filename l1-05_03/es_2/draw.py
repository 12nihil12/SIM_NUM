import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

M=100 #numero sottointervalli
n=10000 #numero blocchi
R=100 #grandezza blocco 



data=pd.read_csv("results.csv",usecols=["chi_squared"])

counts,bins=np.histogram(data["chi_squared"])

plt.stairs(counts,bins,fill=True)

plt.grid(True)
plt.xlabel("value")
plt.ylabel("counts")
plt.show()