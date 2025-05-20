#include <iostream>
#include <cmath>

/* Funkce f(u) pro Schwarzschildovu metriku */
inline double f(double u) { return 1 - 2 * u; }

/* Funkce 'f' vrací součet čísel 'x' a 'y' */
inline double f(double x, double y) { return y; }

/* Funkce 'rk4integrate' provádí integraci pomocí Runge-Kutta 4. řádu */
double rk4step(double x, double y, double h)
{
    double k1 = h * f(x, y);
    double k2 = h * f(x + h / 2.0, y + k1 / 2.0);
    double k3 = h * f(x + h / 2.0, y + k2 / 2.0);
    double k4 = h * f(x + h, y + k3);
    y += (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
    return y;
}

void rk4integrate(double x1, double x2, double y0, int n)
{
    double h = (x2 - x1) / n;
    double y = y0;
    for (int i = 0; i < n; i++)
    {
        y = rk4step(x1 + i * h, y, h);
        std::cout << x1 + (i + 1) * h << " " << y << " " << exp(x1 + (i + 1) * h) << std::endl;
    }
}

void func(double x, const double *y, double *f)
{
    f[0] = y[1];
    f[1] = -y[0];
}

int main(int argc, char *argv[])
{
    double x1 = 0.0;
    double x2 = 10.0;
    double y[2] = {1.0, 0.0}; // y[0]=y, y[1]=y'
    int n = 1000;
    double h = (x2 - x1) / n;

    for (int step = 0; step < n; step++)
    {
        double x = x1 + step * h;
        double k1[2], k2[2], k3[2], k4[2], ytemp[2];

        func(x, y, k1);

        ytemp[0] = y[0] + 0.5 * h * k1[0];
        ytemp[1] = y[1] + 0.5 * h * k1[1];
        func(x + 0.5 * h, ytemp, k2);

        ytemp[0] = y[0] + 0.5 * h * k2[0];
        ytemp[1] = y[1] + 0.5 * h * k2[1];
        func(x + 0.5 * h, ytemp, k3);

        ytemp[0] = y[0] + h * k3[0];
        ytemp[1] = y[1] + h * k3[1];
        func(x + h, ytemp, k4);

        for (int i = 0; i < 2; i++)
        {
            y[i] += (h / 6.0) * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
        }
        printf("%.6lf  %.6lf  %.6lf  %.6lf\n",x1 + (step * h),  y[0], y[1], cos(x1 + (step * h)));
    }
    return 0;
}