
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

bool local = true;

class Totales{

    public:
        float total0;
        float total1; // para cada uno de los canales
        float total2;

        int cant;

        Totales()
        {
            this->total0 = 0;
            this->total1 = 0; // full lazy 
            this->total2 = 0;
            this->cant = 0;
        }

        void agregarATotal( Vec3b color )
        {
            this->total0 += color[0];
            this->total1 += color[1]; // full lazy 
            this->total2 += color[2];
            
            this->cant ++;
        }
};

Totales Total;

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
            std::cout<<"("<<x<<","<<y<<")"<<endl;
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

    Totales promedios;

    public:
        
        Area( Punto p )
        {
            puntos = new vector<Punto>();
            posibles = new deque<Punto>();
            puntos->push_back(p);

            if (local)
            {
                promedios.agregarATotal( p.color );
            }
            else
            {
                Total.agregarATotal( p.color );
            }
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

        bool insertar(Punto p, int cercania )
        {
            float total0;
            float total1; // para cada uno de los canales
            float total2;
            int cant;

            if (local)
            {
                total0 = promedios.total0;
                total1 = promedios.total1;
                total2 = promedios.total2;
                cant   = promedios.cant;
            }
            else
            {
                total0 = Total.total0;
                total1 = Total.total1;
                total2 = Total.total2;
                cant   = Total.cant;
            }
            
            float vinf = total0/cant - cercania;
            float vsup = total0/cant + cercania;

            if (vinf < 0)   vinf = 0;
            if (255 < vsup) vsup = 255;
            
            bool bo0 = vinf < p.color[0] && p.color[0] < vsup;

            vinf = total1/cant - cercania;
            vsup = total1/cant + cercania;
            if (vinf < 0)   vinf = 0;
            if (255 < vsup) vsup = 255;

            bool bo1 = vinf < p.color[1] && p.color[1] < vsup;

            vinf = total2/cant - cercania;
            vsup = total2/cant + cercania;
            if (vinf < 0)   vinf = 0;
            if (255 < vsup) vsup = 255;

            bool bo2 = vinf < p.color[2] && p.color[2] < vsup;

            if ( bo0 && bo1 && bo2 )
            {
                puntos->push_back( p );
                
                if (local)
                {
                    promedios.agregarATotal( p.color );
                }
                else
                {
                    Total.agregarATotal( p.color );
                }
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
            std::cout<<size()<<" "<<colorPromedio()<<endl;
        }

        // imprime una lista de puntos 
        void imprimirListaPuntos()
        {
            for (auto it = puntos->begin(); it != puntos->end(); ++it)
                it->imprimir();
        }

        Vec3b colorPromedio()
        {
            float total0;
            float total1; // para cada uno de los canales
            float total2;
            int cant;

            if (local)
            {
                total0 = promedios.total0;
                total1 = promedios.total1;
                total2 = promedios.total2;
                cant   = promedios.cant;
            }
            else
            {
                total0 = Total.total0;
                total1 = Total.total1;
                total2 = Total.total2;
                cant   = Total.cant;
            }

            Vec3b color;
            color[0] = total0/cant;
            color[1] = total1/cant;
            color[2] = total2/cant;

            return color;
        }
        
        void areaAImagen(Mat &img)
        {
            Vec3b color = colorPromedio();

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
            res = cv::Scalar(255,255,255);
            res = cv::Scalar(0,0,0);
            for (auto area = areas->begin(); area != areas->end(); ++area)
            {
                area->areaAImagen(res);
            }
        }
};

// corta, pega, une, hace magia y retorna una imagen con las areas encontradas
void regiones(Mat &src, Mat &res, bool esquinas, int distancia, vector<Punto> fuentes);

void binarizar(Mat & dest, Mat & hola , int umbral);

// seleccion de fuentes
// toma los valores con max y min intensidad
vector<Punto> intensidadColores( Mat &res ,bool mid);

// todos los pixeles como fuentes -> el mejor hasta el momento
vector<Punto> todos( Mat &res );

// muestra una pantalla, para que el usuario decida fuentes
vector<Punto> manual(Mat & img);

int main ( int argc, char** argv )
{
    if( argc < 5 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen) ( esquinas (0-1) ) (distancia entre colores) (metodo 1(manual) 2(todos) 3(intensidades) ) (opcional: local)"<<endl;
      return -1;
    }
    const char* imageName =      argv[1];
    int         esquinas  = atoi(argv[2]);
    float       distancia = atof(argv[3]);
    int         metodo    = atoi(argv[4]);
    int         localidad = argc == 6 ? atoi(argv[5]) : 1 ; 

    if ( localidad != 0 && localidad != 1 )
    {
        printf("local debe ser 1 o 0\n");
        return -1;
    }
    local = (bool) localidad;

    if ( esquinas != 0 && esquinas != 1 )
    {
        printf("esquinas debe ser 1 o 0\n");
        return -1;
    }

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

    if ( 0 > distancia || distancia > 255 )
    {
        printf("la distancia debe estar entre 0 y 255 \n");
        return -1;
    }
    
    int topeMetodos= 3;
    if(0 >= metodo || metodo > topeMetodos )
    {
        printf("metodo debe estar entre 1 y %d \n",topeMetodos);
        return -1;
    }

    vector<Punto> fuentes;
    Mat dest = src.clone();

    switch(metodo)
    {
        case 1: fuentes = manual ( dest );                      break;
        case 2: fuentes = todos ( dest );                       break;
        case 3: fuentes = intensidadColores ( dest , false );   break;
    }
    
    cout<<"tamaño imagen : "<< src.cols<< " , "<<src.rows<<endl<< "total pixeles : "<<src.cols * src.rows<<endl;
    cout<<"fuentes : "<<fuentes.size()<<endl;
    regiones( dest, dest, (bool)esquinas , distancia, fuentes);

    std::string basename = "";
    
    std::stringstream ss( argv[ 1 ] );
    getline( ss, basename, '.' );

    imwrite( basename + "Areas.jpg" , dest );
    Mat hola;
    binarizar(dest,hola,2);
    imwrite( basename + "Etiqueta.jpg" , hola );
    imwrite( basename + "Segmentada.jpg" , src*hola );
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
        if( (*it)[0] < umbral ) { (*it)[0] = 0; }
        else                    { (*it)[0] = 255; }
    }
    cvtColor( hola, hola, COLOR_GRAY2BGR );
}

// verifica si un punto es viable 
bool mirarPunto(vector<Punto> &puntos, Mat &img, int x, int y, Recorrido rec)
{
    if( rec.interno(x,y) && !rec.visto(x,y) )
    {
        rec.verPos( x, y );
        puntos.push_back( Punto( x , y , img.at<Vec3b>( Point(x, y) ) ) );
        return true;
    }
    return false;
}

// mira los pixeles adyacentes al que se esta mirando, en caso de que estos sean viables los agrega a puntos
bool adyacentes(vector<Punto> &puntos, Mat &img, int x , int y, bool esquinas , Recorrido rec)
{
    bool cambio = false;

    cambio |= mirarPunto(puntos, img, x-1, y,rec);
    cambio |= mirarPunto(puntos, img, x+1, y,rec);
    cambio |= mirarPunto(puntos, img, x, y-1,rec);
    cambio |= mirarPunto(puntos, img, x, y+1,rec);

    if(esquinas)
    {
        cambio |= mirarPunto(puntos, img, x-1, y-1,rec);
        cambio |= mirarPunto(puntos, img, x+1, y-1,rec);
        cambio |= mirarPunto(puntos, img, x-1, y+1,rec);
        cambio |= mirarPunto(puntos, img, x+1, y+1,rec);
    }

    return cambio;
}
// creo que todas estas funciones podrian moverse dentro de "Conjunto :v "
// consigue un area, segun un punto inicial de forma continua, para evitar los limites de la recursion
void conseguirArea(Area &area, Mat &img, bool esquinas,int distancia, Recorrido rec)
{
    vector<Punto> *nuevos;

    // los adyacentes del primer punto 
    nuevos = new vector<Punto>();
    adyacentes(*nuevos, img, area.top().x, area.top().y,esquinas, rec);
    area.agregarPosibles(*nuevos);  

    while( area.sizePosibles( ) > 0 )
    {
        Punto act = area.posiblesTop();

        if( area.insertar( act , distancia ) )
        {
            rec.verPos(area.top().x, area.top().y);
            nuevos = new vector<Punto>(); 
            adyacentes(*nuevos, img, area.top().x, area.top().y,esquinas, rec);
            area.agregarPosibles(*nuevos);
        }
        else
        {
            rec.noVer( act.x, act.y);
        }
    }    
}

// consigue n regiones y afecta la imagen para representarlas 
void regiones(Mat &src, Mat &res, bool esquinas, int distancia, vector<Punto> fuentes)
{
    Recorrido rec(src.cols,src.rows);
    Conjunto *conj = new Conjunto();
    
    Vec3b col = (0,0,0);

    Area* area;
    for (auto it = fuentes.begin(); it != fuentes.end(); ++it)
    {
        // se hace de esta forma, en caso de que dos semillas se sobrelapen, haciendo que solo la primera semilla gane
        if ( !rec.visto(it->x, it->y) ) 
        {
            rec.verPos(it->x, it->y);
            area = new Area( *it ); // primer punto 
            conseguirArea(*area, src, esquinas,distancia, rec);
            
            conj->agregar(*area);
        }
    }

    cout<<endl<<"areas : "<<conj->size()<<endl;
    conj->conjuntoAImagen(res, res);
}

// metodos para conseguir fuentes 
vector<Punto> intensidadColores( Mat &res ,bool mitad)
{
    
    MatIterator_< Vec3b > it, end;
    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );

    int max0 = 0;
    int max1= 0;
    int max2 = 0;

    int min0 = 255;
    int min1= 255;
    int min2 = 255;

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        if ( (*it)[0] > max0)// azul
        {
            max0  = (*it)[0];
        }
        if ( (*it)[1] > max1)// verde
        {
            max1 = (*it)[1];
        }
        if ( (*it)[2] > max2)// rojo
        {
            max2  = (*it)[2];
        }

        if ( (*it)[0] < min0)// azul
        {
            min0  = (*it)[0];
        }
        if ( (*it)[1] < min1)// verde
        {
            min1 = (*it)[1];
        }
        if ( (*it)[2] < min2)// rojo
        {
            min2  = (*it)[2];
        }
    }

    vector<Punto> fuentes;

    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );
    int pos = 0;
    bool max;
    bool min;
    bool mid = false;
    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        max = (*it)[0] == max0 || (*it)[1] == max1 || (*it)[2] == max2;
        min = (*it)[0] == min0 || (*it)[1] == min1 || (*it)[2] == min2;
        mid = mitad && (*it)[0] == (max0+min0)/2 || (*it)[1] == (max1+min1)/2 || (*it)[2] == (max2+min2)/2;
        
        if ( max || min || mid )
        {
            fuentes.push_back( Punto( pos % res.cols ,pos / res.cols, *it) );
        }
        pos++;
    }

    return fuentes;


}

// retorna todos los valores 
vector<Punto> todos( Mat &res )
{
    MatIterator_< Vec3b > it, end;
    vector<Punto> fuentes;

    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );
    int pos = 0;
    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        fuentes.push_back( Punto( pos % res.cols ,pos / res.cols, *it) );
        pos++;
    }

    return fuentes;    
}

// esto se me hace horrible, pero no entiendo bien como funcionan estas funciones
// por lo que no puedo quitar las variables globales
vector<Punto> fuentes;
Mat markerMask, immg;
Point prevPt(-1, -1);
static void onMouse( int event, int x, int y, int flags, void* )
{
    Vec3b col = (0,0,0);
    if( x < 0 || x >= immg.cols || y < 0 || y >= immg.rows )
        return;
    if( event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON) )
        prevPt = Point(-1,-1);
    else if( event == EVENT_LBUTTONDOWN )
        prevPt = Point(x,y);
    else if( event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) )
    {
        Point pt(x, y);
        if( prevPt.x < 0 )
            prevPt = pt;

        fuentes.push_back( Punto( x, y, immg.at<Vec3b>( Point( x, y) ) ) );

        line( markerMask, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        line( immg, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        prevPt = pt;
        imshow("image", immg);
    }
}

vector<Punto> manual(Mat & img)
{
    immg = img.clone();
    namedWindow( "image", WINDOW_NORMAL );

    imshow( "image", img );
    setMouseCallback( "image", onMouse, 0 );

    waitKey(0);
    return fuentes;
}