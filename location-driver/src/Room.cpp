/*
 * Room.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: szsz
 */

#include "Room.h"

Room::Room()
{
	currentWallIndex = 0;
	ContainerWall = walls; //do wywalenia
	direction = false;
}

Room::~Room()
{
	walls.clear();
}

void Room::AddWall(MazeWall wall)
{
	walls.push_back(wall);

	reorderWalls();
	recountBB(wall);
}

MazeWall* Room::CurrentWall()
{
	return &walls[currentWallIndex];
}

MazeWall* Room::GetNextWall()
{
	if(direction)
	{
		currentWallIndex++;

		if(currentWallIndex >= walls.size())
			currentWallIndex = 0;
	}
	else
	{
		if(currentWallIndex == 0)
			currentWallIndex = walls.size() -1;
		else
			currentWallIndex--;
	}

	return &walls[currentWallIndex];

}

int Room::getWallIndex(int startIndex, double X,double Y)
{
	for(unsigned int i = startIndex; i < walls.size(); i++)
	{
		if((IsEqual(X, walls[i].From_X) && IsEqual(Y,walls[i].From_Y)) || (IsEqual(X, walls[i].To_X) && IsEqual(Y,walls[i].To_Y)))
			return i;
	}

	return -1;
}

void Room::reorderWalls()
{
	int index;
	std::vector<MazeWall>::iterator it;
	MazeWall tempMaze;
	double tmpX =  walls[0].To_X;
	double tmpY = walls[0].To_Y;

	for(unsigned int i = 0; i < walls.size();i++)
	{
		index = getWallIndex(i + 1, tmpX ,tmpY);

		if(IsEqual(tmpX, walls[index].To_X) &&  IsEqual(tmpY,walls[index].To_Y))
		{
			tmpX =  walls[index].From_X;
			tmpY = walls[index].From_Y;
		}
		else
		{
			tmpX =  walls[index].To_X;
			tmpY = walls[index].To_Y;
		}

		it = walls.begin();

		if(index > -1)
		{
			tempMaze = walls[index];
			walls.erase(it + index);
			walls.insert(it + i + 1,tempMaze);
		}
	}
}

void Room::recountBB(MazeWall wall)
{
	Box.X_Left_Top = Box.X_Left_Bottom = min(Box.X_Left_Top,min(wall.From_X,wall.To_X));
	Box.Y_Left_Bottom = Box.Y_Right_Bottom = min(Box.Y_Left_Bottom,min(wall.From_Y,wall.To_Y));
	Box.Y_Left_Top =  Box.Y_Right_Top = max(Box.Y_Left_Top,max(wall.From_Y,wall.To_Y));
	Box.X_Right_Top = Box.X_Right_Bottom = max(Box.X_Right_Top,max(wall.From_X,wall.To_X));
}

int Room::ContainerWallCount()
{
	return walls.size();
}

void Room::ChangeDirection()
{
	direction = !direction;
	GetNextWall();
	GetNextWall();
}

void Room::CreateContainerWall()
{
	int iContainerWallCount = ContainerWall.size();
	ContainerWallTable = new MazeWall[iContainerWallCount];
	copy(ContainerWall.begin(),ContainerWall.end(),ContainerWallTable);
}



///*
// *
// *
//int Room::getWallIndex(double From_X,double  From_Y,double To_X,double To_Y)
//{
//	for(unsigned int i = 0; i < walls.size(); i++)
//	{
//		if((IsEqual(walls[i].From_X, To_X) && IsEqual(walls[i].From_Y, To_Y)) ||
//				(IsEqual(walls[i].To_X, From_X) && IsEqual(walls[i].To_Y, From_Y)) || (IsEqual(walls[i].To_X, To_X) && IsEqual(walls[i].To_Y, To_Y)))
//			return i;
//	}
//
//	return -1;
//}
//void Room::insertWall(unsigned int index, MazeWall wall)
//{
//	std::vector<MazeWall>::iterator it;
//	it = walls.begin();
//
//	if(index < 0)
//		walls.insert(it,wall);
//	else if(index > walls.size())
//		walls.push_back(wall);
//	else
//		walls.insert(it+index, wall);
//}*/
//
////int Room::getWallIndex(double From_X,double  From_Y,double To_X,double To_Y)
////{
////	for(unsigned int i = 0; i < walls.size(); i++)
////	{
////		if((IsEqual(walls[i].From_X, From_X) && IsEqual(walls[i].From_Y, From_Y)) || (IsEqual(walls[i].From_X, To_X) && IsEqual(walls[i].From_Y, To_Y)) ||
////				(IsEqual(walls[i].To_X, From_X) && IsEqual(walls[i].To_Y, From_Y)) || (IsEqual(walls[i].To_X, To_X) && IsEqual(walls[i].To_Y, To_Y)))
////			return i;
////	}
////
////	return -1;
////}
////int Room::getWallIndex(double From_X,double  From_Y)
////{
////	for(unsigned int i = 0; i < walls.size(); i++)
////	{
////		if((IsEqual(walls[i].From_X, From_X) && IsEqual(walls[i].From_Y, From_Y)))
////			return i;
////	}
////
////	return -1;
////}
////int Room::getWallIndex(double To_X,double To_Y)
////{
////	for(unsigned int i = 0; i < walls.size(); i++)
////	{
////		//		if((IsEqual(walls[i].From_X, From_X) && IsEqual(walls[i].From_Y, From_Y)) || (IsEqual(walls[i].From_X, To_X) && IsEqual(walls[i].From_Y, To_Y)) ||
////		//		   (IsEqual(walls[i].To_X, From_X) && IsEqual(walls[i].To_Y, From_Y)) || (IsEqual(walls[i].To_X, To_X) && IsEqual(walls[i].To_Y, To_Y)))
////
////		//if ((IsEqual(To_X,walls[i].From_X) && IsEqual(To_Y,walls[i].From_Y)) || (IsEqual(To_X,walls[i].To_X) && IsEqual(To_Y,walls[i].To_Y)))
////		if ((IsEqual(To_X,walls[i].From_X) && IsEqual(To_Y,walls[i].From_Y)) || (IsEqual(To_X,walls[i].To_X) && IsEqual(To_Y,walls[i].To_Y)))
////			return i;
////	}
////
////	return -1;
////}
//
///*
// *
// * 	//ContainerWallTable = &walls;//do wywalenia
//
//bool Room::compareMyType (const void * a, const void * b)
//{
//	const MazeWall* tmp_a = (const MazeWall *) a;
//	const MazeWall* tmp_b = (const MazeWall *) b;
//
//	/*if (tmp_a->Probability < tmp_b->Probability) return 1;
//	else if(tmp_a->Probability == tmp_b->Probability)return 0;
//	else return -1;
//
//	if(tmp_a->To_X == tmp_b->From_X && tmp_a->To_Y == tmp_b->From_Y)
//		return false;
//	else
//		return true;
//}*/
//
////bool Room::compareMyType (MazeWall tmp_a, MazeWall tmp_b)
////{
////	//const MazeWall* tmp_a = (const MazeWall *) a;
////	//const MazeWall* tmp_b = (const MazeWall *) b;
////
////	/*if (tmp_a->Probability < tmp_b->Probability) return 1;
////	else if(tmp_a->Probability == tmp_b->Probability)return 0;
////	else return -1;*/
////
////	if(tmp_a.To_X == tmp_b.From_X && tmp_a.To_Y == tmp_b.From_Y)
////		return false;
////	else
////		return true;
////}
//
////bool Room::operator() (MazeWall tmp_a, MazeWall tmp_b)
////{
////	return true;
////}
//
////bool Room::operator < (const MazeWall& str) const
////{
////        return true;//(key < str.key);
////}
//
////void Room::move(int indexForm,)
//
//
//
////std::vector<MazeWall>::iterator it = walls.begin();
//
////	int indexBefore = getWallIndex(wall.From_X ,wall.From_Y);
////	int indexAfter = getWallIndex(wall.To_X,wall.To_Y);
////
//////	if(indexBefore > -1)
//////	{
//////		if(indexBefore == 0) //gdy jestesmy na pierwszej pozycji
//////			walls.insert(it + indexBefore,wall);
//////		else
//////			walls.insert(it + (indexBefore),wall);
//////	}
////	if(indexAfter > -1)
////		walls.insert(it + (indexAfter),wall);
////	else
////		walls.push_back(wall);
//
//
//
//
////
////	int indexBefore = getWallIndex(wall.To_X,wall.To_Y);
////	int indexAfter = getWallIndex(wall.From_X,wall.From_Y,wall.To_X,wall.To_Y);
////
////	if(indexBefore > -1)
////		walls.insert(it + (indexBefore -1),wall);
////	else if(indexAfter > -1)
////		walls.insert(it + (indexAfter),wall);
////	else
////		walls.push_back(wall);
//
////	if(((indexBefore == -1) && (indexAfter == -1)) || walls.size() == 0)
////
////	else if(indexBefore > -1)
////
////	else
////	else
////		walls.insert(it + (index),wall);
//
//
//
////From_X,double  ,double To_X,double To_Y
//
///****** Sortowanie ******/
////walls.push_back(wall);
//
////std::sort(walls.begin(),walls.end());
//
////qsort(walls.begin(),walls.end(),sizeof(MazeWall),compareMyType);
//
//
///*if(walls.size() == 0)
//	walls.push_back(wall);
//else
//{*/
//
//
///*std::vector<MazeWall>::iterator
//it = walls.begin();
//int index = getWallIndex(wall.From_X,wall.From_Y,wall.To_X,wall.To_Y);
//
//if(index == -1 || index == 0)
//	walls.push_back(wall);
//else
//
//	walls.insert(it + (index +1),wall);
//
//
// */
//
///*if((IsEqual(walls[index].From_X,wall.From_X) &&  IsEqual(walls[index].From_Y,wall.From_Y)) || (IsEqual(walls[index].From_X,wall.To_X) &&  IsEqual(walls[index].From_Y,wall.To_Y))) //za  //From == From || From == To
//		walls.insert(it + (index+1),wall);
//	else //przed // To == From To == To
//	{
//		if(index == 0) //gdy jestesmy na pierwszej pozycji
//			walls.insert(it + index,wall);
//		else
//			walls.insert(it + (index),wall);
//	}*/
////}
//
///*
//if(index == -1)
//		walls.push_back(wall);
//	else
//	{
//		if((IsEqual(walls[index].From_X,wall.From_X) &&  IsEqual(walls[index].From_Y,wall.From_Y)) || (IsEqual(walls[index].From_X,wall.To_X) &&  IsEqual(walls[index].From_Y,wall.To_Y))) //za  //From == From || From == To
//			walls.insert(it + (index+1),wall);
//		else //przed // To == From To == To
//		{
//			if(index == 0) //gdy jestesmy na pierwszej pozycji
//				walls.insert(it + index,wall);
//			else
//				walls.insert(it + (index),wall);
//		}
//	}*/
//
//
///*bool insertComplet = false;
//
//for(int i = 0; i <walls.size(); i++)
//{
//	if((abs(walls[i].From_X - walls[i].To_X) < EPSILON) && ((abs(wall.From_X - wall.To_X)) > EPSILON)) // sciana pionowa
//	{
//		if((abs(walls[i].From_X - wall.From_X) < EPSILON) || (abs(walls[i].To_X - wall.To_X) < EPSILON))
//			walls.insert(it + (i+1),wall);
//		else
//			walls.insert(it + i,wall);
//
//		insertComplet = true;
//		break;
//	}
//	else if((abs(walls[i].From_Y - walls[i].To_Y) < EPSILON) && ((abs(wall.From_Y - wall.To_Y)) > EPSILON)) //sciana pozioma
//	{
//		if((abs(walls[i].From_Y - wall.From_Y) < EPSILON) || (abs(walls[i].To_Y - wall.To_Y) < EPSILON))
//			walls.insert(it + (i+1),wall);
//		else
//			walls.insert(it + i,wall);
//
//		insertComplet = true;
//		break;
//	}
//	//else //ukosna
//	//	throw std::invalid_argument("Invalid add wall: " + wall.Id);
//}
//
//
//
//
//if(!insertComplet)
//	walls.push_back(wall);
//
///*if(((wall.From_X - walls[i].From_X) < EPSILON) || ((wall.From_X - walls[i].To_X) < EPSILON))
//	{
//		walls.insert(it + i,wall);
//		insertComplet = true;
//		break;
//	}
//	else if(((wall.To_X - walls[i].From_X) < EPSILON) || ((wall.To_X - walls[i].To_X) < EPSILON))
//	{
//		walls.insert(it + (i +1),wall);
//		insertComplet = true;
//		break;
//	}*/
//
////	throw std::invalid_argument("Invalid add wall: " + wall.Id);
//
///*std::vector<MazeWall>::iterator itLeft, itRight;
//	itLeft = walls.begin();
//	itRight = walls.end();
//
//	if(((wall.From_X - itLeft->From_X) < EPSILON) && ((wall.From_Y - itLeft->From_Y) < EPSILON))
//		walls.insert(itLeft,wall);
//	else if(((wall.To_X - itLeft->To_X) < EPSILON) && ((wall.To_Y - itLeft->To_Y) < EPSILON))
//		walls.insert(itRight,wall);
//	else
// */
////}
