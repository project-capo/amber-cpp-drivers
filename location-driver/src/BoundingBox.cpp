#include "BoundingBox.h"

BoundingBox::BoundingBox()
	{
		X_Left_Top =
		X_Left_Bottom =
		Y_Left_Bottom =
		X_Right_Bottom = DBL_MAX;

		Y_Left_Top =
		X_Right_Top =
		Y_Right_Top =
		Y_Right_Bottom = 0;
	}

