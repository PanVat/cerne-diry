#include "load_form.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    // CGI hlavička
    cout << "Content-type: text/html\n\n";

    double a, M, r_in, r_out, temp, r0, th;
    double al_min, al_max, be_min, be_max, al_step, be_step;

    // Načti data z formuláře
    process_data(
        a, M, r_in, r_out, temp, r0, th,
        al_min, al_max, be_min, be_max, al_step, be_step
    );

    // Zapiš do params.ini ve správném pořadí a zápisu
    ofstream params("params.ini");
    if (!params) {
        cout << "<h3>Chyba: Nelze otevřít params.ini pro zápis!</h3>";
        return 1;
    }
    // spin
    params << "spin " << a << "\n";
    // observer: vzdálenost, úhel (r0, th)
    params << "obs " << r0 << " " << th << "\n";
    // disk: r_in, r_out, temp
    params << "disk " << r_in << " " << r_out << " " << temp << "\n";
    // alpha_range: al_min, al_max, al_step
    params << "alpha_range " << al_min << " " << al_max << " " << al_step << "\n";
    // beta_range: be_min, be_max, be_step
    params << "beta_range " << be_min << " " << be_max << " " << be_step << "\n";
    params.close();

    cout << "<h2>Data byla uložena do params.ini</h2>";
    return 0;
}