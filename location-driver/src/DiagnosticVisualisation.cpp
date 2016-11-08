/*
 * DiagnosticVisualisation.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: szsz
 */

#include "DiagnosticVisualisation.h"

DiagnosticVisualisation::DiagnosticVisualisation(UdpClient *udpClient,unsigned int numberParticles,Particle2 *particleTable, RoboclawProxy *roboClaw, HokuyoProxy *hokuyo) {
	client = udpClient;
	NumberParticles = numberParticles;
	Particles = particleTable;
	RoboClaw = roboClaw;
	Hokuyo = hokuyo;

	rootJSON = cJSON_CreateObject();
	roboClawJSON = cJSON_CreateObject();
	hokuyoJSON =cJSON_CreateObject();
	particlesJSON =cJSON_CreateObject();

	cJSON_AddItemToObject(rootJSON, "RoboClaw",roboClawJSON);
	cJSON_AddItemToObject(rootJSON, "Hokuyo",hokuyoJSON);
	cJSON_AddItemToObject(rootJSON, "Particles",particlesJSON);






	/*cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
	cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
	cJSON_AddStringToObject(fmt, "type", "rect");
	cJSON_AddNumberToObject(fmt, "width", 1920);
	cJSON_AddNumberToObject(fmt, "height", 1080);
	cJSON_AddFalseToObject (fmt, "interlace");
	cJSON_AddNumberToObject(fmt, "frame rate", 24);

	{
    "name": "Jack (\"Bee\") Nimble",
    "format": {
        "type":       "rect",
        "width":      1920,
        "height":     1080,
        "interlace":  false,
        "frame rate": 24
    }
}

	*/
}

DiagnosticVisualisation::~DiagnosticVisualisation() {
	delete client;
}

void DiagnosticVisualisation::Send()
{

}
