/*
 * MazeWall.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: szsz
 */

#include "MazeWall.h"

MazeWall::MazeWall()
{
	IsGate = false;
}

MazeWall::~MazeWall()
{
	// TODO Auto-generated destructor stub
}

double MazeWall::Round(double dbVal)
{
    return dbVal;
}

void MazeWall::Calculate()
{
	if(From_Y ==  To_Y)
	{
		A = 0;
		B = 1;
		C = -1 * From_Y;
	}
	else if(From_X ==  To_X)
	{
		A = 1;
		B = 0;
		C = -1 *  From_X;
	}
	else
	{

	A = (From_Y - To_Y);
	B = (To_X - From_X);
	C =  (From_X * To_Y) - (From_Y * To_X);
	}

	A = Round(A);
	B = Round(B);
		C = Round(C);
}
