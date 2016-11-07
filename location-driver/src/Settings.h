/*
 * Settings.h
 *
 *  Created on: Nov 7, 2016
 *      Author: szsz
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

class Settings {
public:

	char* PathMap;
	unsigned int NumberParticles;
	double Epsilon;
	int Generation;
	unsigned int NumberRandomNewParticles;
	unsigned int SkipScan;
	char* DiagnosticIPAddress;

	//kht
	int maxMeasuredDistance;
	int houghResolution;

	char *RobotIP;

	Settings();
	virtual ~Settings();
};

#endif /* SETTINGS_H_ */
