/*
 * MazeWall.h
 *
 *  Created on: Feb 16, 2015
 *      Author: szsz
 */

#ifndef MAZEWALL_H_
#define MAZEWALL_H_

#include <stdexcept>
#include <string.h>

using std::string;
using namespace std;


class MazeWall {
public:
	MazeWall();
	virtual ~MazeWall();

	//static bool compareMyDataPredicate(MazeWall lhs, MazeWall rhs) { return true; }

//	bool operator < (const MazeWall& tmp) const;


public :

	bool 	IsGate;
	string 	Type;
	string		Id;
	double		Width;
	double		Height;
	string		Color;
	double 		From_X;
	double		From_Y;
	double		To_X;
	double		To_Y;

	double A;
	double B;
	double C;


	inline double Round(double dbVal)
	{
	    return dbVal;
	}

	void Calculate()
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
};

#endif /* MAZEWALL_H_ */
