/*
 * HoughTransormat.cpp
 *
 *  Created on: 12 Jul 2015
 *      Author: szsz
 */

#include "HoughTransormat.h"

HoughTransormat::HoughTransormat() {
	// TODO Auto-generated constructor stub

	Epsilon = 10.0;

}

HoughTransormat::~HoughTransormat() {
	// TODO Auto-generated destructor stub
}


float HoughTransormat::GetAngle(double* angles,int* distnaces,unsigned int length)
{
	//double* tabC = new double[length];
	Collinear* tabC = new Collinear[length];
	unsigned int indexCount;

	//tabC[0] = 11;

	//double maxC, maxAngle, maxCount;
	//double currentC, currentAngle, currentCount;

	tabC[0].Alfa = angles[0];
	tabC[0].C = getC(distnaces[0],angles[0]);
	tabC[0].Count = 1;

	indexCount = 0;


	double tempC;

for(unsigned int i = 1; i < length; i++)
{
	tempC = getC(distnaces[i],angles[i]);

	if(isCollinear(tabC[indexCount].C,tempC))
	{
		tabC[indexCount].Alfa = angles[i];
		tabC[indexCount].C = tempC;
		tabC[indexCount].Count++;
	}
	else
	{
		indexCount++;
		tabC[indexCount].Alfa = angles[i];
		tabC[indexCount].C = tempC;
		tabC[indexCount].Count = 1;
	}

}

for(int i= 0; i < indexCount + 1; i++)
	printf("angle: %f,c: %f, count %d\n",tabC[i].Alfa,tabC[i].C,tabC[i].Count);


for(unsigned int i = 0; i < length; i++)
{
	printf("%f; ",angles[i]);

}

printf("Nowe\n");

for(unsigned int i = 0; i < length; i++)
{
	printf("%d; ",distnaces[i]);
}

return 0.0;
}

double HoughTransormat::getC(int r,double fi)
{
return r * cos(fi);
}

bool HoughTransormat::isCollinear(double c0,double c1)
{
return (abs(c0 - c1) <= Epsilon);
}
