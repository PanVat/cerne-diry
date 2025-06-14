/*
    Author: Jan Schee
    E-mail: jan.schee@physics.slu.cz
    kepler_disk_cpp - implements the structure of  Kepler disk
                    - it spans between rin and rout, where is rin>=risco and rout>rin
                    - it computes location of risco if required (when user sets rin=0)
                    - it implements local bolometric radiation flux, Fs,  emission from Novikov-Thorne model
                    - it computes frequency shift, g, of particular photon
*/

#include "kepler_disk.h"

void kepler_disk_set_kerr(KERR *k,DISK *d)
{
    d->kerr = k;
}

void kepler_disk_set_rin(double rin, DISK *d)
{
    d->rin = rin;
}

void kepler_disk_set_rout(double rout, DISK *d)
{
    d->rout = rout;
}

void kepler_disk_set_dM(double dM, DISK *d)
{
    d->dM = dM;
}

double kepler_disk_get_isco(DISK *d)
{
    double Z1,Z2;
    double a = d->kerr->a;

    Z1=1 + pow(1-a*a,1.0/3.0)*(pow(1+a,1.0/3.0) + pow(1.0-a,1.0/3.0));
    Z2=sqrt(3.0*a*a+Z1*Z1);

    return 3 + Z2 - sqrt((3-Z1)*(3+Z1+2*Z2));
}

double kepler_disk_get_g(double r,double l, DISK *d)
{
    double Om,r2,f;
    double A,a,a2;
    a=d->kerr->a;
    a2=a*a;

    r2=r*r;
    
    Om=1.0/(a+sqrt(r2*r));
    A=(r2+a2)*(r2+a2) - a2*(r2-2*r+a2);
    f = 1.0 - (2.0/r)*(1 - a*Om)*(1 - a*Om)  - (r2+a2)*Om*Om;

    return sqrt(fabs(f))/(1.0 - l*Om);
}

double kepler_disk_get_Fs(double r, DISK *d)
{
    double p,x,x0,x1,x2,x3;
    double a=d->kerr->a;
    
    x=sqrt(r);
    x0=sqrt(kepler_disk_get_isco(d));

    x1=2*cos((acos(a)-M_PI)/3);
    x2=2*cos((acos(a)+M_PI)/3);
    x3=-2*cos((acos(a))/3);
    

    p=1.5/M_PI;

    p=p*(x - x0 - 1.5*a*log(x/x0) 
           - 3*(x1-a)*(x1-a)/(x1*(x1-x2)*(x1-x3))*log((x-x1)/(x0-x1))
           - 3*(x2-a)*(x2-a)/(x2*(x2-x1)*(x2-x3))*log((x-x2)/(x0-x2)) 
           - 3*(x3-a)*(x3-a)/(x3*(x3-x1)*(x3-x2))*log((x-x3)/(x0-x3)));
    
    
    return (d->dM/(4*M_PI*r))*p;
}


