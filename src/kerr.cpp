/*
    Author: Jan Schee
    E-mail: jan.schee@physics.slu.cz
    kerr.cpp - implements Kerr spacetime parameters specifications
             -  computes location of the event horizon
*/
#include "kerr.h"

void kerr_set_a(double a,KERR *k)
{
    k->a=a;
}

void kerr_set_M(double M, KERR *k)
{
    k->M=M;
}

void kerr_get_horizon(KERR *k)
{
    double M,a;
    M=k->M;
    a=k->a;
    k->uh = M + sqrt(M*M - a*a);
}

