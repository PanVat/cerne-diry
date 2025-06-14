/*
    screen.cpp - using kerr_ray and kepler_disk modules it builds up the disk image
               - the screen is determined by alpha range (alpha_min,alpha_max) and 
                  beta range (beta_min,beta_max), and by resolution nalpha, nbeta
               - it stores the results in 2dim arrays F_screen (flux), g_screen (freq. shift)
                 and r_screen (radial coordinate of emission)

*/
#include "screen.h"

void screen_set_kerr(KERR *k, SCREEN *s)
{
    s->kerr = k;
}

void screen_set_disk(DISK *d, SCREEN *s)
{
    s->disk = d;
}

void screen_set_obs(double uo, double mo, SCREEN *s)
{
    s->uo = uo;
    s->mo = mo; 
}


void screen_set_alpha_range(double min, double max, int n, SCREEN *s)
{
    s->alphamin=min;
    s->alphamax=max;
    s->nalpha=n;
}

void screen_set_beta_range(double min, double max, int n, SCREEN *s)
{
    s->betamin=min;
    s->betamax=max;
    s->nbeta=n;
}

void screen_build(SCREEN *s)
{
    PHOTON ph;
    double alpha,beta,dalpha,dbeta;
    double l,q;
    double y0[4];
    int status;
    double wi,ri,phi,g,Fs,a,ds;
    double yi[4];

    a=s->kerr->a;
    kerr_ray_set_kerr(s->kerr,&ph);


    dalpha = (s->alphamax - s->alphamin)/s->nalpha;
    dbeta  = (s->betamax - s->betamin)/s->nbeta;

    for(int i=0;i<s->nalpha;i++)
    {
        fprintf(stderr,"Finished %.2lf %% \r",100.0*(i+1)/s->nalpha);
        alpha = s->alphamin + i*dalpha;
        for(int j=0;j<s->nbeta;j++)
        {
            beta = s->betamin + j*dbeta;
            
            kerr_ray_get_lq(alpha,beta,s->mo,&ph);
            
            kerr_ray_get_ic(y0,s->uo,s->mo,&ph);

            status = kerr_ray_integrate(y0,1.5/s->uo,&ph);
            //fprintf(stderr,"status=%d wmax=%lf n=%ld\n",status,ph.wtab[ph.wtab.size()-1],ph.wtab.size());
            if(status==0){
                //fprintf(stderr,"OK\n");
                double eps=1.0;
                int it=0;
                for(int k=0;k<ph.wtab.size()-1;k++)
                {
                    if(ph.ytab[1][k]*ph.ytab[1][k+1]<0)
                    {
                        kerr_ray_disk_inresect(k,k+1,&wi,yi,&ph);
                        ri = 1/yi[0];
                        //phi=yi[4];


                        if(ri>=s->disk->rin && ri<=s->disk->rout)
                        {
                            //cerr<<"Disk source"<<endl;
                            g = kepler_disk_get_g(ri,ph.l,s->disk);
                            Fs = kepler_disk_get_Fs(ri,s->disk); 
                            s->g_screen[i][j]=g;
                            s->F_screen[i][j]=s->F_screen[i][j] + eps*g*g*g*g*Fs;
                            s->r_screen[i][j]=ri;
                            //break;
                        }
                        it++;
                        eps=exp(-1.0*it);
                        break;
                    }
                }
            }
        }
    }
    fprintf(stderr,"\n");
}

void screen_alloc(SCREEN *s)
{
    s->F_screen = new double*[s->nalpha];
    s->g_screen = new double*[s->nalpha];
    s->r_screen = new double*[s->nalpha];

    for(int i=0;i<s->nalpha;i++)
    {
        s->F_screen[i] = new double[s->nbeta];
        s->g_screen[i] = new double[s->nbeta];
        s->r_screen[i] = new double[s->nbeta];
    }

    for(int i=0;i<s->nalpha;i++)
    {
        for(int j=0;j<s->nbeta;j++)
        {
            s->F_screen[i][j]=0;
            s->g_screen[i][j]=0;
            s->r_screen[i][j]=0;
        }
    }
}

void screen_read_params(FILE *f,SCREEN *p)
{
    char s[64];
    double a,uo,mo;
    double amin,amax,bmin,bmax;
    int na,nb;
    double rin,rout,dM;

    fscanf(f,"%s %lf",s,&a);
    fscanf(f,"%s %lf %lf",s,&uo,&mo);
    fscanf(f,"%s %lf %lf %lf",s,&rin,&rout,&dM);
    fscanf(f,"%s %lf %lf %d",s,&amin,&amax,&na);
    fscanf(f,"%s %lf %lf %d",s,&bmin,&bmax,&nb);
    
    kerr_set_a(a,p->kerr);
    kerr_set_M(1.0,p->kerr);
    kerr_get_horizon(p->kerr);

    screen_set_obs(1.0/uo,cos(mo*M_PI/180.0),p);
    if(rin<1e-5) rin=kepler_disk_get_isco(p->disk);
    kepler_disk_set_dM(dM,p->disk);
    kepler_disk_set_rin(rin,p->disk);
    kepler_disk_set_rout(rout,p->disk);

    screen_set_alpha_range(amin,amax,na,p);
    screen_set_beta_range(bmin,bmax,nb,p);
}


void screen_save(FILE *f,SCREEN *s)
{
    for(int i=0;i<s->nalpha;i++)
    {
        for(int j=0;j<s->nbeta;j++)
        {
            fprintf(f,"%d %d %.10lf %.10lf %.10lf\n",i,j,s->F_screen[i][j],s->g_screen[i][j],s->r_screen[i][j]);
        }
        fprintf(f,"\n");
    }
}
