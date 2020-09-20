#include "Generator.h"
#define _USE_MATH_DEFINES
#include <cmath>

Generator::Generator(): _time(0), _base_freq(10000), _b_phase(0), _a(1), _b(1)
{
}

double Generator::get_x()
{
    return sin((_time/(M_PI*2)) * _base_freq * _a);
}

double Generator::get_y()
{
    return sin((_time / (M_PI * 2)) * _base_freq * _b + ((_b_phase)/360)*2*M_PI);
}

void Generator::incTime(const double& timePassed)
{
    _time += timePassed;
}

void Generator::setPara(double b_phase, double a, double b)
{
    _b_phase = b_phase;
    _a = a;
    _b = b;
}
