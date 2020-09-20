#pragma once
class Generator
{
public:
    Generator();
    double get_x();
    double get_y();
    void incTime(const double& timePassed);
    void setPara(double b_phase, double a, double b);
private:
    double _time;
    double _base_freq;
    double _b_phase;
    double _a;
    double _b;
};

