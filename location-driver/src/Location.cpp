
#include "Location.h"

#define TESTHOKU 0


Location::Location(char* mapPath,unsigned int numberParticles,double epsilon,int generation,unsigned int ilosc_losowanych_nowych_czastek,unsigned int skipScan,char* sIPPart)
{
#if DIAGNOSTIC == 1
	IPPart =  sIPPart;// "192.168.2.101";//"172.29.53.31";//"192.168.56.1";//"192.168.2.102";//"169.254.162.40"; //wizualizacja
	clientParticle = new UdpClient(IPPart,1234,9000); //wizualizacja
#endif

	amberUdp = getRobotIPAdress(); //przerobic aby bral lokalny adres z robota
	clinetAmber = new UdpClient(amberUdp,26233,9000);

	srand(10);

	countRoomAndBox = parseJasonFile(mapPath,bBox,rooms);

	/*for(int i = 0; i <rooms[0].ContainerWallCount(); i++)
	{

	printf("%s\tA:%fB:%f\C:%fFrom_X:%fFrom_Y:%fTo_X:%f,To_Y:%f\n",rooms[0].ContainerWall[i].Id.c_str(),rooms[0].ContainerWall[i].A,rooms[0].ContainerWall[i].B,rooms[0].ContainerWall[i].C,
			rooms[0].ContainerWall[i].From_X,rooms[0].ContainerWall[i].From_Y,rooms[0].ContainerWall[i].To_X,rooms[0].ContainerWall[i].To_Y);

	}*/

	NumberParticles = numberParticles;
	tablicaCzastek = new Particle[NumberParticles];

	iloscCzastekDoUsuniacia = 0;

	roboClaw = new RoboclawProxy(clinetAmber);

	skaner = new HokuyoProxy(clinetAmber,skipScan);

	EPSILON = epsilon;
	GENERATION = generation;
	ILOSC_LOSOWANYCH_NOWYCH_CZASTEK = ilosc_losowanych_nowych_czastek;
}

Location::~Location()
{
	delete clinetAmber;
	delete tablicaCzastek;

#if DIAGNOSTIC == 1
	delete clientParticle;
#endif
}


void Location::RunLocation()
{
//	RozmiescCzastki(bBox,countRoomAndBox,tablicaCzastek,NumberParticles);
	InitTablicaCzastekLosowo(tablicaCzastek,bBox,countRoomAndBox);

#if DIAGNOSTIC == 1
	SendParticle(&diagnostic,tablicaCzastek);
	wys = diagnostic.c_str();
	size = diagnostic.size();
	clientParticle->Send(wys,size);
#endif

	//speedRoboClaw = roboClaw->GetSpeed(); //??? po co

	gettimeofday(&start, NULL);

#if DIAGNOSTIC == 1
	SendParticle(&diagnostic,tablicaCzastek);
	wys = diagnostic.c_str();
	size = diagnostic.size();
	clientParticle->Send(wys,size);
#endif

	while(true)
	{
		gettimeofday(&end, NULL);
		deletaTime = ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0) / 1000;
		gettimeofday(&start, NULL);

#if TESTHOKU == 0


		skaner->GetScan();
		speedRoboClaw = roboClaw->GetSpeed(); //droga w metrach
		angleRoboClaw = roboClaw->GetAngle(deletaTime);
#endif


#if TESTHOKU == 1


		skaner->GetScan();
		speedRoboClaw = 0;//roboClaw->GetSpeed(); //droga w metrach
		angleRoboClaw = 0;//roboClaw->GetAngle(deletaTime);
#endif

		for (unsigned int i = 0; i < NumberParticles; i++)
		{
			currentRoom = GetRoom(rooms,countRoomAndBox,tablicaCzastek[i].X,tablicaCzastek[i].Y); //pobranie informacji w ktrorym BB jest czastka

			if(currentRoom == NULL)
			{
				tablicaCzastek[i].Probability = 0.0;
				iloscCzastekDoUsuniacia++;
				continue;
			}

			tablicaCzastek[i].UpdateCountProbability5(currentRoom, skaner->GetDistances(),skaner->GetAngles(),skaner->ScanLength); //przeliczamy prawdopodobienstwa

			/*if(tablicaCzastek[i].sMarkToDelete > GENERATION)
			iloscCzastekDoUsuniacia++;
			else
			{*/
			if(tablicaCzastek[i].Probability <  EPSILON)
			{
				tablicaCzastek[i].sMarkToDelete++;
				iloscCzastekDoUsuniacia++;
			}
			else
				tablicaCzastek[i].sMarkToDelete = 0;

			tablicaCzastek[i].ZaktualizujPrzesuniecie4(roboClaw->wheelTrack,roboClaw->Vr,roboClaw->Vl,deletaTime);
		}

#if DIAGNOSTIC == 1
		SendParticle(&diagnostic,tablicaCzastek);
		wys = diagnostic.c_str();
		size = diagnostic.size();
		clientParticle->Send(wys,size);
#endif

		qsort(tablicaCzastek,NumberParticles,sizeof(Particle),compareMyType);

		this->Pos_X = tablicaCzastek[0].X;
		this->Pos_Y = tablicaCzastek[0].Y;
		this->Prop = tablicaCzastek[0].Probability;
		this->Pos_Alfa = tablicaCzastek[0].Alfa;


#if DIAGNOSTIC == 1
		SendParticle(&diagnostic,tablicaCzastek);
		wys = diagnostic.c_str();
		size = diagnostic.size();
		clientParticle->Send(wys,size);
#endif


		UsunWylosujNoweCzastki6(tablicaCzastek,NumberParticles,iloscCzastekDoUsuniacia);
		iloscCzastekDoUsuniacia = 0;

#if DIAGNOSTIC == 1
		SendParticle(&diagnostic,tablicaCzastek);
		wys = diagnostic.c_str();
		size = diagnostic.size();
		clientParticle->Send(wys,size);

		printf("Czas:%f[s]\n",deletaTime);
		fflush(NULL);
#endif
	}
}

void Location::RozmiescCzastki(BoundingBox* bbBox,unsigned int BoundingBoxCount,Particle* ttablicaCzastek,unsigned int ParticleCount)
{
	double maxX = 0.0;
		double maxY = 0.0;

		for(unsigned int i = 0; i < BoundingBoxCount; i++)
		{
		 maxX =  std::max(maxX,bbBox[i].X_Right_Top);
		 maxY =  std::max(maxY,bbBox[i].Y_Right_Top);
		}

	Particle tempRef;
	tempRef.X = 0.3;
	tempRef.Y = 0.2;
	tempRef.Alfa = M_PI / 2;
	//tempRef.AlfaStopnie = 90;
	double przesuniecie = 0.0;
	for(unsigned int i = 0; i < ParticleCount; i++)
	{
		for(unsigned int j = 0; j < 8; j++)
		{
			/*Particle *temp = new Particle();
			temp->X = tempRef.X;
			temp->Y = tempRef.Y + przesuniecie;
			temp->Alfa = tempRef.Alfa;
			temp->AlfaStopnie = tempRef.AlfaStopnie;
			temp->Probability = 0.0;
			tablicaCzastek[i] = temp;
			*/
			ttablicaCzastek[i].X = tempRef.X;
			ttablicaCzastek[i].Y = tempRef.Y;
			ttablicaCzastek[i].Alfa = tempRef.Alfa;
			//tablicaCzastek[i].AlfaStopnie = tempRef.AlfaStopnie;
			ttablicaCzastek[i].Probability = 0.0;
			ttablicaCzastek[i].maxX = maxX;
			ttablicaCzastek[i].maxY = maxY;

			i++;
			przesuniecie = 0.4;
			tempRef.X += przesuniecie;
		}
		tempRef.X = 0.3;
		tempRef.Y += 0.4;
		i--;
	}
	/*
	tempRef.X = 0.3 + 0.4;
	tempRef.Y = 3.35;
	//tempRef.Alfa = 0;
	for(unsigned int i = 4, p = 4; i < 8; i++, p++)
	{
	Particle temp;
	temp.X = tempRef.X;
	temp.Y = tempRef.Y;
	temp.Alfa = tempRef.Alfa;
	//temp.LosujPozycje(bBox[p].X_Left_Bottom,bBox[p].X_Right_Bottom,bBox[p].Y_Left_Bottom,bBox[p].Y_Left_Top);
	tablicaCzastek[i] = temp;
	if((p + 1) == BoundingBoxCount)
	p = -1;
	tempRef.X += 0.4;
	}
	*/
}

void Location::InitTablicaCzastekLosowo(Particle *tablica,BoundingBox* bbBox,int iCountBox)
{
	double maxX = 0.0;
	double maxY = 0.0;

	for(int i = 0; i < iCountBox; i++)
	{
	 maxX =  std::max(maxX,bbBox[i].X_Right_Top);
	 maxY =  std::max(maxY,bbBox[i].Y_Right_Top);
	}

	for (unsigned int i = 0; i < NumberParticles; i++)
	{
		Particle *temp = new Particle(maxX,maxY);
		temp->Losuj22();

		tablica[i] = *temp;
	}
}

Room* Location::GetRoom(Room* bbBox,int length, double X,double Y)
{
	for (int i = 0; i < length; i++)
	{
		if((X >= bbBox[i].Box.X_Left_Bottom) && (X <= bbBox[i].Box.X_Right_Bottom) && (Y >= bbBox[i].Box.Y_Left_Bottom) && (Y <= bbBox[i].Box.Y_Left_Top))
			return &(bbBox[i]);
	}
	return NULL;
}


const char* Location::SendParticle(string *sdiagnostic,Particle *tab)
{
	sdiagnostic->clear();
	for(unsigned int i = 0; i < NumberParticles; i++)
	{
		std::stringstream temp;
		temp << "#" << i << ";" << tab[i].X << ";" << tab[i].Y << ";" << tab[i].Alfa << ";" << tab[i].Probability << ";";
		(*sdiagnostic) = (*sdiagnostic) + temp.str();
	}

	return NULL;
}

void Location::UsunWylosujNoweCzastki2(Particle* ttablicaCzastek,unsigned int iloscCzastekDoUsuniecia)
{
	unsigned int istart = NumberParticles - iloscCzastekDoUsuniecia;
	unsigned int iend = NumberParticles;

	if(iloscCzastekDoUsuniecia > ILOSC_LOSOWANYCH_NOWYCH_CZASTEK)
		iend = NumberParticles - ILOSC_LOSOWANYCH_NOWYCH_CZASTEK;

	//rozmnazamy dobre czastki
	for(unsigned int i = istart, j = 0; i < iend; i++, j++)
		ttablicaCzastek[i].LosujSasiada2(ttablicaCzastek[j].X,ttablicaCzastek[j].Y);

	//nowe czastki
	//for(unsigned int i = end; i < length; i++)
		//tablicaCzastek[i].Losuj22();


/*	iloscCzastekDoUsuniecia = 0;
	for(int i = 0; i < length;i++)
	{
		if(tablicaCzastek[i].sMarkToDelete > GENERATION)
			iloscCzastekDoUsuniecia++;
	}
	if(iloscCzastekDoUsuniecia > 0)
	{
		if(iloscCzastekDoUsuniecia == length)
		{
			iloscCzastekDoUsuniecia--;
			InitTablicaCzastekLosowo(tablicaCzastek,bBox,BoundingBoxCount);
		}
		int zakres = length - iloscCzastekDoUsuniecia;
		//powiel czastki
		for(int i = zakres, j = 0; i < length - ILOSC_LOSOWANYCH_NOWYCH_CZASTEK;i++, j = (j + 1) % zakres)
		{
			tablicaCzastek[i].LosujSasiada(tablicaCzastek[j].X,tablicaCzastek[j].Y,tablicaCzastek[j].Alfa);
		}
		for(int index = length - ILOSC_LOSOWANYCH_NOWYCH_CZASTEK; index < length; index++)
		{
#if DIAGNOSTIC == 1
			if(index < 0)
				printf("UsunWylosujNoweCzastki index,%d",index);
			fflush(NULL);
#endif
			int p = wylosujBB(0,BoundingBoxCount);
			tablicaCzastek[index].LosujPozycje(bBox[p].X_Left_Bottom,bBox[p].X_Right_Bottom,bBox[p].Y_Left_Bottom,bBox[p].Y_Left_Top);
		}
	}*/
}

void Location::UsunWylosujNoweCzastki3(Particle* ttablicaCzastek,unsigned int length,unsigned  int iloscCzastekDoUsuniecia,BoundingBox* bbBox,unsigned int BoundingBoxCount)
{
	iloscCzastekDoUsuniecia = 0;
	for(unsigned int i = 0; i < length;i++)
	{
		if(ttablicaCzastek[i].sMarkToDelete > GENERATION)
			iloscCzastekDoUsuniecia++;
	}
	if(iloscCzastekDoUsuniecia > 0)
	{
		if(iloscCzastekDoUsuniecia == length)
		{
			iloscCzastekDoUsuniecia--;
			InitTablicaCzastekLosowo(ttablicaCzastek,bbBox,BoundingBoxCount);
		}
		int zakres = length - iloscCzastekDoUsuniecia;
		//powiel czastki
		for(unsigned int i = zakres, j = 0; i < length - ILOSC_LOSOWANYCH_NOWYCH_CZASTEK;i++, j = (j + 1) % zakres)
		{
			ttablicaCzastek[i].LosujSasiada(ttablicaCzastek[j].X,ttablicaCzastek[j].Y,ttablicaCzastek[j].Alfa);
		}
		for(unsigned int index = length - ILOSC_LOSOWANYCH_NOWYCH_CZASTEK; index < length; index++)
		{
#if DIAGNOSTIC == 1
			if(index == 0)
				printf("UsunWylosujNoweCzastki index,%d",index);
			fflush(NULL);
#endif
			int p = wylosujBB(BoundingBoxCount);
			ttablicaCzastek[index].LosujPozycje(bbBox[p].X_Left_Bottom,bbBox[p].X_Right_Bottom,bbBox[p].Y_Left_Bottom,bbBox[p].Y_Left_Top);
		}
}
}



void Location::UsunWylosujNoweCzastki4(Particle* ttablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia)
{

	unsigned int doodszczalu =  iloscCzastekDoUsuniecia /= 4;


	for(unsigned int i = (length - doodszczalu), index = 0; i < length - doodszczalu; i++,index++ )
	{
		ttablicaCzastek[i].LosujSasiada(ttablicaCzastek[index].X,ttablicaCzastek[index].Y,ttablicaCzastek[index].Alfa);

	}

	for(unsigned int i = (length - doodszczalu), index = 0; i < length; i++,index++ )
	{
		ttablicaCzastek[i].Losuj22();

	}

}


void Location::UsunWylosujNoweCzastki5(Particle* ttablicaCzastek,unsigned int length)
{
	unsigned int index = length - 1;

	//nowe czastki
	for(unsigned int i = 0; i < ILOSC_LOSOWANYCH_NOWYCH_CZASTEK; i++, index--)
		ttablicaCzastek[index].Losuj22();

	//powielenie
	for(unsigned int i = 0; i < index; i++,index--)
		ttablicaCzastek[index].LosujSasiada(ttablicaCzastek[i].X,ttablicaCzastek[i].Y,ttablicaCzastek[i].Alfa);



}


void Location::UsunWylosujNoweCzastki6(Particle* ttablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia)
{

   if(iloscCzastekDoUsuniecia != length)
	{
	   unsigned int index = length - 1;

	//nowe czastki
	for(unsigned int i = 0; i < ILOSC_LOSOWANYCH_NOWYCH_CZASTEK; i++, index--)
		ttablicaCzastek[index].Losuj22();

	//powielenie
	for(unsigned int i = 0; i < index; i++,index--)
		ttablicaCzastek[index].LosujSasiada(ttablicaCzastek[i].X,ttablicaCzastek[i].Y,ttablicaCzastek[i].Alfa);
	}
	else if(iloscCzastekDoUsuniecia == length)
	{
		ttablicaCzastek[1].LosujSasiada(ttablicaCzastek[0].X,ttablicaCzastek[0].Y,ttablicaCzastek[0].Alfa);

		for(unsigned int i = 2; i< length; i++)
			ttablicaCzastek[i].Losuj22();
	}
}


void Location::UsunWylosujNoweCzastki7(Particle* ttablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia,double wheelTrack, double VL, double Vr,double dt)
{

   if(iloscCzastekDoUsuniecia != length)
	{
	   unsigned int index = length - 1;

	//nowe czastki
	for(unsigned int i = 0; i < ILOSC_LOSOWANYCH_NOWYCH_CZASTEK; i++, index--)
		ttablicaCzastek[index].Losuj22();

	//powielenie
	for(unsigned int i = 0; i < index; i++,index--)
		ttablicaCzastek[index].LosujSasiada3(ttablicaCzastek[i].X,ttablicaCzastek[i].Y,ttablicaCzastek[i].Alfa,wheelTrack, VL,  Vr, dt);
	}
	else if(iloscCzastekDoUsuniecia == length)
	{
		ttablicaCzastek[1].LosujSasiada(ttablicaCzastek[0].X,ttablicaCzastek[0].Y,ttablicaCzastek[0].Alfa);

		for(unsigned int i = 2; i< length; i++)
			ttablicaCzastek[i].Losuj22();
	}
}



int Location::compareMyType (const void * a, const void * b)
{
	if ( (*(Particle*)a).Probability <  (*(Particle*)b).Probability ) return 1;
	else if ( (*(Particle*)a).Probability == (*(Particle*)b).Probability ) return 0;
	else return -1;
}

inline int Location::wylosujBB(int fMax)
{
int f = rand() % fMax;
//f /= RAND_MAX;
return f; // fMin + f * (fMax - fMin);
}

char* Location::getRobotIPAdress()
{
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	 /* I want to get an IPv4 IP address */
	 ifr.ifr_addr.sa_family = AF_INET;

	 /* I want IP address attached to "eth0" */
	 strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

	 ioctl(fd, SIOCGIFADDR, &ifr);

	 close(fd);

	 return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}

