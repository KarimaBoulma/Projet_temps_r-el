/* 
g++ -Wall opencv_exempleCPP.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -std=c++11 
*/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <complex>

using namespace std;
using namespace cv;
Mat image;

 
 #define V 4
 #define ITER 250
//nombre de thread
#define Nb_Thread 1
// mutex pour protéger VALEUR
pthread_mutex_t mutex; 

// taille de l'image
 #define HAUT 500
 #define LARG 500



//declaration des parametres de la couleur
void HSVtoRGB( unsigned char  *r, unsigned char  *g, unsigned char  *b, unsigned char  h, unsigned char  s, unsigned char  v );
// valeur reel et imaginaire de la fracttale

std::complex<long double> c( -0.7269,0.1889);













// déclarer la fonction pour la creation de la fractal
void *CalculFractaleJulia(void *arg);

int main( int argc, char** argv )
{
double temps;
 clock_t t1, t2;
 
 
pthread_t thread_f[Nb_Thread];
  image=cv::Mat(LARG, HAUT, CV_8UC3);
    
     
   
    
for(int i=0;i< Nb_Thread;i++){
t1 = clock();   
 pthread_create (&thread_f[i], NULL, CalculFractaleJulia, (void*)NULL);    
 }
 for(int i=0;i< Nb_Thread;i++){
  pthread_join(thread_f[i],NULL);
 }
 t2 = clock();
 temps = ((double)(t2-t1)/CLOCKS_PER_SEC);
 
 cout<<"le temps d'execution "<< temps <<"sec "<<endl;
//----------------------sauvegarde dans fichier----------------------------

//string  const mon_fichier = "resultat.txt";
ofstream fichier("resultat.txt", ios::app);
if(fichier){

fichier<<"resultat" <<temps<<"\n"<<endl;
fichier.close();

} else {
     cout<< "ERREUR: Impossible d'ouvrir le fichier"<<endl;
}
      
     while( true )


     {
     	 
	     imshow("julia image", image ); 
     	 if(cvWaitKey(5) > 0){
        break;
       }
}
 
  return 0;
}

void *CalculFractaleJulia(void *arg){
  for(int x=0;x<LARG;x++){
      for(int y=0;y<HAUT;y++){
        long double newX=((long double)((long double)x-((long double)LARG/2))/((long double)LARG/2));
        long double newY=((long double)((long double)y-((long double)HAUT/2))/((long double)HAUT/2));
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
		 
    i = h / 43;//43
    f = (h - (i * 43)) * 3;//6
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * f) >> 8))) >> 8;
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
