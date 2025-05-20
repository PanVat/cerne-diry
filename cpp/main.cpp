#define _USE_MATH_DEFINES // Potřebné kvůli matematickým konstantám

#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <cmath>
#include <algorithm>

/* Pohybové konstanty (můžeme si nastavit dle potřeby) */
static const double l = 2.0;
static const double q = 1.0;




std::map<std::string, std::string> parse_data(const std::string &data);
std::string decode_url(const std::string &value);

int main()
{
    /* Deklarace všech proměnných načtených od uživatele */
    double spin;        // Spin 'a'
    double mass;        // Hmotnost 'M'
    double radius_in;   // Vnitřní okraj 'r_in'
    double radius_out;  // Vnější okraj 'r_out'
    double temperature; // Teplota 'T'
    double distance;    // Vzdálenost 'r0'
    double angle;       // Pozorovací úhel 'theta'

    std::map<std::string, std::string> formData;
    char *method = std::getenv("REQUEST_METHOD");

    if (method && std::string(method) == "GET")
    {
        char *query = std::getenv("QUERY_STRING");
        if (query)
        {
            formData = parse_data(query);
        }
    }
    else if (method && std::string(method) == "POST")
    {
        char *contentLength = std::getenv("CONTENT_LENGTH");
        if (contentLength)
        {
            int length = std::atoi(contentLength);
            std::string postData;
            postData.resize(length);
            std::cin.read(&postData[0], length);
            formData = parse_data(postData);
        }
    }

    /* Načte všechna data z formuláře do proměnných */
    try
    {
        spin = formData.count("a") ? std::stod(formData["a"]) : 0.0;
    }
    catch (...)
    {
        spin = 0.0;
    }
    try
    {
        mass = formData.count("m") ? std::stod(formData["m"]) : 0.0;
    }
    catch (...)
    {
        mass = 0.0;
    }
    try
    {
        radius_in = formData.count("rin") ? std::stod(formData["rin"]) : 0.0;
    }
    catch (...)
    {
        radius_in = 0.0;
    }
    try
    {
        radius_out = formData.count("rout") ? std::stod(formData["rout"]) : 0.0;
    }
    catch (...)
    {
        radius_out = 0.0;
    }
    try
    {
        temperature = formData.count("T") ? std::stod(formData["T"]) : 0.0;
    }
    catch (...)
    {
        temperature = 0.0;
    }
    try
    {
        distance = formData.count("r0") ? std::stod(formData["r0"]) : 0.0;
    }
    catch (...)
    {
        distance = 0.0;
    }
    try
    {
        angle = formData.count("theta") ? std::stod(formData["theta"]) : 0.0;
    }
    catch (...)
    {
        angle = 0.0;
    }

    /* Souřadnice, které budou sloužit k výpočtu -> (t, u, m, ϕ) */
    double u = 1 / distance;              // Vzdálenost
    double m = cos(angle * (M_PI / 180)); // Úhel


    return 0;
}

/* Funkce pro dekódování URL formuláře */
std::string decode_url(const std::string &value)
{
    std::string result;
    for (size_t i = 0; i < value.length(); i++)
    {
        if (value[i] == '%')
        {
            int hex;
            sscanf(value.substr(i + 1, 2).c_str(), "%x", &hex);
            result += static_cast<char>(hex);
            i += 2;
        }
        else if (value[i] == '+')
        {
            result += ' ';
        }
        else
        {
            result += value[i];
        }
    }
    return result;
}

/* Funkce pro parsování dat z formuláře */
std::map<std::string, std::string> parse_data(const std::string &data)
{
    std::map<std::string, std::string> parsedData;
    size_t start = 0;
    while (start < data.size())
    {
        size_t equals = data.find('=', start);
        size_t ampersand = data.find('&', start);
        if (equals == std::string::npos)
            break;

        std::string key = data.substr(start, equals - start);
        std::string value = data.substr(equals + 1, (ampersand == std::string::npos ? data.size() : ampersand) - equals - 1);
        parsedData[decode_url(key)] = decode_url(value);

        start = (ampersand == std::string::npos) ? data.size() : ampersand + 1;
    }
    return parsedData;
}