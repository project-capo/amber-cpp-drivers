/*
 * Base.h
 *
 *  Created on: Feb 18, 2015
 *      Author: szsz
 */

#ifndef BASE_H_
#define BASE_H_

#include <cmath>

using namespace std;

class Base
{
protected:
	double EPSILON;
	bool IsEqual(double value1,double value2);
public:

	Base();
	virtual ~Base();
};

#endif /* BASE_H_ */
