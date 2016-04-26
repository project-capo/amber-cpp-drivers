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

class Room;

using std::string;
using namespace std;


class MazeWall
{
public:
	MazeWall();
	virtual ~MazeWall();

public :

	bool 	IsGate;
	Room* NextRoom;

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

	double Round(double dbVal);

	void Calculate();
};

#endif /* MAZEWALL_H_ */
