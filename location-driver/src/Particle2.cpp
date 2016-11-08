/*
 * Particle2.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: szsz
 */

#include "Particle2.h"

Particle2::Particle2() {
	// TODO Auto-generated constructor stub
	 X =1.0;
		 Y = 2.2;
		 P = 3.2;
		 Angle = 4.2;

#if DIAGNOSTIC_MODE == 1
		 Length = 20;
		 CalculatedProbability = new double[Length];
		 CalculatedDistances = new double[Length];
#endif
}

Particle2::~Particle2() {

#if DIAGNOSTIC_MODE == 1
	delete CalculatedProbability;
	delete CalculatedDistances;
#endif



}

