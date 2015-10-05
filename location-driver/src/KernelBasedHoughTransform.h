#pragma once
class KernelBasedHoughTransform
{
public:
	KernelBasedHoughTransform(unsigned int maxScanDistanceMilimeters, unsigned int houghResolutionMilileters = 20);
	~KernelBasedHoughTransform();

	double GetMainDirectionAngle(double* scanAnglesRadians, int* scanDistancesMilimeters, unsigned int measuresCount);

protected:
	unsigned char *binary_image;
	unsigned int binaryImageHalfEdge;
	double houghResolutionMilileters;
};

