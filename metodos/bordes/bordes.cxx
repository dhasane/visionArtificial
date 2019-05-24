#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstring>


using namespace cv;

using namespace std;

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

Mat canny(Mat img , int lowTh, int  highTh , int tam  )
{
  cv::Mat dst;

  //int lowTh = 45;
  //int highTh = 90;
  cv::cvtColor(img, dst, COLOR_RGB2GRAY);                   // convert to grayscale

  blur( dst, dst, cv::Size(tam,tam) );           // Blur Effect             

  cv::Canny( dst, dst, lowTh, highTh , 3);       // Canny Edge Image
  
  cv::cvtColor(dst,dst , COLOR_GRAY2RGB);
  return dst.clone();
}

void binarizar(Mat & dest, Mat & hola , int umbral, int tope, int base);

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

<<<<<<< HEAD
	basename = "";
    
=======
    imwrite( "original.jpg" , src ) ;
    basename = "";
    Mat res, img;
	int tope = 255;
	int base = 0;
	int umbral= 125;
	int val = 100;

>>>>>>> 13f3a650e49f9e1ca069efc5b7caccaaedcd776b
    metodo = "canny";
	img = canny(src , val , val*3  , 5);
	binarizar(img ,res ,umbral, tope, base );
    imwrite( basename + metodo + "resultado.jpg" ,res ) ;

    metodo = "circular";
	img = pruebaCirc(src)-src;
    binarizar(img , res, umbral, tope, base );
    imwrite( basename + metodo + "resultado.jpg" ,res ) ;


    metodo = "gauss";
	img = gauss(src);
    binarizar(img , res, umbral, tope, base  );
    imwrite( basename + metodo + "resultado.jpg" ,res ); 


    metodo = "difsep";
	img = difSeparada(src);
    binarizar(img , res, umbral, tope, base  );
    imwrite( basename + metodo + "resultado.jpg" ,res ); 


    metodo = "roberts";
    img = roberts(src);
	binarizar(img , res, umbral, tope, base );
    imwrite( basename + metodo + "resultado.jpg" ,res ) ;


    metodo = "difpixeles";
	img = difPixeles(src);
    binarizar(img  , res, umbral, tope, base );
    imwrite( basename + metodo + "resultado.jpg" ,res ) ;


    metodo = "experimental";
	img = prueba(src);
    binarizar(img  , res, umbral, tope, base );
    imwrite( basename + metodo + "resultado.jpg" ,res ) ;
    
    
    return 0;
} 
<<<<<<< HEAD
//bunterwind
=======

// tope y base, uno debe ser 0 y el otro 255, hecho asi para poder invertir el resultado mas facilmente 
void binarizar(Mat & dest, Mat & hola , int umbral, int tope, int base)
{
	cvtColor( dest, hola, COLOR_BGR2GRAY );
	MatIterator_< Vec3b > it, end;
	it  = hola.begin< Vec3b >( );
	end = hola.end< Vec3b >( );
	for(  ; it != end; ++it)
	{
		if( (*it)[0] < umbral ) { (*it)[0] = tope; }
		else                    { (*it)[0] = base; }
	}
	cvtColor( hola, hola, COLOR_GRAY2BGR );
}
>>>>>>> 13f3a650e49f9e1ca069efc5b7caccaaedcd776b
