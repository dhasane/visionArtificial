#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstring>


using namespace cv;

using namespace std;




Mat gaussUp(Mat dst, Mat kernel)
{
  Point anchor = Point( -1, -1 );
  double delta = 0;
  int ddepth = -1;

  int tamx ;
  int tamy ;

  tamx = dst.size().width * 2;
  tamy = dst.size().height * 2;
  
  Mat nueva( tamx , tamy , CV_8UC3, Scalar(255,255,255));

  int mov = 0;
  for(int i=0; i < tamx; i++)
  {
    for (int j=0; j< tamy; j++)
    {
      if ( i%2 != 0 and j%2 != 0 )
      {
        nueva.at<Vec3b>( Point( i, j ) ) = dst.at<Vec3b>( Point( mov/dst.size().width, mov%dst.size().width ) );
        mov ++;
      }
      else
      {
        nueva.at<Vec3b>( Point( i, j ) ) = dst.at<Vec3b>(Point( i/2, j/2 ) );

        //nueva.at<Vec3b>( Point( i, j ) ) = dst.at<Vec3b>( Point( i-1 , j ) );
      }   
    }
  }
  filter2D(nueva, nueva, ddepth , kernel, anchor, delta, BORDER_DEFAULT );

  return nueva;
}


Mat gaussDown(Mat dst, Mat kernel)
{
  Point anchor = Point( -1, -1 );
  double delta = 0;
  int ddepth = -1;
  
  int tamx ;
  int tamy ;

  tamx = dst.size().width / 2;
  tamy = dst.size().height / 2;
    
  Mat nueva( tamx , tamy , CV_8UC3, Scalar(255,255,255));

  //filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );


  int mov = 0;
  for(int i=0; i < dst.rows; i++)
  {
    for (int j=0; j< dst.cols; j++)
    {
      if ( i%2 != 0 and j%2 != 0 )
      {
        nueva.at<Vec3b>(Point( mov/tamx, mov%tamx )) = dst.at<Vec3b>(Point( i, j ));
        mov ++;
      }        
    }
  }

  filter2D(dst, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );

  return nueva;
}



int main ( int argc, char** argv )
{

    if( argc < 2 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen)"<<endl;
      return -1;
    }
    Mat src, dst;
    //Mat kernel;
    //int kernel_size;
    /*const char* window_name = "filter2D Demo";
    // Initialize arguments for the filter
    Point anchor = Point( -1, -1 );
    double delta = 0;
    int ddepth = -1;*/

    //const char* imageName = argc >=2 ? argv[1]: "../512x512-Gaussian-Noise.jpg";

    const char* imageName = argv[1];

    // Loads an image
    src = imread( imageName, IMREAD_COLOR ); // Load an image
    if( src.empty() )
    {
        printf(" Error opening image\n");
        return -1;
    }

    //filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );

    //string mod = string( argv[2] );
    
    Mat_<float> kernel(5,5);
    
    kernel << 1,  4,  6,  4, 1, // 16
              4, 16, 24, 16, 4, // 64
              6, 24, 36, 24, 6, // 96
              4, 16, 24, 16, 4, // 64
              1,  4,  6,  4, 1; // 16
    
    //kernel/=16;
    kernel/=256;
    /*
    if ( argc >= 3 and "u" == mod )
    {
      dst = gaussUp(src);
    }
    else 
    {
      dst = gaussDown(src);
    }*/

    imwrite("4.jpg", src );

    dst = gaussUp(src, kernel);
    imwrite("5.jpg", dst );
    dst = gaussUp(dst, kernel);
    imwrite("6.jpg", dst );
    dst = gaussUp(dst, kernel);
    imwrite("7.jpg", dst );

    dst = gaussDown(src, kernel);
    imwrite("3.jpg", dst );
    dst = gaussDown(dst, kernel);
    imwrite("2.jpg", dst );
    dst = gaussDown(dst, kernel);
    imwrite("1.jpg", dst );
    
    return 0;
}
