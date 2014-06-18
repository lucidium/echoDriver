#pragma once

#include "ofMain.h"

class echoKalmanFiltering{
	public:
		echoKalmanFiltering(double q, double r, double f = 1, double h = 1);
		~echoKalmanFiltering();

		void correct(double data);

		void setState(double state, double covariance);
		double getState();

	private:
		double x0;
		double p0;

		double f;
		double q;
		double h;
		double r;

		double state;
		double covariance;
};

class echoSmoothValue {

	public:
		float result;

		void setup(unsigned int historySize)
		{
			this->historySize = historySize;
			it = 0;
		}

		float getSmoothValue(float p)
		{
			it++;
			if (it > historySize-1) it = 0;
			if (history_points.size() < historySize) history_points.push_back(p);
			else history_points[it] = p;

			for (unsigned int i = 0; i < history_points.size();i++)
				result += history_points[i];
				result /= history_points.size()+1;
				return result;
		}

	private:
		vector <float> history_points;
		unsigned int historySize;
		unsigned int it;
};