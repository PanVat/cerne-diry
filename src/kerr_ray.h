#ifndef __KERR_RAY_H__
#define __KERR_RAY_H__

#include "num.h"
#include "kerr.h"

struct PHOTON{
    KERR *kerr;
    double l,q;
    double msgn;
    vector<double> wtab;
    vector<double> ytab[5];
};

extern void kerr_ray_set_kerr(KERR *k,PHOTON *p);
extern void kerr_ray_set_l(double l, PHOTON *p);
extern void kerr_ray_set_q(double q, PHOTON *p);
extern void kerr_ray_get_lq(double alpha, double beta, double mo, PHOTON *p);
extern void kerr_ray_get_ic(double y0[],double uo, double mo, PHOTON *p);
extern int kerr_ray_integrate(double y0[], double wmax, PHOTON *p);
extern void kerr_ray_disk_inresect(int i1, int i2,double *wi, double yi[], PHOTON *p);
extern void kerr_ray_save(FILE *f, PHOTON *p);




#endif