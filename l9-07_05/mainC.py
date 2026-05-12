import matplotlib.animation as animation 
import matplotlib.pyplot as plt


import numpy as np

parent_folder="l_09_all_es/"


C_cities=np.loadtxt(f"{parent_folder}cities_on_circle.csv",skiprows=1)


cols=range(3,4 + len(C_cities))

gen=np.loadtxt(f"{parent_folder}circle_paths.csv",usecols=0,skiprows=1)

lenghts=np.loadtxt(f"{parent_folder}circle_paths.csv",usecols=1,skiprows=1)

paths=np.loadtxt(f"{parent_folder}circle_paths.csv",usecols=cols,skiprows=1)



C_paths={"gen" :gen,"len": lenghts, "paths": paths}




N_gen=1000
R=1
fig,ax=plt.subplots(1,1,figsize=[6,6])

theta=np.linspace(0,2*np.pi,1000)
ax.plot(R*np.cos(theta),R*np.sin(theta),color="black",zorder=0)#circonferenza, per confronto
ax.scatter(C_cities[:,0],C_cities[:,1],color="blue",zorder=2)#città
ax.scatter(C_cities[0,0],C_cities[0,1],color="navy",s=100,zorder=2)#città di partenza
ax.text(C_cities[0,0] + 0.05, C_cities[0,1] + 0.05, "Partenza", fontsize=10, ha='left',color="navy")#segna la città di partenza

index=C_paths["paths"][0].astype(int)#ordine in cui le città vengono visitate (percorso)
path=ax.plot(C_cities[index,0],C_cities[index,1],color="dodgerblue",zorder=1)[0]#unisce le città nell'ordine in cui vengono visitate
ax.set_title(fr"Generazione: 1  | Lunghezza miglior percorso $L$={C_paths["len"][0]:.4f}")


def update(frame):
    index=C_paths["paths"][frame].astype(int)#ordine in cui le città vengono visitate (percorso)
    path.set_xdata(C_cities[index,0])
    path.set_ydata(C_cities[index,1])
    ax.set_title(fr"Generazione: {frame}  | Lunghezza miglior percorso $L$={C_paths["len"][frame]:.4f}")

    return path
    

ax.grid()
ax.set_xlabel("x")
ax.set_ylabel("y")



anim = animation.FuncAnimation(fig=fig, func=update, frames=400, interval=30) 
anim.save("circle.gif")

# saves the animation in our desktop 
