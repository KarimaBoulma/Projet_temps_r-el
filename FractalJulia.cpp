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

