
#include <iostream>
#include <vector>
#include <deque>
#include <stdio.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"

using namespace cv;
using namespace std;


Vec3b BLANCO = Vec3b(255,255,255);
Vec3b NEGRO  = Vec3b(0,0,0);

class Punto{

    public:
        int x;
        int y;
        Vec3b color;

        Punto(int x, int y ,Vec3b col )
        {
            this->x = x;
            this->y = y;
            this->color = col;
        }

        void imprimir()
        {
            std::cout<<"("<<x<<","<<y<<")  "<<this->color<<endl;
        }       
};

class Recorrido{

    private:
        int ** mat;     // matriz 
        int tamx;       // tam en x
        int tamy;       // tam en y
        int noVistos;   // cantidad de pixeles no vistos

    public:
        // crea una matriz para verificar los pixeles ya vistos 
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
            noVistos = tamx * tamy;
        }

        // convierte el pixel en la posicion (x,y) en visto 
        void verPos(int x, int y)
        {
            mat[y][x] = 1;
            noVistos --;
        }

        // ya no visto
        void noVer(int x, int y)
        {
            mat[y][x] = 0;
            noVistos ++;
        }

        // en caso de ya haber sido visto, vist = true, no visto = false
        bool visto(int x, int y)
        {
            if ( mat[y][x] != 0 )
            {
                return true;
            }
            return false;
        }

        // imprime :v 
        void imprimir()
        {
            int cant = 0;
            for(int a = 0; a < this->tamy ; a++)
            {
                for (int b = 0; b < this->tamx ; b++)
                {
                    std::cout<<mat[a][b]<<" ";
                }
            }
        }

        bool interno(int x, int y)
        {
            if (0 < x && x < tamx && 0 < y && y < tamy)
            {
                return true;
            }
            return false;
        }
};

class Area{

    vector<Punto> *puntos;  // puntos del area
    deque<Punto> *posibles;// posibles puntos del area :D
    int id;

    public:
        
        Area( Punto p , int id )
        {
            this->id = id;
            puntos = new vector<Punto>();
            posibles = new deque<Punto>();
            puntos->push_back(p);
        }

        int size()
        {
            return puntos->size();
        }

        int sizePosibles()
        {
            return posibles->size();
        }

        Punto posiblesTop()
        {
            Punto pp = posibles->front();
            posibles->pop_front();
            return pp;
        }

        // une el vector posib al vector posibles
        void agregarPosibles( vector<Punto> posib )
        {
            this->posibles->insert(posibles->end(), posib.begin() , posib.end() );
        }

        bool insertar(Punto p )
        {
            Vec3b bla = Vec3b(255,255,255);
            int ye =  p.color == bla ? 1 : 0;
            
            if ( p.color == bla )
            {
                puntos->push_back( p );
                
                return true;
            }
            return false;
        }

        Punto top()
        {
            return puntos->back();
        }

        void imprimir()
        {
            std::cout<<size()<<endl;
        }

        // imprime una lista de puntos 
        void imprimirListaPuntos()
        {
            for (auto it = puntos->begin(); it != puntos->end(); ++it)
                it->imprimir();
        }

        // imprime una lista de puntos 
        void imprimirListaPosibles()
        {
            for (auto it = posibles->begin(); it != posibles->end(); ++it)
                it->imprimir();
        }
        
        void areaAImagen(Mat &img, int cantidad)
        {
            int r, g ,b ;
            int vv = 255 *3 / cantidad * this->id;
            r = vv % 256;
            vv -= 255;
            if ( vv > 0) g = vv % 256;
            vv -= 255;
            if ( vv > 0) b = vv % 256;
            Vec3b color = Vec3b(r,g,b);

            for (auto pt = puntos->begin(); pt != puntos->end(); ++pt)
            {
                img.at<Vec3b>( Point(pt->x, pt->y) ) = color;
            }
        }
};

class Conjunto{
    vector<Area> *areas;

    public:
        Conjunto()
        {
            areas = new vector<Area>();
        }

        void agregar(Area area)
        {
            areas->push_back( area );
        }

        int size()
        {
            return areas->size();
        }

        void conjuntoAImagen(Mat &img, Mat &res)
        {
            res = img.clone();
            //res = cv::Scalar(255,255,255);
            res = cv::Scalar(0,0,0);
            for (auto area = areas->begin(); area != areas->end(); ++area)
            {
                area->areaAImagen(res, this->size() );
            }
        }
};

void binarizar(Mat & dest, Mat & hola , int umbral);

void regiones(Mat &src, Mat &res, bool esquinas );

int main ( int argc, char** argv )
{
    if( argc < 3 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen) ( esquinas (0-1) )"<<endl;
      return -1;
    }
    const char* imageName =      argv[1];
    int         esquinas  = atoi(argv[2]);

    if ( esquinas != 0 && esquinas != 1 )
    {
        printf("esquinas debe ser 1 o 0\n");
        return -1;
    }

    Mat src;
    src = imread( imageName, IMREAD_COLOR ); 
    if( src.empty() )
    {
        printf("error al abrir la imagen\n");
        return -1;
    }
 
    Mat dest = src.clone();

    cout<<"tamaño imagen : "<< src.cols<< " , "<<src.rows<<endl<< "total pixeles : "<<src.cols * src.rows<<endl;
    regiones( dest, dest, (bool)esquinas );

    std::string basename = "";
    
    std::stringstream ss( argv[ 1 ] );
    getline( ss, basename, '.' );

    imwrite( basename + "Areas.jpg" , dest );
    Mat hola;
    binarizar(dest,hola,2);
    imwrite( basename + "Etiqueta.jpg" , hola );
    return 0;
}

void binarizar(Mat & dest, Mat & hola , int umbral)
{
    cvtColor( dest, hola, COLOR_BGR2GRAY );
    MatIterator_< Vec3b > it, end;
    it  = hola.begin< Vec3b >( );
    end = hola.end< Vec3b >( );
    for(  ; it != end; ++it)
    {
        if( (*it)[0] < umbral ) { (*it)[0] = 255; }
        else                    { (*it)[0] = 0; }
    }
    cvtColor( hola, hola, COLOR_GRAY2BGR );
}

// verifica si un punto es viable 
void mirarPunto(vector<Punto> &puntos, Mat &img, int x, int y, Recorrido rec)
{
    if( rec.interno(x,y) && !rec.visto(x,y) )
    {
        rec.verPos( x, y );
        Punto pt( x , y , img.at<Vec3b>( Point(x, y) ) );
        puntos.push_back( pt ) ;
    }
}

// mira los pixeles adyacentes al que se esta mirando, en caso de que estos sean viables los agrega a puntos
void adyacentes( vector<Punto> &puntos ,Mat &img, int x , int y, bool esquinas , Recorrido rec)
{
    mirarPunto(puntos, img, x-1, y,rec);
    mirarPunto(puntos, img, x+1, y,rec);
    mirarPunto(puntos, img, x, y-1,rec);
    mirarPunto(puntos, img, x, y+1,rec);

    if( esquinas )
    {
        mirarPunto(puntos, img, x-1, y-1,rec);
        mirarPunto(puntos, img, x+1, y-1,rec);
        mirarPunto(puntos, img, x-1, y+1,rec);
        mirarPunto(puntos, img, x+1, y+1,rec);
    }
}

// creo que todas estas funciones podrian moverse dentro de "Conjunto :v "
// consigue un area, segun un punto inicial de forma continua, para evitar los limites de la recursion
void conseguirArea(Area &area, Mat &img, bool esquinas, Recorrido rec)
{
    vector<Punto> *nuevos;
    
    nuevos = new vector<Punto>();

    adyacentes( *nuevos, img, area.top().x, area.top().y,esquinas, rec);
    area.agregarPosibles(*nuevos);  

    while( area.sizePosibles( ) > 0 )
    {
        Punto act = area.posiblesTop();
        if( area.insertar( act ) )
        {
            rec.verPos(area.top().x, area.top().y);
            nuevos = new vector<Punto>(); 
            adyacentes( *nuevos, img, area.top().x, area.top().y,esquinas, rec);
            area.agregarPosibles(*nuevos);
        }
        else
        {
            rec.noVer( act.x, act.y);
        }
    }    
}

void regiones(Mat &src, Mat &res, bool esquinas)
{
    Recorrido rec(src.cols,src.rows);
    Conjunto *conj = new Conjunto();

    Area* area;

    MatIterator_< Vec3b > it, end;
    
    int pos = 0;
    int x;
    int y;
    for( it  = res.begin< Vec3b >( ) ; it != res.end< Vec3b >( ); ++it) 
    {
        x = pos % res.cols;
        y = pos / res.cols;
        //cout<<(*it)<<endl;
        if ( (*it) == NEGRO )
        {
            rec.verPos( x, y );
        }
        else if ( !rec.visto( x, y) && (*it) == BLANCO) 
        {
            //cout<<"ini"<<endl;
            rec.verPos( x, y );
            area = new Area( Punto( x, y, *it ), conj->size() + 1 );  
            conseguirArea( *area, src, esquinas, rec );
            conj->agregar( *area );
            //cout<<"--------------------"<<endl;
        }
        pos++;
    }
    cout<<endl<<"areas : "<<conj->size()<<endl;
    conj->conjuntoAImagen(res, res);  
}
