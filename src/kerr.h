#ifndef __KERR_H__
#define __KERR_H__

#include "num.h"

struct KERR{
    double a,M,uh;
};

extern void kerr_set_a(double a,KERR *k);
extern void kerr_set_M(double M, KERR *k);
extern void kerr_get_horizon(KERR *k);

#endif