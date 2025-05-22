#ifndef WEB_UTILS_H
#define WEB_UTILS_H

#include <iostream>
#include <string>
#include <map> // Pro mapu

using namespace std;

/* Naparsuje data z webu */
map<string, string> parse_data(const string &data);
/* Dekóduje URL adresu */
string decode_url(const string &value);
/* Načte jednotlivá data do proměnných */
void get_form_data(double &var, double val, const string &what, map<string, string> form_data);
/* Hlavní funkce, která všechno zařídí */
void process_data(double &a, double &M, double &r_in, double &r_out, double &r0, double &th,
                  double &al_min, double &al_max, double &be_min, double &be_max,
                  double &al_step, double &be_step);

#endif