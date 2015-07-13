/*
 * HoughTransormat.h
 *
 *  Created on: 12 Jul 2015
 *      Author: szsz
 */

#ifndef HOUGHTRANSORMAT_H_
#define HOUGHTRANSORMAT_H_

#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>

struct Collinear
{
double Alfa;
double C;
unsigned int Count;

};

class HoughTransormat {
public:
	HoughTransormat();
	virtual ~HoughTransormat();

	float GetAngle(double* angles,int* distnaces,unsigned int length);
	double getC(int r,double fi);
	bool isCollinear(double c0,double c1);

	double Epsilon;
};

#endif /* HOUGHTRANSORMAT_H_ */
