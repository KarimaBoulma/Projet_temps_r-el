/**
\mainpage Page de Documentation

Ce programme a pour but de calculer et manipuler la fractale de Julia.
Ce projet m'a été demandé pour le cours de Programmation temps réel durant le 1er semestre du Master 1 informatique à l'université Paris 8.
*/


/**
* \file ProjetKarima2.cpp
* \brief Progamme de calcul et d'affiche de la fractale de Julia
* \author Karima
* \version 1.0
* Programme permettant de calculer et afficher la fractale de julia ainsi que de la manipuler depusi des raccourcis clavier
*/

/* 
g++ -Wall opencv_exempleCPP.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -std=c++11 
*/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <complex>

using namespace std;
using namespace cv;

///L'image de la fractale
Mat image;

 
 ///Variable de Julia
 #define V 4 
///Variable de Julia
 #define ITER 250  

 //nombre de thread
 ///Nombre de thread par défaut
#define Nb_Thread 3

///Mutex pour protéger le calcul des pixels
pthread_mutex_t mutex;

// taille de l'image
///Hauteur de l'image
 #define HAUT 500
///Largeur de l'image
 #define LARG 500


///Deplacement x et y
long double depx=0.0,depy=0.0;
///Zoom sur l'image
long double zoom=1.0;
///Mode couleur ou noir et blanc
bool bcouleur=true;
//declaration des parametres de la couleur
void HSVtoRGB( unsigned char  *r, unsigned char  *g, unsigned char  *b, unsigned char  h, unsigned char  s, unsigned char  v );
// valeur reel et imaginaire de la fracttale

///nombre complex: constante de Julia
std::complex<long double> c( -0.7269,0.1889);



// déclarer la fonction pour la creation de la fractal
void CalculFractaleJulia();


///Main: initialise le mutex et gère l'interaction
int main( int argc, char** argv )
{// declaration de la fonction de temps d'exécution
image=cv::Mat(LARG, HAUT, CV_8UC3);
    
     
   	bool continuer=true;
     CalculFractaleJulia();
     while( continuer )
     {
     	
	    imshow("julia image", image ); 
     	switch(cvWaitKey(5)){
     		case 27:
        	continuer=false;
      		break;
      		case 'a':
        	zoom+=0.1;
        	CalculFractaleJulia();
      		break;
      		case 'b':
      		zoom-=0.1;
      		CalculFractaleJulia();
      		break;
      		case 'c':
      		depx+=0.1;
      		CalculFractaleJulia();
	     	break;
	     	case 'd':
      		depx-=0.1;
      		CalculFractaleJulia();
      		break;
      		case 'e':
      		depy+=0.1;
      		CalculFractaleJulia();
	     	break;
	     	case 'f':
      		depy-=0.1;
      		CalculFractaleJulia();
      		break;
      		case 'g':
      			if(bcouleur){
      				bcouleur=false;
      			}else{
      				bcouleur=true;
      			}
      			CalculFractaleJulia();
      		break;
	     	default:
	     	break;
       	}
}
 
  return 0;
}

///CalculFractaleJulia: permet de calculer la fractale de Julia
void CalculFractaleJulia(){
  for(int x=0;x<LARG;x++){
      for(int y=0;y<HAUT;y++){
        long double newX=((long double)((long double)x-((long double)LARG/2))/((long double)LARG/2*zoom))+depy;
        long double newY=((long double)((long double)y-((long double)HAUT/2))/((long double)HAUT/2*zoom))+depx;
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
	  	  if(bcouleur){
	  	  HSVtoRGB( & r, & g, & b, couleur, brillance, saturation );
          image.at<Vec3b>(x,y)[0]=r;
          image.at<Vec3b>(x,y)[1]=g;
          image.at<Vec3b>(x,y)[2]=b;
      	}else{
      		image.at<Vec3b>(x,y)[0]=couleur;
          image.at<Vec3b>(x,y)[1]=couleur;
          image.at<Vec3b>(x,y)[2]=couleur;
      		
      	}
      }
    } 
  }
	return;
 }


///HSVtoRGB: fonction de convertion de HSV A RGB
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

///randomVal: fonction retournant des randoms unsigned char compris entre 0 et 360 (pour le HSV)
unsigned char randomVal(){
return (unsigned char)(rand()%360);
}
