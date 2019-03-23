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

int main( int argc, char** argv )
{
  src = imread( argv[1], 0 );
  if( !src.data )
    { printf(" No data! -- Exiting the program \n");
      return -1; }

  tmp = src;
  dst = tmp;


  int kernel_size = 6;
  int delta = 0;
  Mat kernel;
  Point anchor = Point( -1, -1 );

 /*
  Mat matriz = (Mat_<double>(3,3) << 0, 1, 0,
                                     1,-4, 1,
                                     0, 1, 0);
   */

/*
  Mat matriz = (Mat_<double>(5,5) << 0, 1, 2, 1, 0,
                                     1, 2, -3, 2, 1,
                                     2, -3, -11, -3, 2,
                                     1, 2, -3, 2, 1,
                                     0, 1, 2, 1, 0);
*/
  Mat matriz = (Mat_<double>(5,5) << 0, 10, 15, 10, 0,
                                     10, 15, -25, 15, 10,
                                     15, -25, -99, -25, 15,
                                     10, 15, -25, 15, 10,
                                     0, 10, 15, 10, 0);

/*
  Mat matriz = (Mat_<double>(7,7) << 0, 2, 3, -1, 3, 2, 0,
                                     2, 3, -1, -2, -1, 3, 2,
                                     3, -1,-2,-3,-2,-1,3,
                                     -1,-2,-3,-11,-3,-2,-1,
                                     3,-1,-2,-3,-2,-1,3,
                                     2,3,-1,-2,-1,3,2,
                                     0,2,3,-1,3,2,0);
                                     
 */
  kernel = matriz/ (float)(kernel_size*kernel_size);
  
  
  /// Loop
  
  int a=1;
  kernel = matriz/ (float)(kernel_size*kernel_size);
  filter2D(tmp, dst, -1 , kernel, anchor, delta, BORDER_DEFAULT );  
  imwrite( "pl"+std::to_string(a)+".png", dst ); 
  a++;
  while( true && a<8)
  {
    //kernel_size = 3 + l;
    kernel = matriz/ (float)(kernel_size*kernel_size);
    filter2D(tmp, dst, -1 , kernel, anchor, delta, BORDER_DEFAULT );          
    resize(dst, dst, Size(), 2, 2, INTER_LINEAR);        
    imwrite( "pl"+std::to_string(a)+".png", dst );
    a++;
    tmp = dst;
  }


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