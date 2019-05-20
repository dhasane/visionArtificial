#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

#include <iostream>
#include <vector>
#include <deque>
#include <stdio.h>
#include <cmath>

using namespace cv;
using namespace std;

#include "filtros.hxx"
#include "distancia.hxx"
//#include "regiones.hxx"

// todas las funciones, de ser necesario, convierten la imagen a gris, y al final lo retornan a bgr, para evitar complicaciones 
// la imagen al ser retornada por una funcion, siempre va a ser bgr 


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

    Mat hist;
    histograma(src,hist);
    imwrite(basename+"proyectohistograma.jpg", hist );

    Mat res = src.clone();
    
	// bordes o areas --------------------------------------------------------------------
    
	Bordes bd;
    //src = bd.canny(src);
    //bd.todos(src);
    //imwrite(basename+"Filtroproyecto.jpg", src );  
    
	// distancias ------------------------------------------------------------------------

	//Conjunto conj( src, true );
	//conj.conjuntoAImagen( src , src );
    
    //GaussianBlur( src, src, Size( 7, 7), 0, 0 );//applying Gaussian filter 
    
	// buscar por similaridad ------------------------------------------------------------



    imwrite(basename+"proyecto.jpg", src );    
    imwrite(basename+"proyectoForma.jpg", res-src );

    return 0;
}
