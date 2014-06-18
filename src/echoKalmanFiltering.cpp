#include "echoKalmanFiltering.h"

echoKalmanFiltering::echoKalmanFiltering(double q, double r, double f, double h)
{
	this->q = q;
	this->r = r;
	this->f = f;
	this->h = h;
}

echoKalmanFiltering::~echoKalmanFiltering()
{

}

void echoKalmanFiltering::correct(double data)
{
	//Прогноз 
	x0 = f * state;
	p0 = f * covariance * f + q;
	
	//Корректировка
	double k = h * p0 / (h * p0 * h + r);
	state = x0 + k * (data - h * x0);
	covariance = (1 - k * h) * p0;
}

void echoKalmanFiltering::setState(double state, double covariance)
{
	this->state = state;
	this->covariance = covariance;
}

double echoKalmanFiltering::getState()
{
	return state;
}