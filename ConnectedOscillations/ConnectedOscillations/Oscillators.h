#pragma once
#define g 9.812
#define _USE_MATH_DEFINES
#include <cmath>
struct Point
{
	double x;
	double y;
};
class Oscillators
{
public:
	Oscillators();
	void setParameters(Point leftUp,
	Point rightUp, double length,
	double springHeight, double mass);
	Point getLeft();
	Point getRight();
	Point getSpringLeft();
	Point getSpringRight();
	void incrementTime(double time);
	void restart(double amp1, double amp2, double spinginess);
public:
	double _time;
	Point _leftUp;
	Point _rightUp;
	double _length;
	double _spingHeight;
	double _springiness;
	double _mass;

	double _fi1;
	double _fi2;
	double _fi10;
	double _fi20;
	double _amp1;
	double _amp2;
	
};

