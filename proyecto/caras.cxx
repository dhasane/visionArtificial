#include <iostream>
#include <vector>
#include <deque>
#include <stdio.h>
#include <cmath>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;

// todas las funciones, de ser necesario, convierten la imagen a gris, y al final lo retornan a bgr, para evitar complicaciones 
// la imagen al ser retornada por una funcion, siempre va a ser bgr 

#include "vision/filtros.hxx"
#include "vision/distancia.hxx"

#include "fuzzy/Clasificacion.cpp"

int main( int argc, char* argv[] )
{
    CommandLineParser parser( argc, argv, "{@input |  | input image}" );
    Mat src = imread( parser.get<String>( "@input" ) );
    
    if( src.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }

    std::stringstream ss( argv[ 1] );
    std::string basename;
    getline( ss, basename, '.' );

    basename = ""; // para las pruebas 

    Bordes bd;

    Mat dst;
    dst.create( src.size(), src.type() );   
    
    


    // ubicar ---------------------------------------------------------------------------

    // void canny(Mat &src, Mat &dst, int lowThreshold )
    bd.canny( src, dst, 25 );

    // void limpiar( bool ero, Mat &img,Mat &res, int tipo , int tam)
    limpiar( false, dst, dst, 2, 2 );
    
    // void binarizar(Mat & dest, Mat & hola , int umbral, int tope, int base)
    binarizar( dst , dst , 1, 0, 255);




	// buscar por similaridad ------------------------------------------------------------

    Conjunto conj( dst, true );
	
	Mat res1;
    conj.conjuntoAImagen( dst, res1 );
	
	imwrite( "imagen.jpg" , res1 ) ;
	
    
    Mat res2;
    res2.create( src.size(), src.type() );  
    vector<float> distancias = conj.conjuntoADistancias( res2 );

    cout << "distancias : " << endl ; 


    // comparar a lo que ya se tiene -----------------------------------------------------
    
    Clasificacion clasif(1);
    std::string archivo = "valores";

    clasif.cargar( archivo );
    for( auto bd = distancias.begin() ; bd != distancias.end(); ++bd)
    {
        cout << *bd << endl;
        clasif.clasificar( *bd );
    }
    clasif.guardar( archivo );
    // */
    


	imwrite( "distancia.jpg" , res2 ) ;

    imwrite(basename+"proyecto.jpg", dst );    

    return 0;
}
