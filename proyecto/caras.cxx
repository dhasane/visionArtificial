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

#include "knn/knn.cxx"

int main( int argc, char* argv[] )
{
    bool guardar = false;
    if( argc < 2 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen) (opcional: entrenamiento)"<<endl;
      return -1;
    }
    const char* imageName =      argv[1];
    int         entrenm   = argc == 3 ? atoi(argv[2]) : 0 ; 

    if ( entrenm != 0 && entrenm != 1 )
    {
        printf("entrenamiento debe ser 1 o 0\n");
        return -1;
    }

    Mat src = imread( imageName );

    if( src.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }

    std::stringstream ss( argv[ 1] );
    std::string basename;
    getline( ss, basename, '.' );

    // basename = ""; // para las pruebas 

    Bordes bd;

    Mat dst;
    dst.create( src.size(), src.type() );   
    
    


    // ubicar ---------------------------------------------------------------------------

    // void canny(Mat &src, Mat &dst, int lowThreshold )
    bd.canny( src, dst, 25 );
    if ( guardar ) imwrite( basename + "canny.jpg" , dst ) ;

    // void limpiar( bool ero, Mat &img,Mat &res, int tipo , int tam)
    limpiar( false, dst, dst, 2, 1 );
    
    if ( guardar ) imwrite( basename + "limpieza.jpg" , dst ) ;

    // void binarizar(Mat & dest, Mat & hola , int umbral, int tope, int base)
    binarizar( dst , dst , 1, 0, 255);

    if ( guardar ) imwrite( basename + "bin.jpg" , dst ) ;


	// buscar por similaridad ------------------------------------------------------------

    Conjunto conj( dst, true );
	
	Mat res1;
    conj.conjuntoAImagen( dst, res1 );
	
	if ( guardar ) imwrite( basename + "imagen.jpg" , res1 ) ;
	
    
    Mat res2;
    res2.create( src.size(), src.type() );  
    vector<float> distancias = conj.conjuntoADistancias( res2 );
    if ( guardar ) imwrite( basename + "distancia.jpg" , res2 ) ;

    // comparar a lo que ya se tiene -----------------------------------------------------
    
    bool entrenamiento = (bool) entrenm;

    std::string archivoFiltro = "valores";
    Clasificacion clasif( 1 , entrenamiento );
    clasif.cargar( archivoFiltro );

    cout << "------------\n";

    if ( guardar ) imwrite( basename + "Resultado.jpg", dst ); 

    if ( entrenamiento )
    {
        cout << "entrenamiento\n";
        for( auto bd = distancias.begin() ; bd != distancias.end(); ++bd)
        {
            cout << *bd << endl;
            clasif.clasificar( *bd );
        }
        clasif.guardar( archivoFiltro );
    }
    else
    {
        conj.conjuntoAImagen( dst, res1, clasif );
	    if ( guardar ) imwrite( basename + "areasClasificadas.jpg" , res1 ) ;

        std::vector<float> evals = clasif.clasificarLista( distancias ) ; 
        string archivoPersonas = "personas";

        Knn knn( 20 );
        knn.cargar( archivoPersonas );

        knn.clasificar( evals );

        knn.guardar( archivoPersonas );
    }
    cout << " terminado \n";
    return 0;
}
