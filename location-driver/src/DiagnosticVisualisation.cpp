/*
 * DiagnosticVisualisation.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: szsz
 */

#include "DiagnosticVisualisation.h"

DiagnosticVisualisation::DiagnosticVisualisation(UdpClient *udpClient,
		unsigned int numberParticles, Particle2 *particleTable,
		RoboclawProxy *roboClaw, HokuyoProxy *hokuyo) {

	client = udpClient;
	NumberParticles = numberParticles;
	Particles = particleTable;
	RoboClaw = roboClaw;
	Hokuyo = hokuyo;

	rootJSON = cJSON_CreateObject();
	roboClawJSON = cJSON_CreateObject();
	hokuyoJSON = cJSON_CreateObject();
	particlesJSONTable = cJSON_CreateArray();

	//cJSON_AddItemToObject(rootJSON, "RoboClaw", roboClawJSON);
	cJSON_AddItemToObject(rootJSON, "Hokuyo", hokuyoJSON);
	cJSON_AddItemToObject(rootJSON, "Particles", particlesJSONTable);

	hokuyoJSONAngleTable = cJSON_CreateDoubleArray(Hokuyo->Angles,
			Hokuyo->Length);
	hokuyoJSONDistancesTable = cJSON_CreateIntArray(Hokuyo->Distances,
			Hokuyo->Length);

	cJSON_AddItemToObject(hokuyoJSON, "Angles", hokuyoJSONAngleTable);
	cJSON_AddItemToObject(hokuyoJSON, "Distances", hokuyoJSONDistancesTable);

	cJSON *itemParticle;
	cJSON *tableCalculatedProbability;
	cJSON *tableCalculatedDistances;

	for (unsigned int i = 0; i < NumberParticles; i++) {
		itemParticle = cJSON_CreateObject();

		cJSON_AddNumberToObject(itemParticle, "X", Particles[i].X);
		cJSON_AddNumberToObject(itemParticle, "Y", Particles[i].Y);
		cJSON_AddNumberToObject(itemParticle, "P", Particles[i].P);
		cJSON_AddNumberToObject(itemParticle, "Angle", Particles[i].Angle);

		tableCalculatedProbability = cJSON_CreateDoubleArray(
				Particles[i].CalculatedProbability, Particles[i].Length);

		cJSON_AddItemToObject(itemParticle, "CalculatedProbability",
				tableCalculatedProbability);

		tableCalculatedDistances = cJSON_CreateDoubleArray(
				Particles[i].CalculatedDistances, Particles[i].Length);
		cJSON_AddItemToObject(itemParticle, "CalculatedDistances",
				tableCalculatedDistances);

		cJSON_AddItemToArray(particlesJSONTable, itemParticle);
	}

	/*particlesJSONTable = cJSON_CreateArray();

	 cJSON *itemParticle;
	 for(int i = 0; i < NumberParticles;i++)
	 {
	 itemParticle = cJSON_CreateObject();

	 cJSON_AddNumberToObject(itemParticle,"X",Particles[i].X);


	 cJSON_AddItemToArray(particlesJSONTable,itemParticle);
	 }

	 */

//	cJSON_AddItemToObject(particlesJSON,"Particle",particlesJSONTable);
//(Particles,
//		NumberParticles);
//cJSON_AddNumberToObject(particlesJSON,"X",)
	char * rendered = cJSON_Print(rootJSON);

	printf(rendered);
	fflush(NULL);

	Hokuyo->Distances[0] = 777;
	Hokuyo->Angles[0] = 555;

	Particles[0].X = Particles[0].Angle = 888;

	Particles[0].CalculatedDistances[0] = 0.7777;
	Particles[0].CalculatedProbability[0] = 5000;

	Send();

	rendered = cJSON_Print(rootJSON);

	printf(rendered);
	fflush(NULL);

	//particlesJSONTable = cJSON_CreateArray();

	//cJSON_AddItemToArray()

//

//	cJSON_AddItemToObject(particlesJSON, "X", )

//cJSON_AddNumberToObject()

//cJSON_AddItemToObject(rootJSON,

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

void DiagnosticVisualisation::Send() {
	cJSON * subItemAngle;
	cJSON * subItemDistances;

	for (int i = 0; i < Hokuyo->Length; i++) {
		subItemDistances = cJSON_GetArrayItem(hokuyoJSONDistancesTable, i);
		subItemDistances->valuedouble = subItemDistances->valueint =
				Hokuyo->Distances[i]; //bug przy przypisywaniu dla valueint

		subItemAngle = cJSON_GetArrayItem(hokuyoJSONAngleTable, i);
		subItemAngle->valuedouble = Hokuyo->Angles[i];
	}

	cJSON *itemParticle;
	cJSON *tableCalculatedProbability;
	cJSON *tableCalculatedDistances;

	for (unsigned int i = 0; i < NumberParticles; i++) {

		itemParticle = cJSON_GetArrayItem(particlesJSONTable, i);

		cJSON_GetObjectItem(itemParticle, "X")->valuedouble = Particles[i].X;
		cJSON_GetObjectItem(itemParticle, "Y")->valuedouble = Particles[i].Y;
		cJSON_GetObjectItem(itemParticle, "P")->valuedouble = Particles[i].P;
		cJSON_GetObjectItem(itemParticle, "Angle")->valuedouble =
				Particles[i].Angle;

		tableCalculatedProbability = cJSON_GetObjectItem(itemParticle,
				"CalculatedProbability");
		tableCalculatedDistances = cJSON_GetObjectItem(itemParticle,
				"CalculatedDistances");

		for (unsigned int j = 0; j < Particles[i].Length; j++) {
			cJSON_GetArrayItem(tableCalculatedProbability, j)->valuedouble =
					Particles[i].CalculatedProbability[j];

			cJSON_GetArrayItem(tableCalculatedDistances, j)->valuedouble =
					Particles[i].CalculatedDistances[j];
		}
	}
}
