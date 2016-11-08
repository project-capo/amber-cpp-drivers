/*
 * Particle2.h
 *
 *  Created on: Nov 7, 2016
 *      Author: szsz
 */

#ifndef PARTICLE2_H_
#define PARTICLE2_H_

#include "Common.h"

class Particle2 {

public:

#if DIAGNOSTIC_MODE == 1
	unsigned int Length;
	double* CalculatedProbability;
	double* CalculatedDistances;
#endif

public:
	double X;
	double Y;
	double P;
	double Angle;

public:
	Particle2();
	virtual ~Particle2();


};

#endif /* PARTICLE2_H_ */
