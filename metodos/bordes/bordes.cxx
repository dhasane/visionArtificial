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
  cv::Mat dst;

  Point anchor = Point( -1, -1 );
  double delta = 0;
  int ddepth = -1;

  filter2D(img, dst , ddepth , kernel , anchor, delta, BORDER_DEFAULT );
  cout<<kernel<<endl;
  return dst.clone();
}


Mat gauss(Mat img)
{
  Mat dst;

  Mat_<float> gauss(3,3);
  //gauss  << 3, 2, 3,
  //          2, 1, 2,
   //         3, 2, 3;
  //gauss /= 21;

  gauss  << 1, 1, 1,
            1, 1, 1,
            1, 1, 1;
  gauss /= 9;

  dst = aplicarKernel(img,gauss);
  // filter2D(img, dst , ddepth , gauss , anchor, delta, BORDER_DEFAULT );
  cout<<gauss<<endl;
  
  MatIterator_< Vec3b > it, end, it2, end2;
  
  it   = img.begin< Vec3b >( );
  end  = img.end< Vec3b >( );

  it2  = dst.begin< Vec3b >( );
  end2 = dst.end< Vec3b >( );

  for(  ; it != end, it2 != end2; ++it, ++it2)
  {
    *it2 -= *it ;
  }

  return dst.clone();
}

Mat difSeparada(Mat img)
{
  cv::Mat dst;
  Mat_<float> diferencia(3,3);
  diferencia << 0, -1,  0,
                1,  0, -1,
                0,  1,  0;

  dst = aplicarKernel( img, diferencia );

  return dst.clone();
}

Mat roberts(Mat img)
{
  cv::Mat dst;
  Mat_<float> diferencia(3,3);
  diferencia << -1,  0, -1,
                 0,  2,  0,
                 0,  0,  0;
  //diferencia /= 2;

  dst = aplicarKernel( img, diferencia );

  return dst.clone();
}

Mat difPixeles(Mat img)
{
  cv::Mat dst;
  Mat_<float> diferencia(3,3);
  diferencia <<  0,  -1,   0,
                 0,   2,  -1,
                 0,   0,   0;

  dst = aplicarKernel( img, diferencia );

  return dst.clone();
}

Mat prueba(Mat img)
{
  cv::Mat dst;

  //int tam = 7;
  int tam = 3;
  Mat_<float> diferencia(tam,tam);
  
  //*/

  /*/
  diferencia <<  1 , 0 , -1 ,
                 0 , 0 ,  0 ,
                -1 , 0 ,  1 ;
  */

  /*
  diferencia << -1 , 0 , -1 ,
                 0 , 0 ,  0 ,
                -1 , 0 ,  3 ;
  */

  /*
  diferencia << -1 , 1 , -1 ,
                 1 , 0 ,  1 ,
                -1 , 1 , -1 ;
  */
  /*
  diferencia << -1 ,  2 , -1 ,
                 2 , -4 ,  2 ,
                -1 ,  2 , -1 ;
  */
  diferencia <<  2 , -1 ,  2 ,
                -1 , -4 , -1 ,
                 2 , -1 ,  2 ;

  //diferencia /= 2;
  dst = aplicarKernel( img, diferencia );

  return dst.clone();
}


Mat pruebaCirc(Mat img)
{
  cv::Mat dst;

  int tam = 7;
  Mat_<float> diferencia(tam,tam);
  
  diferencia <<  4 ,  4 ,  2 ,  2  ,  2 ,  4 , 4 , // 22
                 4 ,  2 ,  2 , -2  ,  2 ,  2 , 4 , // 14     // 36
                 4 ,  2 , -3 , -4  , -3 ,  2 , 4 , // 3
                 2 , -2 , -4 , -10 , -4 , -2 , 2 , // -18    // -12       // total 60
                 4 ,  2 , -3 , -4  , -3 ,  2 , 4 , // 3
                 4 ,  2 ,  2 , -2  ,  2 ,  2 , 4 , // 14     // 36
                 4 ,  4 ,  2 ,  2  ,  2 ,  4 , 4 ; // 22

  diferencia /= 60;
  dst = aplicarKernel( img, diferencia );

  return dst.clone();
}

Mat canny(Mat img)
{
  cv::Mat dst;

  Mat_<float> gauss(3,3);
  gauss  << 1, 1, 1,
            1, 1, 1,
            1, 1, 1;
  gauss /= 9;
  dst = aplicarKernel(img, gauss);
  cout<<gauss<<endl;

  //*/
  float val2 = 0.25;
  Mat_<float> sobelx(3,3);
  sobelx <<   val2,   0,   -val2,
            2*val2,   0, -2*val2,
              val2,   0,   -val2;
  cout<<sobelx<<endl;

  dst = aplicarKernel(dst, sobelx);

  Mat_<float> sobely(3,3);
  sobely << -val2, -2*val2,  -val2,
                0,       0,      0,
             val2,  2*val2,   val2;

  dst = aplicarKernel(dst, sobely);

  cout<<sobely<<endl;

  // aqui hace falta la supresion no maximal :v

  // binarisar

  return dst.clone();
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
    string metodo;
    std::stringstream ss( argv[ 1 ] );
    std::string basename;
    getline( ss, basename, '.' );

    /*
    dst = canny(src);
    dst = gauss(src);
    dst = difSeparada(src);
    dst = roberts(src);
    dst = difPixeles(src);
    dst = prueba(src);
    */
    metodo = "canny";
    imwrite( basename + metodo + "resultado.jpg" , canny(src));

    metodo = "circular";
    imwrite( basename + metodo + "resultado.jpg" , pruebaCirc(src)-src );
    
    metodo = "gauss";
    imwrite( basename + metodo + "resultado.jpg" , gauss(src) );
    
    metodo = "difsep";
    imwrite( basename + metodo + "resultado.jpg" , difSeparada(src) );

    metodo = "roberts";
    imwrite( basename + metodo + "resultado.jpg" , roberts(src) );
    
    metodo = "difpixeles";
    imwrite( basename + metodo + "resultado.jpg" , difPixeles(src) );
    
    metodo = "experimental";
    imwrite( basename + metodo + "resultado.jpg" , prueba(src) );
    
    
    return 0;
}
