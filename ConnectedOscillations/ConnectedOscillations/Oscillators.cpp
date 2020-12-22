#include "Oscillators.h"

Oscillators::Oscillators()
{
	_fi10 = 0;
	_fi20 = 0;
	_amp1 = 0;
	_amp2 = 0;
}

void Oscillators::setParameters(Point leftUp, Point rightUp, double length, double springHeight, double mass)
{
	_leftUp = leftUp;
	_rightUp = rightUp;
	_length = length;
	_spingHeight = springHeight;
	_spingHeight = springHeight;
	_mass = mass;
}

Point Oscillators::getLeft()
{
	Point result, vector;
	vector.x = cos(_fi2 - M_PI / 2) * _length;
	vector.y = sin(_fi2 - M_PI / 2) * _length;
	result.x = _leftUp.x - vector.x;
	result.y = _leftUp.y - vector.y;
	return result;
}

void Oscillators::incrementTime(double time)
{
	_time += time;
	double frequency1 = sqrt(_length / g), frequency2 = sqrt(pow(frequency1, 2) + 2 *( _springiness * pow(_spingHeight, 2))/(_mass*pow(_length, 2)));
	_fi1 = (0.5 * (_amp1 * cos(frequency1 * _time + _fi10) + _amp2 * cos(frequency2 * _time + _fi20)));
	_fi2 = (0.5 * (_amp1 * cos(frequency1 * _time + _fi10) - _amp2 * cos(frequency2 * _time + _fi20)));
	_amp1 *= 0.99;
	_amp2 *= 0.99;
}

void Oscillators::restart(double amp1, double amp2, double springiness)
{
	_amp1 = (amp1 + amp2)/2;
	_amp2 = (amp1 - amp2)/2;
	_springiness = springiness;
	_time = 0;
}

Point Oscillators::getRight()
{
	Point result, vector;
	vector.x = cos(_fi1 - M_PI / 2) * _length;
	vector.y = sin(_fi1 - M_PI / 2) * _length;
	result.x = _rightUp.x - vector.x;
	result.y = _rightUp.y - vector.y;
	return result;
}

Point Oscillators::getSpringLeft()
{
	Point result, vector;
	vector.x = cos(_fi2 - M_PI / 2) * _spingHeight;
	vector.y = sin(_fi2 - M_PI / 2) * _spingHeight;
	result.x = _leftUp.x - vector.x;
	result.y = _leftUp.y - vector.y;
	return result;
}

Point Oscillators::getSpringRight()
{
	Point result, vector;
	vector.x = cos(_fi1 - M_PI / 2) * _spingHeight;
	vector.y = sin(_fi1 - M_PI / 2) * _spingHeight;
	result.x = _rightUp.x - vector.x;
	result.y = _rightUp.y - vector.y;
	return result;
}
