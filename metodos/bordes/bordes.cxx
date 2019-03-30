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

// solo sirve cuando se quiere efectuar el kernel sobre la misma matriz de inicio, en vez guardarlo en otra
Mat aplicarKernel(Mat img, Mat kernel)
{
  Point anchor = Point( -1, -1 );
  double delta = 0;
  int ddepth = -1;

  filter2D(img, img , ddepth , kernel , anchor, delta, BORDER_DEFAULT );
  cout<<kernel<<endl;
  return img;
}


Mat gauss(Mat img)
{
  Mat dst;
  Point anchor = Point( -1, -1 );
  double delta = 0;
  int ddepth = -1;

  Mat_<float> gauss(3,3);
  //gauss  << 3, 2, 3,
  //          2, 1, 2,
   //         3, 2, 3;
  //gauss /= 21;

  gauss  << 1, 1, 1,
            1, 1, 1,
            1, 1, 1;
  gauss /= 9;

  filter2D(img, dst , ddepth , gauss , anchor, delta, BORDER_DEFAULT );
  cout<<gauss<<endl;
  
  MatIterator_< Vec3b > it, end, it2, end2;
  
  it   = img.begin< Vec3b >( );
  end  = img.end< Vec3b >( );

  it2  = dst.begin< Vec3b >( );
  end2 = dst.end< Vec3b >( );

  for(  ; it != end, it2 != end2; ++it, ++it2)
  {
    *it -= *it2 ;
  }

  return img;
}

Mat difSeparada(Mat img)
{
  Mat_<float> diferencia(3,3);
  diferencia << 0, -1,  0,
                1,  0, -1,
                0,  1,  0;

  img = aplicarKernel( img, diferencia );

  return img;
}

Mat roberts(Mat img)
{
  Mat_<float> diferencia(3,3);
  diferencia << -1,  0, -1,
                 0,  2,  0,
                 0,  0,  0;
  diferencia /= 2;

  img = aplicarKernel( img, diferencia );

  return img;
}

Mat difPixeles(Mat img)
{
  Mat_<float> diferencia(3,3);
  diferencia <<  0,  -1,   0,
                 0,   2,  -1,
                 0,   0,   0;

  img = aplicarKernel( img, diferencia );

  return img;
}

Mat prueba(Mat img)
{
  Mat_<float> diferencia(3,3);
  diferencia <<  -1,  -1,  -1,
                  1,   2,   1,
                  0,   1,   0;
  diferencia /= 2;
  img = aplicarKernel( img, diferencia );

  return img;
}

Mat canny(Mat img)
{
  Point anchor = Point( -1, -1 );
  double delta = 0;
  int ddepth = -1;
  Mat_<float> gauss(3,3);
  gauss  << 1, 1, 1,
            1, 1, 1,
            1, 1, 1;
  gauss /= 9;
  
  filter2D(img, img , ddepth , gauss , anchor, delta, BORDER_DEFAULT );
  cout<<gauss<<endl;
  //*/
  float val2 = 0.25;
  Mat_<float> sobelx(3,3);
  sobelx <<   val2,   0,   -val2,
            2*val2,   0, -2*val2,
              val2,   0,   -val2;
  cout<<sobelx<<endl;

  filter2D(img, img , ddepth , sobelx , anchor, delta, BORDER_DEFAULT );

  Mat_<float> sobely(3,3);
  sobely << -val2, -2*val2,  -val2,
                0,       0,      0,
             val2,  2*val2,   val2;

  filter2D(img, img , ddepth , sobely , anchor, delta, BORDER_DEFAULT );

  cout<<sobely<<endl;

  // aqui hace falta la supresion no maximal :v

  // binarisar

  return img;
}


int main ( int argc, char** argv )
{
    if( argc < 2 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen)"<<endl;
      return -1;
    }
    Mat src, dst;
    const char* imageName = argv[1];

    // Loads an image
    src = imread( imageName, IMREAD_COLOR ); // Load an image
    if( src.empty() )
    {
        printf(" Error opening image\n");
        return -1;
    }
    
    // falta alguna forma de definir el grosor ...

    //dst = canny(src);
    dst = gauss(src);
    //dst = difSeparada(src);
    //dst = roberts(src);
    //dst = difPixeles(src);
    //dst = prueba(src);
    imwrite("resultado.jpg", dst );
    
    return 0;
}
