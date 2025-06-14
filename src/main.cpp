/* 
    kray 
    Author: Jan Schee
    E-mail: jan.schee@physics.slu.cz

    Description: a relativistic backward raytracing code to build the image of Kepler disk orbiting
                 a Kerr black hole; along with the disk image the corresponding profiled line is computed
                 assuming the local line emission to be spherically symmetric and the produced line has 
                 Gaussian bell profile, i.e. I_nue = e(r) exp (-(1/2sigma^2)(nuo/g - nu0)^2)

                 There two parameter files the code uses for specific smulations:
                    - params.ini - the screen, disk and observer specifications
                    - pline.ini - specifications of the spectral line emission model

    Usage: kray params.dat screen_out.dat pline.dat pline_out.dat
*/

#include "kerr.h"
#include "kerr_ray.h"
#include "kepler_disk.h"
#include "screen.h"
#include "pline.h"

#define MSG(s,f) {fprintf(stderr,"MSG:%s\n",s);f;fprintf(stderr,"done\n");}

int main(int argc, char *argv[])
{
    KERR kerr;
    DISK disk;
    SCREEN screen;
    PLINE pl;

    kepler_disk_set_kerr(&kerr,&disk);
    screen_set_kerr(&kerr,&screen);
    screen_set_disk(&disk,&screen);

    //building disk image
    FILE *f = fopen(argv[1],"rt");
    screen_read_params(f,&screen);
    fclose(f);

    screen_alloc(&screen);
    
    MSG("building screen",screen_build(&screen))

    f = fopen(argv[2],"wt");
    screen_save(f,&screen);
    fclose(f);


    //building corresponding profiled line
    f=fopen(argv[3],"rt");
    pline_read_params(f,&pl);
    fclose(f);
    pline_arrays_init(&pl);
    
    MSG("building pline",pline_build(&screen,&pl))

    f = fopen(argv[4],"wt");
    pline_save(f,&pl);
    fclose(f);


    return 0;
}