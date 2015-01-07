#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cfloat>
#include <algorithm>

#define TEST 1
#define PROMIEN 0.7

using namespace std;
using std::string;

#define M_PI       3.14159265358979323846
#define M_PI2      6.28318530717958647692
#define ODCHYLENIE 0.3
#define NEW_MIN 0
#define NEW_MAX 1


struct Point
{
	public:
		double X;
		double Y;
};

struct Line
{
public:
	double A;
	double B;
	double C;
};

class Particle
{
private:
	double MinX;
	double MaxX;
	double MinY;
	double MaxY;
	double MaxRadius;
	double rMAX;


	double alfaNew; //zmienna pomocnicza
	double d;



public:	

	double maxX;
		double maxY;

Particle()
{
		Alfa = alfaNew = d = MaxRadius = maxX = maxY = MaxY = MaxX = MinX = MinY = rMAX = X = Y = Probability =  0.0;
		sMarkToDelete = 0;
}

	Particle(double xMax,double yMax)
	{
		Alfa = alfaNew = d = MaxRadius = maxX = maxY = MaxY = MaxX = MinX = MinY = rMAX = X = Y = Probability =  0.0;
		sMarkToDelete = 0;

		maxX = xMax;
		maxY = yMax;
	}

	/*Particle(double dMinX,double dMaxX,double dMinY, double dMaxY,double dRMAX)
	{
		MinX = dMinX;
		MaxX = dMaxX;
		MinY = dMinY;
		MaxY = dMaxY;
		rMAX = dRMAX;
		MaxRadius = 360;

		Losuj2();
	}*/


	Particle(double dMaxX,double dMaxY,double dRMAX)
	{
		MaxX = dMaxX;
		MaxY = dMaxY;
		rMAX = dRMAX;
		MaxRadius = 360;

		Losuj();
	}


	//static const int nPlaces = 2;
	//static const double dbShift = 100; //pow(10.0,nPlaces);

	inline double Round(double dbVal)
	{
	    //return dbVal;
		//return ceil(dbVal * dbShift) / dbShift;

		//int test = (int) (dbVal * 100);
		//return ((double) test) * 0.01;

		double zm = dbVal * 100;
		zm = round(zm);
		zm *= 0.01;
		return zm;
	}

	double X;
	double Y;
	double Alfa;
	//double AlfaStopnie;
	double Probability;

	short sMarkToDelete;

	void Move(double newX,double newY, double newAlfa);


	inline void Losuj() //Generuj czastki ograniczone do wielosci mapy 
	{
		X = ((double)rand() / RAND_MAX) * MaxX; 
		Y = ( (double)rand() / RAND_MAX) * MaxY;
		Alfa = ((double)rand() / RAND_MAX) * 360; //kat ograniczony do 360 stopni
		Probability = 0.0;
		sMarkToDelete = 0;
	}

	inline double fRand(double fMin, double fMax)
	{
	    double f = (double)rand() / RAND_MAX;
	    return fMin + f * (fMax - fMin);
	}

	inline void LosujPozycje(double X1,double X2,double Y1, double Y2)
	{
		X = fRand(X1,X2);
		Y = fRand(Y1,Y2);

		#if TEST == 1
			if(X < 0)
				printf("LosujPozycje X UJEMNE,X1%e,X2%e,Y1%eY2%e",X1,X2,Y1,Y2);
			if(Y < 0)
				printf("LosujPozycje Y UJEMNE,X1%e,X2%e,Y1%eY2%e",X1,X2,Y1,Y2);
			fflush(NULL);
		#endif

		Alfa = fRand(0,2 * M_PI);
		//AlfaStopnie = Alfa * (180 / M_PI);;
		Probability = 0.0;
		sMarkToDelete = 0;
	}

	inline void Losuj2()
	{
		X = abs((MaxX - MinX) * ( (double)rand() / (double)RAND_MAX ) + MinX); 	//(MinX + ((double)  rand()) / MaxX);  //X = (MinX +  (double)rand() / RAND_MAX) * MaxX; 
		Y =  abs((MaxY - MinY) * ( (double)rand() / (double)RAND_MAX ) + MinY); //Y = (MinY + ((double)rand() / MaxY); //Y = (MinY + (double)rand() / RAND_MAX) * MaxY;
		
		Alfa = ((double)rand() / RAND_MAX) * 360; //kat ograniczony do 360 stopni
		Probability = 0.0;
		sMarkToDelete = 0;

	}

	inline void Losuj(double X0,double Y0, double alfa) //Generuj czastke w sasiedztwie innej czastki
	{
		double t = ((double)rand() / RAND_MAX) * 360; //kat ograniczony do 360 stopni
		double R1 =  ((double)rand() / RAND_MAX) * rMAX; //kat ograniczony do 360 stopni 

		X = abs( X0 + R1 * cos(t)); 
		Y = abs(Y0 + R1 * sin(t)); 
		
		if(X > MaxX)
			X = MaxX;

		if(Y > MaxY)
			Y = MaxY;

		Alfa =  alfa; //((double)rand() / RAND_MAX) * 360; //kat ograniczony do 360 stopni
		Probability = 0.0;
		sMarkToDelete = 0;
	}

	//ok
	/*inline void Losuj33(double maxX,double maxY) //Generuj czastke losowa na mapie
	{
			X =  fRand(0,maxX);
			Y = fRand(0,maxY);
			Alfa =  fRand(0,2 * M_PI);
			Probability = 0.0;
			sMarkToDelete = 0;
	}*/

	inline void Losuj22() //Generuj czastke losowa na mapie
		{
				X =  fRand(0,maxX);
				Y = fRand(0,maxY);
				Alfa =  fRand(0,2 * M_PI);
				Probability = 0.0;
				sMarkToDelete = 0;
		}

	inline void Losuj33(double minX,double maxX,double minY,double maxY) //Generuj czastke losowa na mapie
		{
				X =  fRand(minX,maxX);
				Y = fRand(minY,maxY);
				Alfa =  fRand(0,2 * M_PI);
				Probability = 0.0;
				sMarkToDelete = 0;
		}

	inline void LosujSasiada(double X0,double Y0, double alfa) //Generuj czastke w sasiedztwie innej czastki
	{
		double t = fRand(0,2 * M_PI);
		double R1 =  fRand(0,PROMIEN);

		X =  X0 + R1 * cos(t);
		Y = Y0 + R1 * sin(t);

		if(X < 0)
			X *= -1;

		if(Y < 0)
			Y *= -1;

#if DIAGNOSTIC == 1
			if(X < 0)
				printf("LosujPozycje X UJEMNE,%e",X);
			if(Y < 0)
				printf("LosujPozycje Y UJEMNE,%e",Y);
			fflush(NULL);
#endif

		Alfa =  alfa;
		Probability = 0.0;
		sMarkToDelete = 0;
	}


	inline void LosujSasiada5(double X0,double Y0, double alfa) //Generuj czastke w sasiedztwie innej czastki
	{
		double zakres = 0.15;
		double zakras_Alfa = 0.055555556;

		double Xmin = X0 - zakres;
		double Xmax = X0 + zakres;

		double Ymin = Y0 - zakres;
		double Ymax = Y0 + zakres;


		X  = fRand(Xmin,Xmax);
		Y = fRand(Ymin,Ymax);


		Alfa =  fRand(alfa - zakras_Alfa,alfa + zakras_Alfa);


		Probability = 0.0;
		sMarkToDelete = 0;

		/*double t = fRand(0,2 * M_PI);
		double R1 =  fRand(0,PROMIEN);

		X =  X0 + R1 * cos(t);
		Y = Y0 + R1 * sin(t);

		if(X < 0)
			X *= -1;

		if(Y < 0)
			Y *= -1;


		Alfa =  alfa;
		Probability = 0.0;
		sMarkToDelete = 0;*/
	}


	inline void LosujSasiada6(double X0,double Y0, double alfa) //Generuj czastke w sasiedztwie innej czastki zakras alfa od 0 2pi
	{
		double zakres = 0.2;
		double zakras_Alfa = 0.055555556;

		double Xmin = X0 - zakres;
		double Xmax = X0 + zakres;

		double Ymax = Y0 + zakres;

		X  = fRand(Xmin,Xmax);
		Y = fRand(Y0,Ymax);


		Alfa =  fRand(0 ,2 * M_PI);


		Probability = 0.0;
		sMarkToDelete = 0;

		/*double t = fRand(0,2 * M_PI);
		double R1 =  fRand(0,PROMIEN);

		X =  X0 + R1 * cos(t);
		Y = Y0 + R1 * sin(t);

		if(X < 0)
			X *= -1;

		if(Y < 0)
			Y *= -1;


		Alfa =  alfa;
		Probability = 0.0;
		sMarkToDelete = 0;*/
	}

	inline void LosujSasiada7(double X0,double Y0, double alfa,double Vr,double Vl,double dt,double wheelTrack) //Generuj czastke w sasiedztwie innej czastki
		{
		double speed = (Vr + Vl) / 2;
		double losujSpeed = fRand(0,speed);


		double zakras_Alfa = 0.055555556;

		X = X0;
		Y = Y0;
		Alfa =  fRand(alfa - zakras_Alfa,alfa + zakras_Alfa);

		ZaktualizujPrzesuniecie4(wheelTrack,losujSpeed,losujSpeed,dt);

		//double zakres = 0.15;


			//double Xmin = X0 - zakres;
			//double Xmax = X0 + zakres;

		//	double Ymin = Y0 - zakres;
			//double Ymax = Y0 + zakres;


			//X  = fRand(Xmin,Xmax);
			//Y = fRand(Ymin,Ymax);





			Probability = 0.0;
			sMarkToDelete = 0;

			/*double t = fRand(0,2 * M_PI);
			double R1 =  fRand(0,PROMIEN);

			X =  X0 + R1 * cos(t);
			Y = Y0 + R1 * sin(t);

			if(X < 0)
				X *= -1;

			if(Y < 0)
				Y *= -1;


			Alfa =  alfa;
			Probability = 0.0;
			sMarkToDelete = 0;*/
		}

	inline void LosujSasiada2(double X0,double Y0) //Generuj czastke w sasiedztwie innej czastki
	{
		double t = fRand(0,2 * M_PI);
		double R1 =  fRand(0,PROMIEN);

		X =  X0 + R1 * cos(t);
		Y = Y0 + R1 * sin(t);

		if(X < 0)
			X *= -1;

		if(Y < 0)
			Y *= -1;

#if DIAGNOSTIC == 1
			if(X < 0)
				printf("LosujPozycje X UJEMNE,%e",X);
			if(Y < 0)
				printf("LosujPozycje Y UJEMNE,%e",Y);
			fflush(NULL);
#endif

		Alfa =  fRand(0,2 * M_PI);
		Probability = 0.0;
		sMarkToDelete = 0;
	}


	inline void LosujSasiada3(double X0,double Y0, double alfa,double wheelTrack,double Vl,double Vr,double dt) //Generuj czastke w sasiedztwie innej czastki
	{
		this->X = X0;
		this->Y = Y0;
		this->Alfa = alfa;

		ZaktualizujPrzesuniecie4(wheelTrack,Vl,Vr,dt);

		//double t = fRand(0,2 * M_PI);
		//double R1 =  fRand(0,PROMIEN);

		//X =  X0 + R1 * cos(t);
	//	Y = Y0 + R1 * sin(t);

		if(X < 0)
			X *= -1;

		if(Y < 0)
			Y *= -1;

#if DIAGNOSTIC == 1
			if(X < 0)
				printf("LosujPozycje X UJEMNE,%e",X);
			if(Y < 0)
				printf("LosujPozycje Y UJEMNE,%e",Y);
			fflush(NULL);
#endif

		//Alfa =  alfa;
		Probability = 0.0;
		sMarkToDelete = 0;
	}


	/*inline Point getIntersection(double A,double B,double C,double X0,double Y0,double Alfa)
	{
		Point p;
		double A0 = tan(Alfa);
		double B1 = -1;
		double C1 = Y0 - (A0 * X0);

		double W;

		W =


	}*/

	inline bool canCountDistanceToWall (double Alfa_Czastki,double YCzastki,double YPrzeciecia)
	{
		bool result = false;
		double y = YPrzeciecia - YCzastki;

		if((Alfa_Czastki >= 0) && (Alfa_Czastki <= M_PI))
		{
			if(y >= 0)
				result = true;
		}
		else
		{
			if(y <= 0)
				result = true;
		}

		return result;
	}

inline void getLine(double x,double y,double alfa,double *A,double *B,double *C)
{
	//double a2 = - tan( alfa);
		//double b2 = 1;
	 	//double c2 =  Y2 + (a2 * X2);

	/*(*A) = - tan(alfa);
	(*B) = 1;
	(*C) = (Y + ((*A) * X));
*/
	(*A) = tan(alfa);
	(*A) = Round(*A);
	//(*A) = round((*A) * 10000) / 10000;

	(*B) = -1;

	(*C) = (y - ((*A) * x));
	(*C) = Round((*C));
	//(*C) = round((*C) * 10000) / 10000;

}

inline Line getLine(double x,double y,double alfa)
{
	Line temp;

	temp.A = Round(tan(alfa));
	temp.B = -1;
	temp.C =Round((y - (temp.A * x)));

	return temp;
}


inline Point calculateIntersection(MazeWall *wall,double alfa,double X2,double Y2)
{
	Point temp;

	double W;
	double Wx;
	double Wy;
	double a1 = wall->A;
	double b1 = wall->B;
	double c1 = wall->C;

	double a2,b2,c2;

	getLine(X2,Y2,alfa,&a2,&b2,&c2);

	W = a1 * b2 - b1 * a2;

	if(W != 0)
	{
		Wx =  b1 * c2 - c1 * b2;
		Wy = c1 * a2 - a1 * c2;

		temp.X = Wx / W;
		temp.X = Round(temp.X);

		temp.Y = Wy / W;
		temp.Y = Round(temp.Y);
	}
	else
		temp.X = temp.Y = -1;

	return temp;
}

inline double calculateDistnace(double X1,double Y1,double X2,double Y2)
{
	return sqrt(pow(X1 - X2 ,2) + pow(Y1 - Y2,2));
}

inline double getDistnace(MazeWall *wall,double alfa,double X2,double Y2)
{	
	double dist = -1.0;

	double W;
	double Wx;
	double Wy;
	double x;
	double y;
	double a1 = wall->A;
	double b1 = wall->B;
	double c1 = wall->C;

	double a2,b2,c2;
	//double prawie_zero;

	getLine(X2,Y2,alfa,&a2,&b2,&c2);


	/*
	prawie_zero = (X2 * a2 + Y2 * b2 + c2);

	if( (prawie_zero) >  0.0001)
		prawie_zero = 0.0;


	double prawie_zeroDlasciany = (a1 * wall->From_X + b1 * wall->From_Y + c1);

	if(prawie_zeroDlasciany > 0.0001)
		prawie_zeroDlasciany = 0.0;

	prawie_zeroDlasciany = (a1 * wall->To_X + b1 * wall->To_Y + c1);

	if(prawie_zeroDlasciany > 0.0001)
			prawie_zeroDlasciany = 0.0;

*/

	W = a1 * b2 - b1 * a2;
	
	if(W != 0)
	{
		Wx =  b1 * c2 - c1 * b2;
		Wy = c1 * a2 - a1 * c2;

		x = Wx / W;
		y = Wy / W;

		x = Round(x);
		y = Round(y);
		//X = round(X * 100) / 100;
		//Y = round(Y * 100) / 100;

		if(canCountDistanceToWall(alfa,Y2,y))
		{
			//if((wall->From_X <= X) && (X <= wall->To_X) && (wall->From_Y <= Y) && (Y <= wall->To_Y))
				//dist = sqrt(pow(X - X2 ,2) + pow( Y - Y2,2)); //wartosc oczekiwan*/

			if((wall->From_X == wall->To_X) && (wall->From_Y <= y) && (y <= wall->To_Y))
				dist = sqrt(pow(x - X2 ,2) + pow( y - Y2,2)); //wartosc oczekiwan*/
			else if((wall->From_Y == wall->To_Y) && (wall->From_X <= x) && (x <= wall->To_X))
				dist = sqrt(pow(x - X2 ,2) + pow( y - Y2,2)); //wartosc oczekiwan*/
			else if((wall->From_X <= x) && (x <= wall->To_X) && (wall->From_Y <= y) && (y <= wall->To_Y))
				dist = sqrt(pow(x - X2 ,2) + pow( y - Y2,2)); //wartosc oczekiwan*/

			/*if((wall->From_Y == wall->To_Y) && (wall->From_X <= X) && (X <= wall->To_X))
				dist = sqrt(pow(X - X2 ,2) + pow( Y - Y2,2)); //wartosc oczekiwana
			else if((wall->From_X == X <= wall->To_X) && (wall->From_Y <= Y) && (Y <= wall->To_Y))
				dist = sqrt(pow(X - X2 ,2) + pow( Y - Y2,2)); //wartosc oczekiwan*/

		}

//		if(dist > -1)
	//	printf("Sciana: %s,Sciana Start_X: %fSciana Start_Y: %f,Sciana End_X: %f,Sciana EndY: %f  Przecicie X: %f Y: %fDist: %f\n",wall->Id.c_str(),wall->From_X,wall->From_Y,wall->To_X,wall->To_Y, X,Y,dist);
	//				fflush(NULL);
	}

	//printf("Sciana: %s,Sciana Start_X: %fSciana Start_Y: %f,Sciana End_X: %f,Sciana EndY: %f  Przecicie X: %f Y: %fDist: %f\n",wall->Id.c_str(),wall->From_X,wall->From_Y,wall->To_X,wall->To_Y, X,Y,dist);
		//		fflush(NULL);

	return dist;
}

		inline void ZaktualizujPrzesuniecie4(double wheelTrack,double Vl, double  Vr, double dt)
		{
			if(Vl != Vr)
			{
				alfaNew = (((Vr - Vl) * dt) / wheelTrack);

				X += (wheelTrack * (Vr + Vl)) / (2*(Vr - Vl)) * (sin(alfaNew + Alfa) - sin(Alfa));
				Y -= (wheelTrack * (Vr + Vl)) / (2*(Vr - Vl)) * (cos(alfaNew + Alfa) - cos(Alfa));

				Alfa += alfaNew;

			}
			else
			{
				d = ((Vr + Vl) / 2) * dt;

				X += d * cos(Alfa);
				Y += d * sin(Alfa);
			}
		}

		inline double Normalize(double value,double min,double max) //normalizacja min max
		{
			if(max > 0)
				return (((value - min) * (NEW_MAX - NEW_MIN)) / (max - min)) + NEW_MIN;
			else
				return 0;
		}

		inline bool canCountDistance1 (MazeWall *wall,double X2,double Y2, double alfaMin, double alfaMax)
		{
			double aMin = - tan( (alfaMin * M_PI) / 180.0);
			double bMin = 1;
			double cMin =  Y2 + (aMin * X2);

			double aMax = - tan( (alfaMax * M_PI) / 180.0);
			double bMax = 1;
			double cMax = -( Y2 + (aMax * X2));

			double ResultAmin = aMin * wall->From_X + bMin * wall->From_Y + cMin;
			double ResultAmax = aMax * wall->From_X + bMax * wall->From_Y + cMax;

			if((ResultAmin < 0) && (ResultAmax < 0))
				return false;

			ResultAmin = aMin * wall->To_X + bMin * wall->To_Y + cMin;
		    ResultAmax = aMax * wall->To_X + bMax * wall->To_Y + cMax;

		    if((ResultAmin < 0) && (ResultAmax < 0))
		    	return false;

		    return true;
		}

		inline bool canCountDistance (double X2,double Y2,double alfaCzastki,double X_przeciecia,double Y_przeciecia, double alfaSkanu)
		{
			//srodek robota robota
			double aCz = - tan( (alfaCzastki * M_PI) / 180.0);
			double bCz = 1;
			double cCz =  Y2 + (aCz * X2);

			bool result = false;

			double wynik = aCz * X_przeciecia + bCz * Y_przeciecia + cCz;


			if(alfaSkanu > alfaCzastki)
			{
				if(wynik > 0)
					result = true;
			}
			else
			{
				if(wynik < 0)
					result = true;

			}
				   return result;
		}


		inline void UpdateCountProbability3(Room* box,int scanTable[],double angleTable[],int length)
		{
			double dist;
			double gauss;
			double sumProbability = 0.0;
			Probability = 0.0;
			int iloscScian = box->ContainerWallCount();
			//double norm;

#if DIAGNOSTIC == 1
			double tablicaOdleglosci[length];
			double tablicaKatow[length];
			double tablicaSkan[length];
			double tablicaGauss[length];
			std::string tablicaScien[length];
#endif
			//int www = 0;
			int ilosc_pomiarow_uzytych_do_wyliczenia_prawdopdobienstwa = 0;

			for (int i = 0; i < length; i++)
			{
				int test = 0;
				test++;

#if DIAGNOSTIC == 1
				tablicaOdleglosci[i] = -1;
				tablicaKatow[i] = this->Alfa + angleTable[i];
				tablicaSkan[i] = (((double) scanTable[i]) / 1000);
				tablicaGauss[i] = -1;
#endif

				for (int j = 0; j < iloscScian; j++)
				{

						dist =  getDistnace(&box->ContainerWallTable[j],this->Alfa + angleTable[i],this->X,this->Y); //wartosc oczekiwana

					if(dist > 0)
					{



						double scan = (((double) scanTable[i]) / 1000);
						gauss =  Gauss2(dist,scan); //exp((-1 * pow(scanTable[i] - dist,2)) / ( 2 * ODCHYLENIE * ODCHYLENIE)) / (2 * M_PI * ODCHYLENIE);

#if DIAGNOSTIC == 1
						tablicaOdleglosci[i] = dist;
						tablicaKatow[i] = angleTable[i];
						tablicaSkan[i] = scan;
						tablicaGauss[i] = gauss;

						tablicaScien[i] = (&box->ContainerWallTable[j])->Id;
#endif

						ilosc_pomiarow_uzytych_do_wyliczenia_prawdopdobienstwa++;

						//www++;


						//Normalize(gauss,0,Gauss2(10,10)); //gauss;


					sumProbability += gauss;
					}
				}
			}

#if DIAGNOSTIC == 1
			for(int i = 0; i < length;i++)
			{
				printf("ID: %d Obliczone: %f Kat: %f Skan: %f Gaus: %fSciana: %s\n",i, tablicaOdleglosci[i],tablicaKatow[i],tablicaSkan[i],tablicaGauss[i],tablicaScien[i].c_str());
			}
#endif

			fflush(NULL);

			double yy = ((double) ilosc_pomiarow_uzytych_do_wyliczenia_prawdopdobienstwa) * Gauss2(1,1);
			Probability =  Normalize(sumProbability,0,yy);  //(sumProbability / index);  /// (length / PRZLIECZENIE_DLA_POMIARU_SKANERA)); //sumProbability
		}


		inline double radianNaStpnie(double radian)
		{
			return  radian * 180 /M_PI;
		}

		//inline bool isIntersectionOK(Line lineStart, Line lineEnd,double alfa,Point intersection,MazeWall *wall)
		inline bool isIntersectionOK(MazeWall *wall,double alfa,Point intersection)
		{
			bool result = false;

			if((intersection.X >= 0) && (intersection.Y >= 0))
			{
				double y = Round(intersection.Y - this->Y);

				if((alfa >= 0) && (alfa <= M_PI))
				{
					if(y >= 0)
					{
							if((wall->From_X == wall->To_X) && (wall->From_Y <= intersection.Y) && (intersection.Y <= wall->To_Y))
									result = true;
							else if((wall->From_Y == wall->To_Y) && (wall->From_X <= intersection.X) && (intersection.X <= wall->To_X))
									result = true;
							else if((wall->From_X <= intersection.X) && (intersection.X <= wall->To_X) && (wall->From_Y <= intersection.Y) && (intersection.Y <= wall->To_Y))
									result = true;
					}
				}
				else if((alfa > M_PI) && (alfa < M_PI2))
					{
						if(y <= 0)
						{
							if((wall->From_X == wall->To_X) && (wall->From_Y <= intersection.Y) && (intersection.Y <= wall->To_Y))
									result = true;
							else if((wall->From_Y == wall->To_Y) && (wall->From_X <= intersection.X) && (intersection.X <= wall->To_X))
									result = true;
							else if((wall->From_X <= intersection.X) && (intersection.X <= wall->To_X) && (wall->From_Y <= intersection.Y) && (intersection.Y <= wall->To_Y))
									result = true;
						}
					}
					else
					{
						int bug = 22;
						bug++;
					}
			}

			return result;



			/*double tmplineStar = lineStart.A * intersection.X + lineStart.B * intersection.Y + lineStart.C;
			double tmplineEnd = lineEnd.A * intersection.X + lineEnd.B * intersection.Y + lineEnd.C;

			if((alfa >= 0) && (alfa < M_PI))
			{
				if(!((tmplineStar >= 0) && (tmplineEnd <= 0)))
				{
					if((wall->From_X == wall->To_X) && (wall->From_Y <= Y) && (Y <= wall->To_Y))
						result = true;
					else if((wall->From_Y == wall->To_Y) && (wall->From_X <= X) && (X <= wall->To_X))
						result = true;
					else if((wall->From_X <= X) && (X <= wall->To_X) && (wall->From_Y <= Y) && (Y <= wall->To_Y))
						result = true;
				}
			}
			else
			{
				if(!((tmplineStar <= 0) && (tmplineEnd >= 0)))
				{
					if((wall->From_X == wall->To_X) && (wall->From_Y <= Y) && (Y <= wall->To_Y))
						result = true;
					else if((wall->From_Y == wall->To_Y) && (wall->From_X <= X) && (X <= wall->To_X))
						result = true;
					else if((wall->From_X <= X) && (X <= wall->To_X) && (wall->From_Y <= Y) && (Y <= wall->To_Y))
						result = true;
				}
			}*/

			//return result;
		}

		inline void UpdateCountProbability5(Room* box,int scanTable[],double angleTable[],int length)
			{
			Point intersection;

			double dist;
					double gauss;
					double sumProbability = 0.0;
					Probability = 0.0;
					int iloscScian = box->ContainerWallCount();
					//double norm;

#if DIAGNOSTIC == 1
					double tablicaOdleglosci[length];
					double tablicaKatow[length];
					double tablicaSkan[length];
					double tablicaGauss[length];
					std::string tablicaScien[length];

					MazeWall * wall;
#endif

					int ilosc_pomiarow_uzytych_do_wyliczenia_prawdopdobienstwa = 0;

					for (int i = 0; i < length; i++)
					{
						int test = 0;
						test++;

#if DIAGNOSTIC == 1
						tablicaOdleglosci[i] = -1;
						tablicaKatow[i] = this->Alfa + angleTable[i];
						tablicaSkan[i] = (((double) scanTable[i]) / 1000);
						tablicaGauss[i] = -1;
#endif
						dist = DBL_MAX;

						double dist2;
						double tempAlfa;

						tempAlfa = this->Alfa + angleTable[i];

						if(tempAlfa < 0)
							tempAlfa += M_PI2;
						else if(tempAlfa > M_PI2)
							tempAlfa -= M_PI2;

						tempAlfa = Round(tempAlfa);

						for (int j = 0; j < iloscScian; j++)
						{
							 intersection = calculateIntersection(&box->ContainerWallTable[j],tempAlfa,this->X,this->Y);

							 if(isIntersectionOK(&box->ContainerWallTable[j],tempAlfa,intersection))
								 dist2 = calculateDistnace(this->X,this->Y,intersection.X,intersection.Y);
							 else
								 dist2 = -1;

							if(dist2 > -1)
							{
								dist = min(dist2,dist);

								#if DIAGNOSTIC == 1
									if(min(dist2,dist) == dist2)
										tablicaScien[i] = (&box->ContainerWallTable[j])->Id;

								#endif
							}

#if DIAGNOSTIC == 1
							 wall = &box->ContainerWallTable[j];
#endif
							//printf("Sciana: %s,Sciana Start_X: %fSciana Start_Y: %f,Sciana End_X: %f,Sciana EndY: %f  Przecicie X: %f Y: %fDist: %f\n",wall->Id.c_str(),wall->From_X,wall->From_Y,wall->To_X,wall->To_Y, intersection.X,intersection.Y,dist2);
							//fflush(NULL);
						}

						//printf("End\n");

						if(dist > 0)
						{
												double scan = (((double) scanTable[i]) / 1000);


												if((scan < 0.05f) && (dist > 5.6f))  //zakres pomieru skenera od 20mm do 5600mm. gdy pomier wyszedl poza zakres i obliczenia rowniez uznajemy pomira z ok
													gauss =  Gauss2(scan,scan);
												else
													gauss =  Gauss2(dist,scan);

#if DIAGNOSTIC == 1
												tablicaOdleglosci[i] = dist;
												tablicaKatow[i] = angleTable[i];
												tablicaSkan[i] = scan;
												tablicaGauss[i] = gauss;
#endif

												ilosc_pomiarow_uzytych_do_wyliczenia_prawdopdobienstwa++;
											sumProbability += gauss;
						}

						//int www;
				//		www= 112;
					}


					/*for(int i = 0; i < length;i++)
					{
						printf("ID: %d Obliczone: %f Kat: %f Skan: %f Gaus: %fSciana: %s\n",i, tablicaOdleglosci[i],tablicaKatow[i],tablicaSkan[i],tablicaGauss[i],tablicaScien[i].c_str());

						fflush(NULL);
					}*/



					double yy = ((double) ilosc_pomiarow_uzytych_do_wyliczenia_prawdopdobienstwa) * Gauss2(1,1);
					Probability =  Normalize(sumProbability,0,yy);  //(sumProbability / index);  /// (length / PRZLIECZENIE_DLA_POMIARU_SKANERA)); //sumProbability
				}


		inline void UpdateCountProbability4(Room* box,int scanTable[],double angleTable[],int length)
		{
				double dist;
				double gauss;
				double sumProbability = 0.0;
				Probability = 0.0;
				int iloscScian = box->ContainerWallCount();
				//double norm;

#if DIAGNOSTIC == 1
				double tablicaOdleglosci[length];
				double tablicaKatow[length];
				double tablicaSkan[length];
				double tablicaGauss[length];
				std::string tablicaScien[length];
#endif
				//int www = 0;
				int ilosc_pomiarow_uzytych_do_wyliczenia_prawdopdobienstwa = 0;

				for (int i = 0; i < length; i++)
				{
					int test = 0;
					test++;
#if DIAGNOSTIC == 1
					tablicaOdleglosci[i] = -1;
					tablicaKatow[i] = this->Alfa + angleTable[i];
					tablicaSkan[i] = (((double) scanTable[i]) / 1000);
					tablicaGauss[i] = -1;
#endif

					dist = DBL_MAX;

					double dist2;

					//printf("ID %d,Kat %f KatStopnie:%f\t",i,tablicaKatow[i],radianNaStpnie(tablicaKatow[i]));

					//printf("Start\n");

					for (int j = 0; j < iloscScian; j++)
					{

						//printf("%s\n",(&box->ContainerWallTable[j])->Id.c_str());

						dist2 = getDistnace(&box->ContainerWallTable[j],this->Alfa + angleTable[i],this->X,this->Y);

						if(dist2 > -1)
						{
							dist = min(dist2,dist);

							#if DIAGNOSTIC == 1
								if(min(dist2,dist) == dist2)
									tablicaScien[i] = (&box->ContainerWallTable[j])->Id;

							#endif
						}
					}

					//printf("End\n");

					if(dist > 0)
					{
											double scan = (((double) scanTable[i]) / 1000);
											gauss =  Gauss2(dist,scan);

#if DIAGNOSTIC == 1
											tablicaOdleglosci[i] = dist;
											tablicaKatow[i] = angleTable[i];
											tablicaSkan[i] = scan;
											tablicaGauss[i] = gauss;
#endif

											ilosc_pomiarow_uzytych_do_wyliczenia_prawdopdobienstwa++;
										sumProbability += gauss;
					}

					//int www;
					//www= 112;
				}

#if DIAGNOSTIC == 1
				for(int i = 0; i < length;i++)
				{
					printf("ID: %d Obliczone: %f Kat: %f Skan: %f Gaus: %fSciana: %s\n",i, tablicaOdleglosci[i],tablicaKatow[i],tablicaSkan[i],tablicaGauss[i],tablicaScien[i].c_str());

					fflush(NULL);
				}
#endif


				double yy = ((double) ilosc_pomiarow_uzytych_do_wyliczenia_prawdopdobienstwa) * Gauss2(1,1);
				Probability =  Normalize(sumProbability,0,yy);  //(sumProbability / index);  /// (length / PRZLIECZENIE_DLA_POMIARU_SKANERA)); //sumProbability
			}


	inline double Gauss2(double prop,double real)
	{
	double a,b,c,dd;
	double x = prop;
	double delta = sqrt(ODCHYLENIE);
	double ni = real;

	a = 1 / (delta  *sqrt(2 * M_PI));
	b = ni;
	c = delta;
	dd = 0;

	return (a * exp( pow(x - b,2) / (-2 * pow(c,2)))) + dd;
	}
};



/*
	inline void UpdateCountProbability(Room* box,int scanTable[],double angleTable[],int length)
	{
		double dist;
		double gauss;
		double sumProbability = 0.0;
		int iloscScian = box->ContainerWallCount();

		for (int i = 0; i < length; i = i + PRZLIECZENIE_DLA_POMIARU_SKANERA)
		{
			int test = 0;
			test++;

			for (int j = 0; j < iloscScian; j++)
			{
				dist =  getDistnace(&box->ContainerWallTable[j],angleTable[i],this->X,this->Y); //wartosc oczekiwana

				if(dist > 0)
				{
					double scan = (((double) scanTable[i]) / 1000);
				gauss =  Gauss2(scan,dist); //exp((-1 * pow(scanTable[i] - dist,2)) / ( 2 * ODCHYLENIE * ODCHYLENIE)) / (2 * M_PI * ODCHYLENIE);
				
				sumProbability +=  gauss;//Normalize(gauss,0,dist); 
				}
			}
		}

		Probability = sumProbability;
	}
	*/
		
/*
	inline void UpdateCountProbability1(Room* box,int scanTable[],double angleTable[],int length)
	{
		double dist;
		double gauss;
		double sumProbability = 0.0;
		int iloscScian = box->ContainerWallCount();

		for (int i = 0; i < length; i = i + PRZLIECZENIE_DLA_POMIARU_SKANERA)
		{
			printf("ID: %d Kat: %f Odleglosc: %d\n",i,angleTable[i],scanTable[i]);

			int test = 0;
			test++;

			for (int j = 3; j < iloscScian; )
			{
				double scan = 0;
				dist =  getDistnace(&box->ContainerWallTable[j],angleTable[i],this->X,this->Y); //wartosc oczekiwana

				if(dist > 0)
				{
				scan = (((double) scanTable[i]) / 1000);
				gauss =  Gauss2(scan,dist); //exp((-1 * pow(scanTable[i] - dist,2)) / ( 2 * ODCHYLENIE * ODCHYLENIE)) / (2 * M_PI * ODCHYLENIE);

				sumProbability +=  gauss;//Normalize(gauss,0,dist);
				}


				for(int w = 0; w < iloscScian; w++)
				{
					printf("X %f, Y %f\n",(&box->ContainerWallTable[w])->From_X, (&box->ContainerWallTable[w])->From_Y);
					fflush(NULL);
				}*/


				/*if((&box->ContainerWallTable[j])->From_X == (&box->ContainerWallTable[j])->To_X)
				{
					int eee;

					eee = 00;

					;

				}

			}
		}

		Probability = sumProbability;
	}
*/


		
		/*
		double sumProbability = 0.0;
		double X;
		double Y;
		double dist;
		double alfa2;
		double b2;
		double gauss;

		double W;
		double Wx;
		double Wy;


		if(box != NULL)
		{
			for(int j = 0; j < box->ContainerWall.size(); j++)
			{
		for (int i = 0; i < length; i++)
		{
		alfa2 = tan(angleTable[i]);
			b2 = this->Y - (alfa2 * this->Y);*/

		//	X = (b2 - box->ContainerWall[j].B) / (box->ContainerWall[j].A - alfa2);			
		//	Y = ((box->ContainerWall[j].A  * b2) - (alfa2 * box->ContainerWall[j].B)) / (box->ContainerWall[j].A - alfa2);

		/*	W = alfa2 * box->ContainerWall[j].B + box->ContainerWall[j].A;
			Wx = b2 * box->ContainerWall[j].B + box->ContainerWall[j].C;
			Wy = alfa2 * box->ContainerWall[j].C - box->ContainerWall[j].A * b2;


			if(W == 0)
				continue;
			else*/

			/*{

			X = Wx/W;
			Y = Wy/W;*/


			/*if((box->ContainerWall[j].From_X <= X <= box->ContainerWall[j].To_X) && (box->ContainerWall[j].From_Y <= Y <= box->ContainerWall[j].To_Y))
			{
				dist =  getDistnace(&box->ContainerWall[j],angleTable[i],this->X,this->Y); //wartosc oczekiwana
					
					
					//sqrt(( X - this->X ) * ( X - this->X ) + ( Y - this->Y ) * ( Y - this->Y )); //wartosc oczekiwana
				gauss =  exp((-1 * pow(scanTable[i] - dist,2)) / ( 2 * ODCHYLENIE * ODCHYLENIE)) / (2 * M_PI * ODCHYLENIE);  
				
				sumProbability +=  Normalize(gauss,0,dist); 
			}
			else
				continue;
			}
		}
			}
		}
		Probability = sumProbability;

		
	}*/



	//inline double Distance(Room* box)
	//{


	//}

/*	inline void ZaktualizujPrzesuniecie2(double wheelTrack,int frontRightSpeed, int  rearRightSpeed, int frontLeftSpeed, int rearLeftSpeed ,double time)
	{
		double Vr = ((double) (frontRightSpeed +  rearRightSpeed)) / 2000 ;
	   double Vl = ((double)(frontLeftSpeed + rearLeftSpeed)) / 2000;

			double sr = Vr * time;
			double sl = Vl * time;

			double s = (sr + sl) / 2;

			double alfa = ((sr - sl) / wheelTrack);
			alfa += this->Alfa;


			this->X = s * cos((alfa * M_PI) / 180) + this->X;
		    this->Y = s * sin((alfa * M_PI) / 180) + this->Y;

		    Alfa = alfa;

			printf("X%f\nY%f",X,Y);
		    fflush(NULL);



	}

	inline double KatLuku(double l,double r)
	{
		return ((360 * l) / ( 2 * M_PI * r));
	}


	inline void ZaktualizujPrzesuniecie3(double wheelTrack,double frontRightSpeed, double  rearRightSpeed, double frontLeftSpeed, double rearLeftSpeed ,double time)
	{
		double Vr = ((double) (frontRightSpeed +  rearRightSpeed)) / 2 ;
	   double Vl = ((double)(frontLeftSpeed + rearLeftSpeed)) / 2;
	   double s;

	   if(Vr == Vl) //ok
	   {
		   s = ((Vr + Vl) / 2) * time; //droga w mm
		   s = s / 1000; //droga w m

		   X += s * cos((Alfa * M_PI) / 180 ); // A'= [x + Sx, ...]
		   Y += s * cos(((90 - Alfa) * M_PI) / 180); // A' [... ,y + Sy]
	   }
	   else if((Vr != 0) && (Vl == 0))
	   {
		   double r = wheelTrack;
		   double sr =  Vr * time; // droga ww mm
		   sr = sr / 1000; // droga w m
		   	double kat =   KatLuku(sr,r);

		    X += sr * cos(((Alfa + kat) * M_PI) / 180 ); // A'= [x + Sx, ...]
		 	Y += sr * cos(((90 - (Alfa + kat)) * M_PI) / 180); // A' [... ,y + Sy]

		 	Alfa += kat;

		 	if(Alfa > 360)
 		 		Alfa -= 360;

	   }
	   else if((Vr == 0) && (Vl != 0))
	   {
		   double r = wheelTrack;
		 		   double sl =  Vl * time; // droga ww mm
		 		   sl = sl / 1000; // droga w m
		 		   	double kat =   KatLuku(-sl,r);

		 		    X += sl * cos(((Alfa + kat) * M_PI) / 180 ); // A'= [x + Sx, ...]
		 		 	Y += sl * cos(((90 - (Alfa + kat)) * M_PI) / 180); // A' [... ,y + Sy]
		 		 	Alfa += kat;

		 		 	if(Alfa > 360)
		 		 		Alfa -= 360;
	   }
	   else
	   {
		   double sr =  Vr * time; // droga ww mm
		    sr = sr / 1000; // droga w m
		  double sl =  Vl * time; // droga ww mm
		 	sl = sl / 1000; // droga w m


			 double x;


		 	if(Vl > Vr)
		 	{
		 		x = (sr * wheelTrack) / (sl - sr);
		 	}
		 	else
		 	{
		 		x = (sl * wheelTrack) / (sr - sl);
		 	}

		 	  double r = x + wheelTrack;
		 	double s = (sr + sl) / 2;



		 		double kat =   KatLuku(s,r);

		    X += s * cos(((Alfa + kat) * M_PI) / 180 ); // A'= [x + Sx, ...]
		 	Y += s * cos(((90 - (Alfa + kat)) * M_PI) / 180); // A' [... ,y + Sy]

		 	Alfa += kat;

		 	if(Alfa > 360)
 		 		Alfa -= 360;
	   }

	} */

/*	inline void ZaktualizujPrzesuniecie(double V,double alfa,double dt) //liczy droge i aktualizuje przemieszczenie
	{
		double s = V * dt;

		ZaktualizujPrzesuniecie(s,alfa);
	}*/
	
	/*inline void ZaktualizujPrzesuniecie(double s,double  alfa)
	{
			X += s * cos((alfa * M_PI) / 180 ); // A'= [x + Sx, ...]
			Y += s * cos(((90 - alfa) * M_PI) / 180); // A' [... ,y + Sy]

			Alfa += alfa;
	}*/


//	inline void ObrotCzastkiKat(double alfaNew)
//	{
//		X = X * cos((alfaNew * M_PI) / 180) - Y * sin((alfaNew * M_PI) / 180);
	//	Y = X * sin((alfaNew * M_PI) / 180) + Y * cos((alfaNew * M_PI) / 180);

	//	Alfa += alfaNew;

//	}


	//const char* TransmitParticle(int index)
	//{
	//	stringstream temp; 
	//	string tmp = "rrrrrrwwww";
	//	//const char yy;
	//	
	//	temp << "#" << index << ";" << X << ";" << Y << ";" << Alfa << ";" << Probability << ";" << '\0';	
	//	//tmp = temp.str();

	//	//return tmp.c_str();
	//	return tmp.c_str();
	//}

