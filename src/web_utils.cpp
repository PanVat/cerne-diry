#include "web_utils.h"

map<string, string> parse_data(const string &data)
{
    map<string, string> parsedData;
    size_t start = 0;
    while (start < data.size())
    {
        size_t equals = data.find('=', start);
        size_t ampersand = data.find('&', start);
        if (equals == string::npos)
            break;

        string key = data.substr(start, equals - start);
        string value = data.substr(equals + 1, (ampersand == string::npos ? data.size() : ampersand) - equals - 1);
        parsedData[decode_url(key)] = decode_url(value);

        start = (ampersand == string::npos) ? data.size() : ampersand + 1;
    }
    return parsedData;
}

string decode_url(const string &value)
{
    string result;
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

void get_form_data(double &var, double val, const string &what, map<string, string> form_data)
{
    try
    {
        var = form_data.count(what) ? stod(form_data[what]) : val;
    }
    catch (...)
    {
        var = val;
    }
}

void process_data(double &a, double &M, double &r_in, double &r_out, double &r0, double &th,
                  double &al_min, double &al_max, double &be_min, double &be_max,
                  double &al_step, double &be_step)
{

    map<string, string> form_data;
    char *method = getenv("REQUEST_METHOD");

    if (method && string(method) == "GET")
    {
        char *query = getenv("QUERY_STRING");
        if (query)
        {
            form_data = parse_data(query);
        }
    }
    else if (method && string(method) == "POST")
    {
        char *length = getenv("CONTENT_LENGTH");
        if (length)
        {
            int len = atoi(length);
            string post_data;
            post_data.resize(len);
            cin.read(&post_data[0], len);
            form_data = parse_data(post_data);
        }
    }
    /* Načtení všech dat z formuláře do příslušných proměnných */

    /* Spin */
    get_form_data(a, 0.0, "a", form_data);
    /* Hmotnost */
    get_form_data(M, 0.0, "M", form_data);
    /* Vnitřní okraj */
    get_form_data(r_in, 0.0, "r_in", form_data);
    /* Vnější okraj */
    get_form_data(r_out, 0.0, "r_out", form_data);
    /* Počáteční vzdálenost */
    get_form_data(r0, 0.0, "r0", form_data);
    /* Počáteční úhel */
    get_form_data(th, 0.0, "th", form_data);
    /* Alfa min */
    get_form_data(al_min, 0.0, "al_min", form_data);
    /* Alfa max */
    get_form_data(al_max, 0.0, "al_max", form_data);
    /* Beta min */
    get_form_data(be_min, 0.0, "be_min", form_data);
    /* Beta max */
    get_form_data(be_max, 0.0, "be_max", form_data);
    /* Alfa krok */
    get_form_data(al_step, 0.0, "al_step", form_data);
    /* Beta krok */
    get_form_data(be_step, 0.0, "be_step", form_data);
}