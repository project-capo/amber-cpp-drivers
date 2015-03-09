/*
 * Base.cpp
 *
 *  Created on: Feb 18, 2015
 *      Author: szsz
 */

#include "Base.h"

Base::Base()
{
	EPSILON = 0.0000001;
}

Base::~Base()
{
	// TODO Auto-generated destructor stub
}

bool Base::IsEqual(double value1,double value2)
{
	return  (abs((value1 - value2)) < EPSILON);
}
