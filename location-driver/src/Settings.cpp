/*
 * Settings.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: szsz
 */

#include "Settings.h"

Settings::Settings() {
	// TODO Auto-generated constructor stub

	maxMeasuredDistance = 5600;
    houghResolution = 20;

    RobotIP = "127.0.0.1";


    //PathMap = NULL;
    NumberParticles = 10;
    Epsilon = 0;
    Generation = 0;
    NumberRandomNewParticles = 10;
    SkipScan = 1;
    DiagnosticIPAddress = "127.0.0.1";

}

Settings::~Settings() {
	// TODO Auto-generated destructor stub
}

