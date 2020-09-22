#include "Generator.h"
#define _USE_MATH_DEFINES
#include <cmath>

Generator::Generator(): _time(0), _base_freq(500), _b_phase(0), _a(1), _b(1)
{
}

double Generator::get_x()
{
    return sin((_time * M_PI * 2) * _base_freq * _a);
}

double Generator::get_y()
{
    return sin((_time * M_PI * 2) * _base_freq * _b + ((_b_phase) / 360) * 2 * M_PI);
}

void Generator::incTime(const double& timePassed)
{
    _time += timePassed;
    if (_time > 1000)
    {
        _time -= 1000;
    }
}

void Generator::setPara(double b_phase, double a, double b)
{
    _b_phase = b_phase;
    if (a > b)
    {
        _a = 1;
        _b = b / a;
    }
    else
    {
        _b = 1;
        _a = a / b;
    }

}
