import matplotlib.animation as animation 
import matplotlib.pyplot as plt


import numpy as np

parent_folder="l_09_all_es/"


SQ_cities=np.loadtxt(f"{parent_folder}cities_in_square.csv",skiprows=1)


cols=range(3,4 + len(SQ_cities))

gen=np.loadtxt(f"{parent_folder}square_paths.csv",usecols=0,skiprows=1)

lenghts=np.loadtxt(f"{parent_folder}square_paths.csv",usecols=1,skiprows=1)

paths=np.loadtxt(f"{parent_folder}square_paths.csv",usecols=cols,skiprows=1)

SQ_paths={"gen" :gen,"len": lenghts, "paths": paths}




N_gen=1000
fig,ax=plt.subplots(1,1,figsize=[6,6])

ax.scatter(SQ_cities[:,0],SQ_cities[:,1],color="blue",zorder=2)#città 

ax.scatter(SQ_cities[0,0],SQ_cities[0,1],color="navy",s=100,zorder=2)#città di partenza

ax.text(SQ_cities[0,0] + 0.05, SQ_cities[0,1] + 0.03, "Partenza", fontsize=10, ha='right',color="navy")#segna la città di partenza


index=SQ_paths["paths"][0].astype(int)#ordine in cui le città vengono visitate (percorso)
path=ax.plot(SQ_cities[index,0],SQ_cities[index,1],color="dodgerblue",zorder=1)[0]#unisce le città nell'ordine in cui vengono visitate
ax.set_title(fr"Generazione: 1  | Lunghezza miglior percorso $L$={SQ_paths["len"][0]:.4f}")


def update(frame):
    index=SQ_paths["paths"][frame].astype(int)#ordine in cui le città vengono visitate (percorso)
    path.set_xdata(SQ_cities[index,0])
    path.set_ydata(SQ_cities[index,1])
    ax.set_title(fr"Generazione: {frame}  | Lunghezza miglior percorso $L$={SQ_paths["len"][frame]:.4f}")

    return path
    

ax.grid()
ax.set_xlabel("x")
ax.set_ylabel("y")



anim = animation.FuncAnimation(fig=fig, func=update, frames=200, interval=40) 
anim.save("square.gif")

# saves the animation in our desktop 
