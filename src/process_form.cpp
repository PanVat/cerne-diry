#include "load_form.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>

using namespace std;

/* Nastaví proměnné prostředí HOME a USERPROFILE kvůli matplotlibu */
void startup(const char *lpApplicationName, char *lpCommandLine, const char *lpCurrentDirectory = NULL)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    SetEnvironmentVariableA("HOME", "C:\\xampp\\htdocs\\");
    SetEnvironmentVariableA("USERPROFILE", "C:\\xampp\\htdocs\\");

    /* Aby nevyskakovalo okno */
    DWORD dwCreationFlags = CREATE_NO_WINDOW;

    if (CreateProcessA(
            lpApplicationName,
            lpCommandLine, // editovatelný buffer!
            NULL, NULL, FALSE,
            dwCreationFlags, // zde je důležité
            NULL, lpCurrentDirectory, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        std::cerr << "Failed to start process." << std::endl;
    }
}

int main()
{
    /* CGI hlavička */
    cout << "Content-type: text/html\n\n";

    double a, M, r_in, r_out, temp, r0, th;
    double al_min, al_max, be_min, be_max, al_step, be_step;

    /* Načte data z formuláře */
    process_data(
        a, M, r_in, r_out, temp, r0, th,
        al_min, al_max, be_min, be_max, al_step, be_step);

    /* Zapiš do params.ini ve správném pořadí a zápisu */
    string prefix = "C:/xampp/htdocs/";
    ofstream params(prefix + "run/params.ini");
    if (!params)
    {
        cout << "<h3>Chyba: Nelze otevřít params.ini pro zápis!</h3>";
        return 1;
    }
    params << "spin " << a << "\n";
    params << "obs " << r0 << " " << th << "\n";
    params << "disk " << r_in << " " << r_out << " " << temp << "\n";
    params << "alpha_range " << al_min << " " << al_max << " " << al_step << "\n";
    params << "beta_range " << be_min << " " << be_max << " " << be_step << "\n";
    params.close();

    /* Sestavení argumentů pro kray.exe */
    std::string exe = prefix + "run/kray.exe";
    std::string args =
        "\"" + exe + "\" \"" + prefix + "run/params.ini\" \"" +
        prefix + "run/screen.dat\" \"" +
        prefix + "run/pline.ini\" \"" +
        prefix + "run/pline.dat\"";
    std::vector<char> cmdline(args.begin(), args.end());
    cmdline.push_back('\0');

    /* Spuštění scriptu kray.exe */
    startup(exe.c_str(), cmdline.data(), (prefix + "run").c_str());

    std::string venv_python = prefix + ".venv/Scripts/python.exe";
    std::string plot_script = prefix + "plot/plot.py";
    std::string screen_script = prefix + "plot/screen.py";

    /* Spustí plot.py a vytvoří graf */
    std::string plot_cmd = "\"" + venv_python + "\" \"" + plot_script + "\"";
    std::vector<char> plotline(plot_cmd.begin(), plot_cmd.end());
    plotline.push_back('\0');
    startup(venv_python.c_str(), plotline.data(), (prefix + "plot").c_str());

    /* Spustí screen.py a vytvoří obrázek */
    std::string screen_cmd = "\"" + venv_python + "\" \"" + screen_script + "\"";
    std::vector<char> screenline(screen_cmd.begin(), screen_cmd.end());
    screenline.push_back('\0');
    startup(venv_python.c_str(), screenline.data(), (prefix + "plot").c_str());

    return 0;
}