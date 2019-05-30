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

#include "librerias/vision/filtros.hxx"
#include "librerias/vision/distancia.hxx"

#include "librerias/fuzzy/Clasificacion.cpp"

#include "librerias/knn/knn.cxx"

int main( int argc, char* argv[] )
{
    if( argc < 2 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen) (archivo filtro) (archivo objetivo (puede no existir aun) ) (opcional: entrenamiento)"<<endl;
      return -1;
    }
    const char* imageName = argv[1];
	string arcFiltr       = argv[2];
	string arcObj         = argv[3];
    int         entrenm   = argc == 5 ? atoi(argv[4]) : 0 ; 

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

    bool guardar = true;

    // meta parametros ---------------------------
    int valorKnn = 1;
    int valorClasificacion = 1 ;
    // -------------------------------------------


    std::stringstream ss( argv[ 1] );
    std::string basename;
    getline( ss, basename, '.' );


    Bordes bd;

    Mat dst;
    dst.create( src.size(), src.type() );   
    
    // ubicar ---------------------------------------------------------------------------

    bd.canny( src, dst, 25 );
    if ( guardar ) imwrite( basename + "canny.jpg" , dst ) ;
    
    limpiar( false, dst, dst, 2, 1 );
    if ( guardar ) imwrite( basename + "limpieza.jpg" , dst ) ;

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

    // std::string archivoFiltro = "valores";
    Clasificacion clasif( valorClasificacion , entrenamiento );
    clasif.cargar( arcFiltr );

    clasif.imprimir( );

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
        clasif.guardar( arcFiltr );
    }
    else
    {
        conj.conjuntoAImagen( dst, res1, clasif );
	    if ( guardar ) imwrite( basename + "areasClasificadas.jpg" , res1 ) ;

        std::vector<float> evals = clasif.clasificarLista( distancias ) ; 
        std::cout << " evaluado : " ;
        for( auto bd = evals.begin() ; bd != evals.end(); ++bd)
        {
            std::cout << *bd << " ";
        }
        std::cout<<endl;

        Knn knn( valorKnn );
        knn.cargar( arcObj );

        knn.clasificar( evals, basename );

        knn.guardar( arcObj );
    }
    return 0;
}
