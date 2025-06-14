/*
    Author: Jan Schee
    E-mail: jan.schee@physics.slu.cz
    kerr_ray.cpp - responsible for null geodesic integration
                 - photons move along null geodesics
                 - each geodesics is uniquely identified with impactparameters l,q
                 - l,q are associated with coordinated of intersection between null 
                    geodesics and the image plane alpha,beta
*/

#include "kerr_ray.h"

void kerr_ray_set_kerr(KERR *k, PHOTON *p)
{
    p->kerr = k;
}

void kerr_ray_set_l(double l, PHOTON *p)
{
    p->l = l;
}

void kerr_ray_set_q(double q, PHOTON *p)
{
    p->q = q;
}

void kerr_ray_get_lq(double alpha, double beta, double mo, PHOTON *p)
{
    double a = p->kerr->a;
    p->l = alpha*sqrt(1.0 - mo*mo);
    p->q = beta*beta + mo*mo*(alpha*alpha - a*a);
    p->msgn = GSL_SIGN(beta);
}

void kerr_ray_get_ic(double y0[],double uo, double mo, PHOTON *p)
{
    double l,l2,q,uo2,mo2,a,a2;
    double U,M;
    
    l=p->l;
    l2=l*l;
    q=p->q;
    a=p->kerr->a;
    a2=a*a;

    uo2=uo*uo;
    mo2=mo*mo;

    U=1.0 + uo2*(a2-l2-q + uo*(2.0*((a-l)*(a-l)+q) - a2*q*uo));
    M=q + mo2*(a2-l2-q - a2*mo2);

    y0[0]=uo;
    y0[1]=mo;
    y0[2]=uo2*sqrt(fabs(U))/(1+a2*mo2*uo2);
    y0[3]=p->msgn*uo2*sqrt(fabs(M))/(1+a2*mo2*uo2);
    //y0[4]=0.0;
}

int  kerr_ray_func(double w, const double y[], double f[], void *par)
{
    PHOTON *p=(PHOTON *)par;
    double l,l2,q,a,a2;
    double dU,dM,Sigma,dSigma;
    double u,u2,m,m2,ku,km;

    l=p->l;
    l2=l*l;
    q=p->q;
    a=p->kerr->a;
    a2=a*a;

    u=y[0];u2=u*u;
    m=y[1];m2=m*m;
    ku=y[2];
    km=y[3];

    Sigma=1.0/u2 + a2*m2;
    dSigma=-2.0*ku/(u2*u) + 2.0*a2*m*km;
    dU=2.0*u*(a2-l2-q + u*(3.0*((a-l)*(a-l)+q) - 2*a2*q*u));
    dM=2.0*m*(a2-l2-q - 2*a2*m2);

    f[0]=ku;
    f[1]=km;
    f[2]=0.5*(dU - 2.0*dSigma*Sigma*ku)/(Sigma*Sigma);
    f[3]=0.5*(dM - 2.0*dSigma*Sigma*km)/(Sigma*Sigma);
   // f[4]=u2*(-a + l/(1-m2) + a*(1+a*(a-l)*u2)/(1-2*u+a2*u2))/(1+a2*u2*m2);

    return GSL_SUCCESS;
}

int kerr_ray_integrate(double y0[], double wmax, PHOTON *p)
{
    const int dim=4;
    const gsl_odeiv2_step_type * T = gsl_odeiv2_step_rk8pd;
    int status=0;
    gsl_odeiv2_step * s = gsl_odeiv2_step_alloc (T, dim);
    gsl_odeiv2_control * c = gsl_odeiv2_control_y_new (1e-14, 0.0);
    gsl_odeiv2_evolve * e = gsl_odeiv2_evolve_alloc (dim);

    gsl_odeiv2_system sys = {kerr_ray_func, NULL, dim, p};

    double w = 0.0, w1 = wmax;
    double h = 1e-6;
    double y[dim];

    for(int i=0;i<dim;i++) y[i]=y0[i];

    p->wtab.clear();
    for(int i=0;i<dim;i++) p->ytab[i].clear();

    while (w < w1)
    {
      int sts = gsl_odeiv2_evolve_apply (e, c, s,
                                           &sys,
                                           &w, w1,
                                           &h, y);

      if (sts != GSL_SUCCESS)
          break;

      p->wtab.push_back(w);
      for(int j=0;j<dim;j++) p->ytab[j].push_back(y[j]);

      if(y[0]>0.9*p->kerr->uh){
        break;
      }
    }

    gsl_odeiv2_evolve_free (e);
    gsl_odeiv2_control_free (c);
    gsl_odeiv2_step_free (s);

    return status;
}

void kerr_ray_disk_inresect(int i1, int i2,double *wi, double yi[], PHOTON *p)
{
    int n=p->wtab.size();
    double w1,w2,m1,m2;

    w1=p->wtab[i1];
    w2=p->wtab[i2];
    m1=p->ytab[1][i1];
    m2=p->ytab[1][i2];

    *wi = (0.0-m1)*(w2-w1)/(m2-m1) + w1;

    for(int i=0;i<4;i++)
    {
        yi[i]=(p->ytab[i][i2] - p->ytab[i][i1])*(*wi - w1)/(w2-w1) + p->ytab[i][i1];
    }
}

void kerr_ray_save(FILE *f, PHOTON *p)
{
    int n = p->wtab.size();
    fprintf(f,"# %d\n",n);
    for(int i=0;i<n;i++)
    {
        fprintf(f,"%.10lf %.10lf %.10lf %.10lf %.10lf\n",
            p->wtab[i],p->ytab[0][i],p->ytab[1][i],p->ytab[2][i],p->ytab[3][i]
        );
    }
}


