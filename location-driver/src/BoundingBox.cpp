#include "BoundingBox.h"

MazeWall FindWall(MazeWall* wall,int WallsCount,string id)
{
	for (int i = 0; i < WallsCount; i++)
	{
		if(wall[i].Id == id)
			return wall[i]; 
	}
}

int conteins(vector<Room> rooms,string textToFind)
{
	for (int i = 0; i < rooms.size(); i++)
		if(rooms.at(i).SpaceId == textToFind)
			return i;
	return -1; // -1 not find
}

char* OpenFile(char *filename)
{
	FILE *f=fopen(filename,"rb");
	fseek(f,0,SEEK_END);
	long len=ftell(f);
	fseek(f,0,SEEK_SET);
	char *data=(char*)malloc(len+1);
	fread(data,1,len,f);
	fclose(f);
	return data;
}

/*  Get Walls */
int GetMazeWalls(cJSON *json,MazeWall* & tableWalls)
{
	cJSON *walls,*wallsArray, *type,*id,*width,*height,*color,*from,*fromX,*fromY, *to,*toX,*toY;

	walls = cJSON_GetObjectItem(json,"walls");

	int wallsCount = cJSON_GetArraySize(walls);

	tableWalls = new MazeWall[wallsCount];

	for(int i = 0; i < wallsCount;i++)
	{
		wallsArray = cJSON_GetArrayItem(walls,i);

		type = cJSON_GetObjectItem(wallsArray,"type");
		tableWalls[i].Type = string(type->valuestring);

		id = cJSON_GetObjectItem(wallsArray,"id");
		tableWalls[i].Id =  string(id->valuestring);

		width = cJSON_GetObjectItem(wallsArray,"width");
		tableWalls[i].Width = width->valuedouble;

		height = cJSON_GetObjectItem(wallsArray,"height");
		tableWalls[i].Height = height->valuedouble;

		color = cJSON_GetObjectItem(wallsArray,"color");
		tableWalls[i].Color = string(color->valuestring);

		from = cJSON_GetObjectItem(wallsArray,"from");

		fromX = cJSON_GetObjectItem(from,"x");
		tableWalls[i].From_X = fromX->valuedouble;

		fromY = cJSON_GetObjectItem(from,"y");
		tableWalls[i].From_Y = fromY->valuedouble;

		to = cJSON_GetObjectItem(wallsArray,"to");

		toX = cJSON_GetObjectItem(to,"x");
		tableWalls[i].To_X = toX->valuedouble;

		toY = cJSON_GetObjectItem(to,"y");
		tableWalls[i].To_Y = toY->valuedouble;

		tableWalls[i].Calculate();
	}

	return wallsCount;
}

int GetMazeSpaces(cJSON *json,MazeSpaces* & tableSpaces)
{
	cJSON *space_walls,*space_wallsArray, *type, *spaceId, *wallId;

	space_walls = cJSON_GetObjectItem(json,"space-walls");

	int space_wallsCount = cJSON_GetArraySize(space_walls);

	tableSpaces = new MazeSpaces[space_wallsCount];

	for(int i = 0; i < space_wallsCount;i++)
	{
		space_wallsArray = cJSON_GetArrayItem(space_walls,i);

		type = cJSON_GetObjectItem(space_wallsArray,"type");
		tableSpaces[i].Type = string(type->valuestring);

		spaceId = cJSON_GetObjectItem(space_wallsArray,"spaceId");
		tableSpaces[i].SpaceId = string(spaceId->valuestring);

		wallId = cJSON_GetObjectItem(space_wallsArray,"wallId");
		tableSpaces[i].WallId = string(wallId->valuestring);
	}

	return space_wallsCount;
}

int GetRooms(MazeWall* wall,int WallsCount,MazeSpaces* spaces,int SpacesCount,Room* & rooms)
{
	vector<Room> temp;
	Room tmpRoom;
	int index;

	for (int i = 0; i < SpacesCount; i++)
	{
		index = conteins(temp,spaces[i].SpaceId);

		if(index == -1)
		{
			tmpRoom = Room();
			tmpRoom.SpaceId = spaces[i].SpaceId;
			tmpRoom.ContainerWall.push_back(FindWall(wall,WallsCount,spaces[i].WallId));

			temp.push_back(tmpRoom);  	
		}
		else
			temp[index].ContainerWall.push_back(FindWall(wall,WallsCount,spaces[i].WallId));
	}

	for(unsigned int i = 0; i < temp.size();i++)
	{
		temp[i].CreateContainerWall();
	}

	rooms = new Room[temp.size()];

	copy(temp.begin(),temp.end(),rooms);

	return temp.size();
}

int AssignBoundingBox(Room* rooms,int RoomCount,BoundingBox* & box) 
{
	vector<BoundingBox> tempBox;
	BoundingBox tmpBox;

	for (int i = 0; i < RoomCount; i++)
	{
		tmpBox = BoundingBox();

		for (int j = 0; j < rooms[i].ContainerWallCount(); j++)
		{			
			tmpBox.X_Left_Top = tmpBox.X_Left_Bottom = min(tmpBox.X_Left_Top,min(rooms[i].ContainerWall[j].From_X,rooms[i].ContainerWall[j].To_X));

			tmpBox.Y_Left_Bottom = tmpBox.Y_Right_Bottom = min(tmpBox.Y_Left_Bottom,min(rooms[i].ContainerWall[j].From_Y,rooms[i].ContainerWall[j].To_Y));
			tmpBox.Y_Left_Top =  tmpBox.Y_Right_Top = max(tmpBox.Y_Left_Top,max(rooms[i].ContainerWall[j].From_Y,rooms[i].ContainerWall[j].To_Y));

			tmpBox.X_Right_Top = tmpBox.X_Right_Bottom = max(tmpBox.X_Right_Top,max(rooms[i].ContainerWall[j].From_X,rooms[i].ContainerWall[j].To_X));
		}

		tempBox.push_back(tmpBox);

		rooms[i].Box = tmpBox;
	}

	box = new BoundingBox[tempBox.size()];
	copy(tempBox.begin(),tempBox.end(),box);

	return tempBox.size();
}

int parseJasonFile(char *filename,BoundingBox* & bBox,Room* & rooms)
{
	char* data;
	cJSON *json;
	MazeWall* allWalls = NULL;
	MazeSpaces* allSpaces = NULL;

	int WallsCount;
	int SpacesCount;

	int RoomsCount;
	int BoundingBoxCount;

	data = OpenFile(filename);			
	json=cJSON_Parse(data);

	WallsCount = GetMazeWalls(json,allWalls);
	SpacesCount = GetMazeSpaces(json,allSpaces);

	RoomsCount = GetRooms(allWalls,WallsCount,allSpaces,SpacesCount,rooms);

	BoundingBoxCount = AssignBoundingBox(rooms,RoomsCount,bBox);

	cJSON_Delete(json);
	free(data);
	return BoundingBoxCount;
}
