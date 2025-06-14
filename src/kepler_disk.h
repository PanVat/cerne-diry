#ifndef __KEPLER_DISK_H__
#define __KEPLER_DISK_H__

#include "num.h"
#include "kerr.h"

struct DISK{
    KERR *kerr;
    double rin,rout;
    double dM;
};

extern void kepler_disk_set_kerr(KERR *k,DISK *d);
extern void kepler_disk_set_rin(double rin, DISK *d);
extern void kepler_disk_set_rout(double rout, DISK *d);
extern void kepler_disk_set_dM(double dM, DISK *d);
extern double kepler_disk_get_isco(DISK *d);
extern double kepler_disk_get_g(double r,double l, DISK *d);
extern double kepler_disk_get_Fs(double r, DISK *d);

#endif