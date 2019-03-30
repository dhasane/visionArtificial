#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

// encuentra la varianza para un color y su peso respectivo 
void var(int color[] ,int total,int liminf, int limsup,float & varianza, float & peso)
{
    float promedio = 0;
    int tlocal     = 0;

    varianza = 0 ;
    peso     = 0;

    for ( int a = liminf ; a < limsup ; a ++)
    {
        peso     += color[a];
        promedio += a*color[a];
        tlocal   += color[a];
    }
    peso     /= total;
    promedio /= tlocal;
    for ( int a = liminf ; a < limsup ; a ++)
    {
        varianza += pow((a-promedio), 2 ) + color[a];
    }
    
    if (tlocal != 0)
    {
        varianza /= tlocal;
    }
    else
    {
        varianza  = 0;
    }
}

// encuentra el umbral optimo, a traves del metodo de otsu 
float encontrarUmbral(int color[])
{
    int totalcolor = 0;

    for ( int i  = 0 ; i <= 255 ; i++)
    {
        totalcolor += color[i];
    }
    cout<<totalcolor<<endl;

    float vA  = 0;
    float cvA = 0;

    float pA  = 0;
    float cpA = 0;

    float minv   = 0 ;
    float varmin = 0 ;

    float actv   = 0 ;
    float varact = 0 ;

    for ( int corte  = 1 ; corte <= 255 ; corte++)
    {

        var(color,totalcolor, 0, corte, vA, pA);

        var(color,totalcolor, corte+1, 256,cvA,cpA);
        
        
        actv   = corte;
        varact = pA*vA + cpA*cvA;

        if ( corte == 1 )
        {
            minv   = actv;
            varmin = varact;
        }
        else if ( varact < varmin)
        {
            minv   = actv;
            varmin = varact;
        }
    }
    cout<<"umbral optimo en : "<<minv<<" con "<<varmin<<endl;

    return minv;
}

int main ( int argc, char** argv )
{
    //const char* imageName = argc >=2 ? argv[1]: "../512x512-Gaussian-Noise.jpg"; // quiero aprender a usar esto 
    const char* imageName = argv[1];
    // se carga la imagen 
    Mat src = imread( imageName, IMREAD_COLOR ); // Load an image
    if( src.empty() )
    {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default ../512x512-Gaussian-Noise.jpg] \n");
        return -1;
    }
    
    // vectores de aparicion de intensidades 
    int verde[256] = {0};
    int rojo [256] = {0};
    int azul [256] = {0};
    
    MatIterator_< Vec3b > it, end;
    it  = src.begin< Vec3b >( );
    end = src.end< Vec3b >( );

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        azul [(*it)[0]] += 1;
        verde[(*it)[1]] += 1;
        rojo [(*it)[2]] += 1;
    }

    // se encuentran los umbrales 

    float umbAzul  = encontrarUmbral(azul);

    float umbVerde = encontrarUmbral(verde);

    float umbRojo  = encontrarUmbral(rojo);

    // se crea una nueva imagen en la que se guardaran los valores 
    Mat AZ(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));
    Mat RO(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));
    Mat VE(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));


    // se inicializan los iteradores 
    it = src.begin< Vec3b >( );
    end = src.end< Vec3b >( );

    MatIterator_< Vec3b > az, ro ,ve, eaz, ero , eve ;

    az = AZ.begin< Vec3b >();
    ro = RO.begin< Vec3b >();
    ve = VE.begin< Vec3b >();

    eaz = AZ.end< Vec3b >();
    ero = RO.end< Vec3b >();
    eve = VE.end< Vec3b >();

    // se recorre la imagen original y en caso de una intensidad ser menor al umbral especifico, se guarda 0, de lo contrario 255 
    for(  ; it != end, az != eaz, ro != ero, ve != eve; ++it, ++az, ++ro, ++ve)
    {
        if ( (*it)[0] > umbAzul  ) { (*az)[0] = 255; (*it)[0] = 255 ; } // azul
        else                       { (*az)[0] = 0;   (*it)[0] = 0   ; }

        if ( (*it)[1] > umbRojo  ) { (*ve)[1] = 255; (*it)[0] = 255 ; } // verde 
        else                       { (*ve)[1] = 0;   (*it)[0] = 0   ; }

        if ( (*it)[2] > umbVerde ) { (*ro)[2] = 255; (*it)[0] = 255 ; } // rojo 
        else                       { (*ro)[2] = 0;   (*it)[0] = 0   ; }
    }

    imwrite("azul.jpg" ,  AZ);
    imwrite("rojo.jpg" ,  RO);
    imwrite("verde.jpg",  VE);

    imwrite("union.jpg",  src);
    
    return 0;
}
