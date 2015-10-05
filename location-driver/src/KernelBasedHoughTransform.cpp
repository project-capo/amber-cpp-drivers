#include "KernelBasedHoughTransform.h"

#include "kht.h"

#include <algorithm>
#include <iostream>


KernelBasedHoughTransform::KernelBasedHoughTransform(unsigned int maxScanDistanceMilimeters, unsigned int houghResolutionMilileters)
{
	this->houghResolutionMilileters = (double)houghResolutionMilileters;
	this->binaryImageHalfEdge = maxScanDistanceMilimeters / houghResolutionMilileters + 1;
	this->binary_image = new unsigned char[(2 * binaryImageHalfEdge) * (2 * binaryImageHalfEdge)];
}


KernelBasedHoughTransform::~KernelBasedHoughTransform()
{
	delete this->binary_image;
}

double KernelBasedHoughTransform::GetMainDirectionAngle(double* scanAnglesRadians, int* scanDistancesMilimeters, unsigned int measuresCount)
{
//	for (int i = 0; i < this->binaryImageWidthAndHeight * this->binaryImageWidthAndHeight; i++)	//clear -- optimize this!
//		this->binary_image[i] = 0;
	std::fill(this->binary_image, this->binary_image + ((2 * binaryImageHalfEdge) * (2 * binaryImageHalfEdge)), 0);	//clear data

	int pointX, pointY, lastPointX = -100, lastPointY = -100;

	for (int scanId = 0; scanId < measuresCount; scanId++)
	{
		double distance = scanDistancesMilimeters[scanId] / this->houghResolutionMilileters;
		pointX = binaryImageHalfEdge + (int)(distance*sin(scanAnglesRadians[scanId]));
		pointY = (binaryImageHalfEdge - (int)(distance*cos(scanAnglesRadians[scanId])));

		if (abs(pointX - lastPointX) < 3 && abs(pointY - lastPointY) < 3)
		{
			for (int x = std::min(pointX, lastPointX); x <= std::max(pointX, lastPointX); x++)							// fill all points between -- works nice :)
			for (int y = std::min(pointY, lastPointY); y <= std::max(pointY, lastPointY); y++)
				binary_image[x + y * 2 * binaryImageHalfEdge] = 255;
			;

			//			binary_image[(pointX + lastPointX) / 2 + (pointY + lastPointY) * binaryImageHalfEdge] = 255;		//add point in between to kill the cluster
		}
		else
		{
			binary_image[pointX + pointY * 2 * binaryImageHalfEdge] = 255;
		}

		lastPointX = pointX;
		lastPointY = pointY;
	}


	lines_list_t lines;
	kht(lines, this->binary_image, this->binaryImageHalfEdge * 2, this->binaryImageHalfEdge * 2);

	if (lines.size() == 0)
		return -1;
	


	//calculate avg angle, then do it again, skipping errors
	double anglesSum = 0;
	int acceptedAnglesWithImportanceAccumulator = 0;
	int linesToTake = std::min(3,(int)lines.size());

	for (int i = 0; i < linesToTake; i++)
	{
//		std::cout << "angle:" << lines[i].theta << " distance:" << lines[i].rho << std::endl;

		double thetaNormalized = fmod(lines[i].theta, 90);
		anglesSum += thetaNormalized ;
		acceptedAnglesWithImportanceAccumulator += 1;
	}
	double averageAngle = anglesSum / acceptedAnglesWithImportanceAccumulator;
//	std::cout << "all angle average:" << averageAngle << std::endl;

	anglesSum = 0;
	acceptedAnglesWithImportanceAccumulator = 0;
	for (int i = 0; i < linesToTake; i++)
	{
		double thetaNormalized = fmod(lines[i].theta, 90);
		if (abs(thetaNormalized - averageAngle) < 6)
		{
			anglesSum += thetaNormalized ;
			acceptedAnglesWithImportanceAccumulator += 1;
		}
	}
	if (acceptedAnglesWithImportanceAccumulator == 0)
		return fmod(lines[0].theta, 90);

	averageAngle = anglesSum / acceptedAnglesWithImportanceAccumulator;

	return averageAngle;
}

