"""
Author: Jan Schee
E-mail: jan.schee.physics.slu.cz
Descr: the script creates screen.png image out of screen.dat text table. 
The result is 2d bolometrix flux map, representing image of Kepler disk
orbiting Ker black hole. Note, only first order images are considered here 
since they are by far the brightest.
"""
import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl

screendata=np.loadtxt("C:/xampp/htdocs/run/screen.dat",dtype=float)

w=600
h=600
min=-30.0
max=30.0
d=(max-min)/w

z=screendata[:,2]

x=[0,100,200,300,400,500,600]
labels=np.linspace(-30.0,30.0,7)

data2d = np.reshape(z,(w,-1))
data2d=np.rot90(data2d)

fig,ax = plt.subplots()

fig.colorbar(mpl.cm.ScalarMappable(norm=mpl.colors.Normalize(0, 1), cmap='gist_heat'),
             ax=ax, orientation='vertical', label='bolometric flux')
im=ax.imshow(data2d,cmap='gist_heat')
ax.set_xticks(x,labels)
ax.set_yticks(x,labels)
ax.set_xlabel(r'$\alpha$')
ax.set_ylabel(r'$\beta$',rotation=0)

plt.savefig("C:/xampp/htdocs/img/screen.png")


