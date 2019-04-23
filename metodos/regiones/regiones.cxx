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

class Punto{

    public:

        int x;
        int y;
        Vec3b color;

        Punto(int x, int y )
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

class Recorrido{

    private:
    int ** mat;
    int tamx;
    int tamy;

    public:
        Recorrido(int tamx, int tamy)
        {
            this->tamx = tamx;
            this->tamy = tamy;

            mat = new int*[tamy];
            for(int a = 0; a < tamy ; a++)
            {
                mat[a] = new int[tamx];
                for (int b = 0; b < tamx ; b++)
                {
                    mat[a][b] = 0;
                }
            }

        }

        void visto(int x, int y)
        {
            mat[y][x] = 1;
        }

        void imprimir()
        {
           for(int a = 0; a < this->tamy ; a++)
            {
                for (int b = 0; b < this->tamx ; b++)
                {
                    std::cout<<mat[a][b]<<" ";
                }
                std::cout<<std::endl;
            } 
        }
};

class Area{
    list<Punto> area;

    Area()
    {
        //area = new list<Punto>();
    }
};

void regiones(Mat &src, Mat &res, list<Punto> fuentes);
list<Punto> intensidadColores( Mat &res );

void imprimirLista(list<Punto> fuentes)
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


    list<Punto> fuentes = intensidadColores ( src );

    cout<<"tamaño imagen : "<< src.cols<< " , "<<src.rows<<endl;

    imprimirLista( fuentes);
    Mat dest;
    regiones(src,dest,fuentes);


    //imwrite( basename + + ".jpg" , prueba(src) );
    imwrite( "hola.jpg" , dest );
    
    
    return 0;
}

void regiones(Mat &src, Mat &res, list<Punto> fuentes)
{
    Recorrido rec(src.cols,src.rows);

    res = src.clone();
    Vec3b col = (0,0,0);
    for (auto it = fuentes.begin(); it != fuentes.end(); ++it)
    {
        res.at<Vec3b>(Point(it->x, it->y)) = col;
        rec.visto(it->x, it->y);
    }
      
    
    rec.imprimir();
}


list<Punto> intensidadColores( Mat &res )
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

    list<Punto> fuentes;

    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );
    int pos = 0;
    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        if ( (*it)[0] == maxAzul || (*it)[1] == maxVerde || (*it)[2] == maxRojo)
        {
            fuentes.insert(fuentes.end(), Punto( pos % res.cols ,pos / res.cols) );
        }
        pos++;
    }

    cout<<maxAzul<<"  "<<maxVerde<<"  "<<maxRojo<<"  "<<endl;

    return fuentes;


}
