/*
 * Room.h
 *
 *  Created on: Feb 16, 2015
 *      Author: szsz
 */

#ifndef ROOM_H_
#define ROOM_H_

#include <stdexcept>
#include <algorithm>
#include <string.h>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include "MazeWall.h"
#include "BoundingBox.h"
#include "Base.h"


using std::string;
using namespace std;

class Room : Base
{
private:
	unsigned int currentWallIndex;
	bool direction;

	int getWallIndex(int startIndex,double X,double Y);
	void reorderWalls();
	void recountBB(MazeWall wall);

public:
	string SpaceId;
	vector<MazeWall> walls;

	Room();
	void AddWall(MazeWall wall);

	MazeWall* CurrentWall();
	MazeWall* GetNextWall();

	virtual ~Room();

	BoundingBox Box;

	//void ResetDirection();

	void ChangeDirection();

//private:
//	int iContainerWallCount;

public:  // do wywalenia

	vector<MazeWall> ContainerWall;

	MazeWall *ContainerWallTable;


	//przeniesc inicjalizacje gdy dodajemy nowe sciany
	int ContainerWallCount();

	void CreateContainerWall();
//	{
//		int iContainerWallCount = ContainerWall.size();
//		ContainerWallTable = new MazeWall[iContainerWallCount];
//		copy(ContainerWall.begin(),ContainerWall.end(),ContainerWallTable);
//	}
//
//

	//int getWallIndex(double From_X,double  From_Y,double To_X,double To_Y);

	//void insertWall(unsigned int index,MazeWall wall);
	//int getWallIndex(double From_X,double  From_Y,double To_X,double To_Y);
	//int getWallIndex(double To_X,double To_Y);

	//bool compareMyType (MazeWall tmp_a, MazeWall tmp_b);
};


#endif /* ROOM_H_ */
