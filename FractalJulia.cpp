/* 
g++ -Wall opencv_exempleCPP.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -std=c++11 
*/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <complex>
#include <time.h>
#include <fstream>
using namespace std;
using namespace cv;
Mat image;

 
 #define V 4
 #define ITER 250
//nombre de thread
#define Nb_Thread 10
// mutex pour protéger VALEUR
pthread_mutex_t mutex; 

// taille de l'image
 #define HAUT 800
 #define LARG 800



//declaration des parametres de la couleur
void HSVtoRGB( unsigned char  *r, unsigned char  *g, unsigned char  *b, unsigned char  h, unsigned char  s, unsigned char  v );
// valeur reel et imaginaire de la fracttale

std::complex<long double> c( 0.0285,0.013);

long double reel=0.285,imaginaire=0.01;
double X = 0;
double Y = 0;











// déclarer la fonction pour la creation de la fractal
void *CalculFractaleJulia(void *arg);

int main( int argc, char** argv )
{
double temps;
 clock_t t1, t2;
 
 
pthread_t thread_f[Nb_Thread];
  image=cv::Mat(LARG, HAUT, CV_8UC3);
    
     
refresh: 
    
for(int i=0;i< Nb_Thread;i++){
t1 = clock();   
 pthread_create (&thread_f[i], NULL, CalculFractaleJulia, (void*)NULL);
 t2 = clock();
     temps = ((double)(t2-t1)*1000/CLOCKS_PER_SEC);
     
 }
 cout<<"le temps d'execution "<< temps <<"sec "<<endl;
  
ofstream fichier("resultat.dat", ios::app);
 
        if(fichier)
        {
           
                //fichier << "le temps " << duree<< endl;
                fichier<< " #nombres de threads            #temps d'execution en sec\n"<< "                    "<<Nb_Thread<<"            "<< temps << endl;
                fichier.close();
        }
        else
              {
                cout << "Impossible d'ouvrir le fichier !" << endl;
              }   
    //Gestion des entrées
  while(char key = cvWaitKey(66)) {
    switch(key){
      case 'a':
      

// Modification de la constante de la fractale 
      reel = reel + 0.1;
      goto refresh;
      break;
      case 'z':
      
      reel = reel - 0.1;
      goto refresh;
      break;
      case 'o':
      
      imaginaire = imaginaire + 0.1;
      goto refresh;
      break;
      case 'p':
      
      imaginaire = imaginaire - 0.05;
      goto refresh;
      break;

//Utilisation de Zoom/Dezoom 
      case 'd':
      
      X -= 100;
      Y -= 100;
      goto refresh;
      break;
      case 's':
     
      X += 100;
      Y += 100;
      goto refresh;
      break;

      default:
      break;
    }
    if (key == 'q')
    break;
    imshow("fractale image",image ); 
  }
// ferme la fenêtre
  cvDestroyWindow("fractale image"); 
  return 0;
}









// fractale

void *CalculFractaleJulia(void *arg){
std::complex<long double> c (reel,imaginaire);
  for(int x=0;x<LARG;x++){
      for(int y=0;y<HAUT;y++){
        long double newX=((long double)((long double)x-((long double)((LARG)-X)/2))/((long double)((LARG)-X)/2));
        long double newY=((long double)((long double)y-((long double)((HAUT)-Y)/2))/((long double)((HAUT)-Y)/2));
        std::complex<long double> point(newY,newX);
        int iteration=0;
        long double norm = std::norm(point);
        while(iteration<ITER && norm<V){
          point=(point*point)+c;
          norm = std::norm(point);
          iteration++;
        }
        if(iteration==ITER && norm<V){
          image.at<Vec3b>(x,y)[0]=0; 
          image.at<Vec3b>(x,y)[1]=0; 
          image.at<Vec3b>(x,y)[2]=0;
        }
        else{
          int couleur=(int)((float)iteration/(float)ITER*255);
unsigned char brillance=150; 
          unsigned char saturation=150; 
          unsigned char r,g,b;
// definition de la couleur de l'image
	  HSVtoRGB( & r, & g, & b, couleur, brillance, saturation );
          image.at<Vec3b>(x,y)[0]=r;
          image.at<Vec3b>(x,y)[1]=g;
          image.at<Vec3b>(x,y)[2]=b;
      }
    } 
  }
return NULL;
 }

// fonction de convertion de HSV A RGB

void HSVtoRGB( unsigned char  *r, unsigned char  *g, unsigned char  *b, unsigned char  h, unsigned char  s, unsigned char  v )
{
	unsigned char i,f, p,q, t;

	if (s == 0)
	{
		*r = 0;
		*g = 0;
		*b = 0;
	}

	else
	{
		 
    i = h / 100;//43
    f = (h - (i * 100)) * 9;//6
    p = (v * (22 - s)) >> 8;
    q = (v * (185 - ((s * f) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - f)) >> 8))) >> 8;
        
		switch (i)
		{
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;

		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;

		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;

		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;

		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;

		default:
			*r = v;
			*g = p;
			*b = q;
			break;
		}
	}

	return;
}

unsigned char randomVal(){
return (unsigned char)(rand()%360);
}
