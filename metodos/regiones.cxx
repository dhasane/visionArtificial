#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstring>

using namespace cv;
using namespace std;

/// Global variables
Mat src, dst, tmp;

struct Pixel {
  Vec3b pixel;
  uchar intensidad;
  bool revisado;
};

int main( int argc, char** argv )
{

  src = imread( argv[1], 0 );
  if( !src.data )
    { printf(" No data! -- Exiting the program \n");
      return -1; }

  int numeroFilas = src.rows;
  int numeroColumnas = src.cols;
  Pixel matriz[numeroFilas][numeroColumnas];;

  for(int i=0; i<numeroFilas; i++){
    for(int j=0; j<numeroColumnas; j++){
        matriz[i][j].pixel=src.at<Vec3b>(i,j);
        Vec3b pixel = src.at<Vec3b>(i,j);
        uchar B = pixel[0];
        uchar G = pixel[1];
        uchar R = pixel[2];
        matriz[i][j].intensidad = (B+G+R)/3;
    }
  }

  for(int i=0; i<numeroFilas; i++){
    for(int j=0; j<numeroColumnas; j++){
        cout<<(int)matriz[i][j].intensidad<<endl;
    }
  }

  tmp = src;
  dst = tmp;
  


  return 0;
}




/*
printf( "** Zoom In: Image x 2 \n" );
          // Apply filter
          kernel_size = 2;
          matriz = (Mat_<double>(3,3) << 0, 1, 0,
                                     1,-3, 1,
                                     0, 1, 0);
          kernel = matriz/ (float)(kernel_size*kernel_size);
          filter2D(tmp, dst, -1 , kernel, anchor, delta, BORDER_DEFAULT );          
          resize(dst, dst, Size(), 2, 2, INTER_LINEAR);        
          imwrite( "lb"+std::to_string(1)+".png", dst );
          a++;
          k=k+2;
          tmp = dst;
          kernel_size = 4;
          matriz = (Mat_<double>(5,5) << 0, 1, 2, 1, 0,
                                     1, 2, -3, 2, 1,
                                     2, -3, -11, -3, 2,
                                     1, 2, -3, 2, 1,
                                     0, 1, 2, 1, 0);
          kernel = matriz/ (float)(kernel_size*kernel_size);
          filter2D(tmp, dst, -1 , kernel, anchor, delta, BORDER_DEFAULT );          
          resize(dst, dst, Size(), 2, 2, INTER_LINEAR);        
          imwrite( "lb"+std::to_string(2)+".png", dst );
          a++;
          k=k+2;
          tmp = dst;
          kernel_size = 6;
          matriz = (Mat_<double>(7,7) << 0, 2, 3, -1, 3, 2, 0,
                                     2, 3, -1, -2, -1, 3, 2,
                                     3, -1,-2,-3,-2,-1,3,
                                     -1,-2,-3,-11,-3,-2,-1,
                                     3,-1,-2,-3,-2,-1,3,
                                     2,3,-1,-2,-1,3,2,
                                     0,2,3,-1,3,2,0);
          kernel = matriz/ (float)(kernel_size*kernel_size);
          filter2D(tmp, dst, -1 , kernel, anchor, delta, BORDER_DEFAULT );          
          resize(dst, dst, Size(), 2, 2, INTER_LINEAR);        
          imwrite( "lb"+std::to_string(3)+".png", dst );
          a++;
          k=k+2;
          tmp = dst;
*/