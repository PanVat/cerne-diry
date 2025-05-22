#include <iostream>
#include <cmath>
#include <array>
#include <vector>

/* Pro načtení dat z webového formuláře */
#include "web_utils.h"

//TODO vyřešit knihovny
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_odeiv2.h>

using namespace std;

struct PHOTON
{
    double l, q;
    vector<double> wtab;
    vector<double> ytab[4];
};

struct PARAMS
{
    double uo, mo;
    double rin, rout;
    double alphamin, alphamax;
    double betamin, betamax;

    int nalpha, nbeta;

    double **F_screen;
    double **g_screen;
};

void ray_screen_alloc(PARAMS *p)
{
    p->F_screen = new double *[p->nalpha];
    p->g_screen = new double *[p->nalpha];

    for (int i = 0; i < p->nalpha; i++)
    {
        p->F_screen[i] = new double[p->nbeta];
        p->g_screen[i] = new double[p->nbeta];
    }

    for (int i = 0; i < p->nalpha; i++)
    {
        for (int j = 0; j < p->nbeta; j++)
        {
            p->F_screen[i][j] = 0;
            p->g_screen[i][j] = 0;
        }
    }
}

int ray_func(double w, const double y[], double f[], void *par)
{
    PHOTON *p = (PHOTON *)par;
    double l, l2, q;
    double U, dU, dM;
    double u, u2, m, m2, ku, km;

    l = p->l;
    l2 = l * l;
    q = p->q;

    u = y[0];
    u2 = u * u;
    m = y[1];
    m2 = m * m;
    ku = y[2];
    km = y[3];

    U = 1.0 - (1.0 - 2.0 * u) * (l2 + q) * u2;
    dU = -2.0 * u * (1.0 - 3.0 * u) * (l2 + q);
    dM = -2.0 * m * (l2 + q);

    f[0] = ku;
    f[1] = km;
    f[2] = 2.0 * u2 * u * (U + 0.25 * u * dU);
    f[3] = (2.0 / u) * ku * km + 0.5 * u2 * u2 * dM;

    return GSL_SUCCESS;
}

int ray_integrate(double wmax, double y0[], PHOTON *p)
{
    const int dim = 4;
    const gsl_odeiv2_step_type *T = gsl_odeiv2_step_rk8pd;
    int status = 0;
    gsl_odeiv2_step *s = gsl_odeiv2_step_alloc(T, dim);
    gsl_odeiv2_control *c = gsl_odeiv2_control_y_new(1e-14, 0.0);
    gsl_odeiv2_evolve *e = gsl_odeiv2_evolve_alloc(dim);

    gsl_odeiv2_system sys = {ray_func, NULL, dim, p};

    double w = 0.0, w1 = wmax;
    double h = 1e-6;
    double y[dim];

    for (int i = 0; i < dim; i++)
        y[i] = y0[i];

    p->wtab.clear();
    for (int i = 0; i < dim; i++)
        p->ytab[i].clear();

    while (w < w1)
    {
        int sts = gsl_odeiv2_evolve_apply(e, c, s,
                                          &sys,
                                          &w, w1,
                                          &h, y);

        if (sts != GSL_SUCCESS)
            break;

        p->wtab.push_back(w);
        for (int j = 0; j < dim; j++)
            p->ytab[j].push_back(y[j]);

        if (y[0] >= 0.5)
        {
            break;
        }
    }

    gsl_odeiv2_evolve_free(e);
    gsl_odeiv2_control_free(c);
    gsl_odeiv2_step_free(s);

    return status;
}

void ray_get_ic(double y0[], double msgn, double uo, double mo, PHOTON *p)
{
    double l, l2, q, uo2, mo2;

    l = p->l;
    l2 = l * l;
    q = p->q;
    uo2 = uo * uo;
    mo2 = mo * mo;

    y0[0] = uo;
    y0[1] = mo;
    y0[2] = uo2 * sqrt(fabs(1.0 - (1.0 - 2.0 * uo) * (l2 + q) * uo2));
    y0[3] = msgn * uo2 * sqrt(fabs(q - (l2 + q) * mo2));
}

void ray_save(FILE *f, PHOTON *p)
{
    int n = p->wtab.size();
    fprintf(f, "# %d\n", n);
    for (int i = 0; i < n; i++)
    {
        fprintf(f, "%.10lf %.10lf %.10lf %.10lf %.10lf\n",
                p->wtab[i], p->ytab[0][i], p->ytab[1][i], p->ytab[2][i], p->ytab[3][i]);
    }
}

void ray_screen_save(FILE *f, PARAMS *p)
{
    for (int i = 0; i < p->nalpha; i++)
    {
        for (int j = 0; j < p->nbeta; j++)
        {
            fprintf(f, "%d %d %.10lf %.10lf\n", i, j, p->F_screen[i][j], p->g_screen[i][j]);
        }
        fprintf(f, "\n");
    }
}

void ray_disk_inresect(int i1, int i2, double *wi, double yi[], PHOTON *p)
{
    int n = p->wtab.size();
    double w1, w2, m1, m2;

    w1 = p->wtab[i1];
    w2 = p->wtab[i2];
    m1 = p->ytab[1][i1];
    m2 = p->ytab[1][i2];

    *wi = (0.0 - m1) * (w2 - w1) / (m2 - m1) + w1;

    for (int i = 0; i < 4; i++)
    {
        yi[i] = (p->ytab[i][i2] - p->ytab[i][i1]) * (*wi - w1) / (w2 - w1) + p->ytab[i][i1];
    }
}

void ray_get_lq(double alpha, double beta, double mo, double *l, double *q)
{
    *l = alpha * sqrt(1.0 - mo * mo);
    *q = beta * beta + mo * mo * alpha * alpha;
}

double ray_get_g(double r, PHOTON *p)
{
    double l, Om, r2, f;

    l = p->l;
    r2 = r * r;
    Om = 1.0 / sqrt(r2 * r);

    f = 1.0 - 2.0 / r - r2 * Om * Om;

    return sqrt(f) / (1.0 - l * Om);
}

double ray_get_Fs(double r, double dM)
{
    double p;

    p = 3.0 * dM / (8 * M_PI);
    p = p * (sqrt(r) - sqrt(6) + (sqrt(3.0) / 3.0) * log((sqrt(r) + sqrt(3)) * (sqrt(6) - sqrt(3)) / ((sqrt(r) - sqrt(3)) * (sqrt(6) + sqrt(3)))));
    p = p / (r - 3);
    p = p / pow(r, 2.5);
    return p;
}

void ray_screen(PARAMS *p)
{
    PHOTON ph;
    double alpha, beta, dalpha, dbeta;
    double l, q;
    double y0[4];
    int status;
    double wi, ri, g, Fs;
    double yi[4];

    dalpha = (p->alphamax - p->alphamin) / p->nalpha;
    dbeta = (p->betamax - p->betamin) / p->nbeta;

    for (int i = 0; i < p->nalpha; i++)
    {
        fprintf(stderr, "Finished %.2lf %% \r", 100.0 * (i + 1) / p->nalpha);
        alpha = p->alphamin + i * dalpha;
        for (int j = 0; j < p->nbeta; j++)
        {
            beta = p->betamin + j * dbeta;

            ray_get_lq(alpha, beta, p->mo, &l, &q);

            ph.l = l;
            ph.q = q;

            ray_get_ic(y0, GSL_SIGN(beta), p->uo, p->mo, &ph);

            status = ray_integrate(2.0 / p->uo, y0, &ph);
            // fprintf(stderr,"status=%d wmax=%lf n=%ld\n",status,ph.wtab[ph.wtab.size()-1],ph.wtab.size());
            if (status == 0)
            {
                // fprintf(stderr,"OK\n");
                double eps = 1.0;
                int it = 0;
                for (int k = 0; k < ph.wtab.size() - 1; k++)
                {
                    if (ph.ytab[1][k] * ph.ytab[1][k + 1] < 0)
                    {
                        ray_disk_inresect(k, k + 1, &wi, yi, &ph);
                        ri = 1 / yi[0];

                        if (ri > p->rin && ri < p->rout)
                        {

                            g = ray_get_g(ri, &ph);
                            Fs = ray_get_Fs(ri, 2.0);
                            p->g_screen[i][j] = g;
                            p->F_screen[i][j] = p->F_screen[i][j] + eps * g * g * g * g * Fs;
                        }
                        it++;
                        eps = exp(-1.0 * it);
                    }
                }
            }
        }
    }
    fprintf(stderr, "\n");
}

int main(int argc, char *argv[])
{
    PARAMS par;

    double uo, mo;
    double y0[4];
    double wi;
    double yi[4];

    /* Data o černé díře */
    double a, M, r_in, r_out, r0, th;
    /* Data o vlastnostech výstupního obrázku */
    double al_min, al_max, be_min, be_max;
    /* Rozlišení výstupního obrázku */
    double al_step, be_step;

    /* Načtení dat z webového formuláře */
    process_data(a, M, r_in, r_out, r0, th,
                   al_min, al_max, be_min, be_max,
                   al_step, be_step);

    /* Vzdálenost pozorovatele */
    par.uo = r0;

    /* Konverze úhlu na radiány */
    par.mo = cos(th * M_PI / 180.0);

    /* Minimální alpha */
    par.alphamin = -30.0;
    par.alphamax = 30.0;
    par.betamin = -30.0;
    par.betamax = 30.0;

    par.nalpha = 400;
    par.nbeta = 400;

    par.rin = 6.0;
    par.rout = 100.0;

    ray_screen_alloc(&par);

    ray_screen(&par);

    FILE *f = fopen("screen.dat", "wt");
    ray_screen_save(f, &par);
    fclose(f);

    return 0;
}