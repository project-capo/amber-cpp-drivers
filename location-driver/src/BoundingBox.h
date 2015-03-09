#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include <string.h>
#include <string>
#include <vector>
#include <float.h>
#include <math.h>

using std::string;
using namespace std;


struct BoundingBox
{
public:
	double X_Left_Top;
	double Y_Left_Top;

	double X_Left_Bottom;
	double Y_Left_Bottom; 

	double X_Right_Top;
	double Y_Right_Top;

	double X_Right_Bottom;
	double Y_Right_Bottom; 

	BoundingBox();
};


struct MazeSpaces
{
public:
	string Type;
	string SpaceId;
	string WallId;
};

#endif /* BOUNDINGBOX_H_ */
