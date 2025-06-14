#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "num.h"
#include "kerr.h"
#include "kerr_ray.h"
#include "kepler_disk.h"

struct SCREEN
{
    KERR *kerr;
    DISK *disk;
    double uo,mo;
    double alphamin,alphamax;
    double betamin,betamax;

    int nalpha,nbeta;

    double **F_screen;
    double **g_screen;
    double **r_screen;

};

extern void screen_set_kerr(KERR *k, SCREEN *s);
extern void screen_set_disk(DISK *d, SCREEN *s);
extern void screen_set_obs(double uo, double mo, SCREEN *s);
extern void screen_set_alpha_range(double min, double max, int n, SCREEN *s);
extern void screen_set_beta_range(double min, double max, int n, SCREEN *s);
extern void screen_build(SCREEN *s);
extern void screen_alloc(SCREEN *s);
extern void screen_read_params(FILE *f,SCREEN *p);
extern void screen_save(FILE *f,SCREEN *s);


#endif