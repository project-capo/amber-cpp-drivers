/*
 * JesonHelper.h
 *
 *  Created on: Feb 18, 2015
 *      Author: szsz
 */

#ifndef JESONHELPER_H_
#define JESONHELPER_H_

#define _CRT_SECURE_NO_WARNINGS 1
#define _SCL_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

#include <string.h>
#include <string>
#include <vector>
#include <float.h>
#include <math.h>

#include "Room.h";

using std::string;
using namespace std;


int conteins(vector<Room> rooms,string textToFind);

char* OpenFile(char *filename);

/*  Get Walls */
int GetMazeWalls(cJSON *json,MazeWall* & tableWalls);

int GetMazeSpaces(cJSON *json,MazeSpaces* & tableSpaces);

int GetRooms(MazeWall* wall,int WallsCount,MazeSpaces* spaces,int SpacesCount,MazeWall* gate,int GateCount,MazeSpaces* gate_spaces,int GateSpacesCount,Room* & rooms);

//int AssignBoundingBox(Room* rooms,int RoomCount,BoundingBox* & box);

int  parseJasonFile(char *filename,Room* & rooms);

MazeWall FindWall(MazeWall* wall,int WallsCount,string id);

int GetMazeGate(cJSON *json,MazeWall* & tableWalls);

int GetMazeSpaces(cJSON *json,MazeSpaces* & tableSpaces);



#endif /* JESONHELPER_H_ */
