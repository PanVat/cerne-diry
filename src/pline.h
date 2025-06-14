#ifndef __PLINE_H__
#define __PLINE_H__

#include "num.h"
#include "screen.h"

struct PLINE
{
    double nuo_min, nuo_max;
    int n;
    double nu0,sigma;

    double *F_tab;
    double *nuo_tab;
    double p;
};

extern void pline_set_nu0(double nu0,PLINE *pl);
extern void pline_set_sigma(double sigma,PLINE *pl);
extern void pline_set_nuo_range(double min, double max, int n, PLINE *pl);
extern void pline_set_p(double p,PLINE *pl);
extern void pline_arrays_init(PLINE *pl);
extern void pline_build(SCREEN *s, PLINE *pl);
extern void pline_save(FILE *f,PLINE *pl);
extern void pline_read_params(FILE *f,PLINE *pl);

#endif