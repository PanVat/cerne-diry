"""
Author: Jan Schee
E-mail: jan.schee.physics.slu.cz
Descr: the script creates plplot.png image out of pline.dat text table. 
The result is the plot of specific radiation flux vs frequency of observation, representing spectral line emitted
from Kepler disk orbiting Ker black hole. Note, only first order images are considered here 
since they are by far the brightest.
"""
import matplotlib.pyplot as plt
import numpy as np



pldata=np.loadtxt("../run/pline.dat",dtype=float)

nu0=6.4

x,y=pldata[:,0],pldata[:,1]

Fmax=y.max()
print(Fmax)

y=y/Fmax
x=nu0*x

fig,ax = plt.subplots()

ax.plot(x,y)
ax.set_xlabel("observed frequency[keV]")
ax.set_ylabel("specific flux[unitless]")
plt.savefig("plplot.png")