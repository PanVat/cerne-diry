 /*
    Author: Jan Schee
    E-mail: jan.schee@physics.slu.cz

    num.cpp - implements some numerical utilities
*/
#include "num.h"

double  num_drand(double min, double max)
{
    return min + (double)(max-min)*rand()/RAND_MAX;
}