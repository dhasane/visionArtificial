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

Mat erocion( Mat &img, int erosion_elem, int erosion_size )
{
    int erosion_type;
    Mat src;
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

    Mat element = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size ) );

    /// Apply the erosion operation
    erode( img, src, element );
    //imshow( "Erosion Demo", erosion_dst );
    return src.clone();
}

/** @function Dilation */
Mat dilatar( Mat &img, int dilation_elem, int dilation_size )
{
  int dilation_type;
  Mat src;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  dilate( img , src, element );
  //imshow( "Dilation Demo", dilation_dst );
  return src.clone();
}

// ambas comparten el tamaño, debiido a que se quiere "volver"
Mat limpiar(Mat &img, int tipoero, int tipodil, int tam)
{
                      // tipo // tamaño
    Mat res = erocion(img , tipoero , tam );
    res     = dilatar(res , tipodil , tam );
    
    return res.clone();
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


    Mat res ; 
    char pos[20];
    
    for ( int a = 0 ; a < 3 ; a ++)
    {
        for ( int b = 0 ; b < 3 ; b ++)
        {
            res = limpiar( src, a, b, 2 );
            sprintf (pos, "%d-%d-",a, b);
            imwrite( basename + "-"+ pos + "limpio.jpg" , src-res );
        }
    }

    
    
    
    
    return 0;
}
