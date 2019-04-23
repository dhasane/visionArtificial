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

class Fuentes{

    public:

        int x;
        int y;

        Fuentes(int x, int y )
        {
            this->x = x;
            this->y = y;
        }

        void imprimir()
        {
            std::cout<<"("<<x<<","<<y<<")"<<endl;
        }
        
    private:
        
};

list<Fuentes> intensidadColores( Mat &res );

void imprimirLista(list<Fuentes> fuentes)
{
    for (auto it = fuentes.begin(); it != fuentes.end(); ++it)
      it->imprimir();
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


    list<Fuentes> fuentes = intensidadColores ( src );

    cout<<"tamaño imagen : "<< src.cols<< " , "<<src.rows<<endl;

    imprimirLista( fuentes);
    
    //imwrite( basename + metodo + ".jpg" , prueba(src) );
    
    
    return 0;
}


list<Fuentes> intensidadColores( Mat &res )
{
    
    MatIterator_< Vec3b > it, end;
    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );

    int maxAzul = 0;
    int maxVerde= 0;
    int maxRojo = 0;

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        if ( (*it)[0] > maxAzul)// azul
        {
            maxAzul  = (*it)[0];
        }
        if ( (*it)[1] > maxVerde)// verde
        {
            maxVerde = (*it)[1];
        }
        if ( (*it)[2] > maxRojo)// rojo
        {
            maxRojo  = (*it)[2];
        }
    }

    list<Fuentes> fuentes;

    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );
    int pos = 0;
    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        if ( (*it)[0] == maxAzul || (*it)[1] == maxVerde || (*it)[2] == maxRojo)
        {
            fuentes.insert(fuentes.end(), Fuentes( pos % res.cols ,pos / res.cols) );
        }
        pos++;
    }

    cout<<maxAzul<<"  "<<maxVerde<<"  "<<maxRojo<<"  "<<endl;

    return fuentes;


}
