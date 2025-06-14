/*
    Author: Jan Schee
    E-mail: jan.schee@physics.slu.cz
    pline.cpp - it uses computed results from screen modele stored in F_sreen, g_screen and r_screen
              - it calculates the resulting profiled line, i.e. plot of specific flux vs observed frequency
*/

#include "pline.h"

void pline_set_nu0(double nu0,PLINE *pl)
{
    pl->nu0 = nu0;
}

void pline_set_sigma(double sigma,PLINE *pl)
{
    pl->sigma = sigma;
}

void pline_set_nuo_range(double min, double max, int n, PLINE *pl)
{
    pl->nuo_min = min;
    pl->nuo_max = max;
    pl->n = n;
}

void pline_set_p(double p,PLINE *pl)
{
    pl->p = p;
}

void pline_arrays_init(PLINE *pl)
{
    pl->F_tab = new double[pl->n];
    pl->nuo_tab = new double[pl->n];

    for(int i=0;i<pl->n;i++){
        pl->F_tab[i] = pl->nuo_tab[i]=0.0;
    }
}

void pline_build(SCREEN *s, PLINE *pl)
{
    double dnuo = (pl->nuo_max - pl->nuo_min)/pl->n;
    double nuo,g,r,nu0,sigma;

    nu0 = pl->nu0;
    sigma = pl->sigma;

    for(int k=0;k<pl->n;k++)
    {
        nuo = pl->nuo_min + k*dnuo;
        pl->nuo_tab[k]=nuo;

        for(int i=0;i<s->nalpha;i++)
        {
            for(int j=0;j<s->nbeta;j++)
            {
                g = s->g_screen[i][j];
                r = s->r_screen[i][j];

                if(fabs(g)>1e-4 && fabs(r)>1.0){
                    pl->F_tab[k] += g*g*g*pow(r,-pl->p)*exp(-(nuo/g-nu0)*(nuo/g-nu0)/(2*sigma*sigma)); 
                }
            }
        }
    }
}

void pline_save(FILE *f,PLINE *pl)
{
    for(int k=0;k<pl->n;k++)
    {
        fprintf(f,"%lf %lf\n",pl->nuo_tab[k],pl->F_tab[k]);
    }
}

void pline_read_params(FILE *f,PLINE *pl)
{
    char s[64];
    double nuomin,nuomax;
    int n;
    double nu0;
    double p,sigma;
    
    fscanf(f,"%s %lf %lf %d",s,&nuomin,&nuomax,&n);
    fscanf(f,"%s %lf %lf %lf",s,&nu0,&p,&sigma);

    pline_set_nuo_range(nuomin,nuomax,n,pl);
    pline_set_nu0(nu0,pl);
    pline_set_p(p,pl);
    pline_set_sigma(sigma,pl);
}