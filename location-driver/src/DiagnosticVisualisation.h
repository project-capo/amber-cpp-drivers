/*
 * DiagnosticVisualisation.h
 *
 *  Created on: Nov 8, 2016
 *      Author: szsz
 */

#ifndef DIAGNOSTICVISUALISATION_H_
#define DIAGNOSTICVISUALISATION_H_


#include "Particle2.h"
#include "RoboclawProxy.h"
#include "HokuyoProxy.h"
#include "UdpClientLinux.h"
#include "cJSON.h"

class DiagnosticVisualisation {
private:
	UdpClient *client;
	unsigned int NumberParticles;
	RoboclawProxy *RoboClaw;
	HokuyoProxy *Hokuyo;
	Particle2 *Particles;

	cJSON *rootJSON,*roboClawJSON,*hokuyoJSON,*particlesJSON;

public:
	DiagnosticVisualisation(UdpClient *udpClient,unsigned int numberParticles,Particle2 *particleTable, RoboclawProxy *roboClaw, HokuyoProxy *hokuyo);
    virtual ~DiagnosticVisualisation();

    void Send();
};

#endif /* DIAGNOSTICVISUALISATION_H_ */
