
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
        
    private:
        
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
            //*
            if ( mat[y][x] != 0 )
            {
                return true;
            }//*/
            return false;
        }

        // imprime :v 
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


    float total0;
    float total1; // para cada uno de los canales
    float total2;

    int cant;

    

    public:
        
        Area( Punto p )
        {
            puntos = new vector<Punto>();
            posibles = new deque<Punto>();
            puntos->push_back(p);
            total0 = p.color[0];
            total1 = p.color[1]; // full lazy 
            total2 = p.color[2];
            cant = 1;
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
            //cout<<size()<<endl;

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

            if ( bo0 && bo1 && bo1 )
            {
                puntos->push_back( p );
                
                // medir promedios
                this->total0 += p.color[0];
                this->total1 += p.color[1];
                this->total2 += p.color[2];
                cant ++;
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

        void imprimirListaPuntos()
        {
            for (auto it = puntos->begin(); it != puntos->end(); ++it)
                it->imprimir();
        }

        Vec3b colorPromedio()
        {
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
            //res = cv::Scalar(255,255,255);
            res = cv::Scalar(0,0,0);
            for (auto area = areas->begin(); area != areas->end(); ++area)
            {
                area->areaAImagen(res);
            }
        }
};

// corta, pega, une, hace magia y retorna una imagen con las areas 
void regiones(Mat &src, Mat &res, bool esquinas, int distancia, vector<Punto> fuentes);


void imprimirListaPuntos(vector<Punto> fuentes);

// seleccion de fuentes
// toma los valores con max y min intensidad
vector<Punto> intensidadColores( Mat &res );

// todos los pixeles como fuentes -> el mejor hasta el momento
vector<Punto> todos( Mat &res );

// muestra una pantalla, para que el usuario decida fuentes
vector<Punto> manual(Mat & img);

int main ( int argc, char** argv )
{
    if( argc < 2 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen)"<<endl;
      return -1;
    }
    Mat src;
    const char* imageName = argv[1];

    // Loads an image
    src = imread( imageName, IMREAD_COLOR ); 
    if( src.empty() )
    {
        printf(" Error opening image\n");
        return -1;
    }
    
    string metodo;
    std::stringstream ss( argv[ 1 ] );
    std::string basename;
    getline( ss, basename, '.' );

    Mat dest = src.clone();

    //cvtColor( src, dest, COLOR_BGR2GRAY );

    //vector<Punto> fuentes = intensidadColores ( dest );
    //vector<Punto> fuentes = todos ( dest );
    vector<Punto> fuentes = manual ( dest );

    cout<<"tamaño imagen : "<< src.cols<< " , "<<src.rows<<endl<< "total pixeles : "<<src.cols * src.rows<<endl;

    cout<<"fuentes : "<<fuentes.size()<<endl;

    regiones( dest, dest, false, 50, fuentes);

    /*
    imwrite( basename + "hola.jpg" , dest );
    /*/
    imwrite( "hola.jpg" , dest );
    //*/
    
    return 0;
}

// imprime una lista de puntos 
void imprimirListaPuntos(vector<Punto> fuentes)
{
    for (auto it = fuentes.begin(); it != fuentes.end(); ++it)
      it->imprimir();
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

// mira los pixeles adyacentes al que se esta mirando, en caso de que estos sean viables
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
        if( area.insertar( area.posiblesTop() , distancia ) )
        {
            rec.verPos(area.top().x, area.top().y);
            nuevos = new vector<Punto>(); 
            adyacentes(*nuevos, img, area.top().x, area.top().y,esquinas, rec);
            area.agregarPosibles(*nuevos);
        }
        else
        {
            rec.noVer(area.top().x, area.top().y);
        }
    }    
}

// consigue n regiones y afecta la imagen para representarlas 
void regiones(Mat &src, Mat &res, bool esquinas, int distancia, vector<Punto> fuentes)
{
    Recorrido rec(src.cols,src.rows);

    Conjunto *conj = new Conjunto();
    

    res = src.clone();
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

    cout<<endl<<conj->size()<<endl;
    conj->conjuntoAImagen(res, res);
}


// metodos para conseguir fuentes 
vector<Punto> intensidadColores( Mat &res )
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
    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        max = (*it)[0] == max0 || (*it)[1] == max1 || (*it)[2] == max2;
        min = (*it)[0] == min0 || (*it)[1] == min1 || (*it)[2] == min2;
        if ( max || min )
        {
            fuentes.push_back( Punto( pos % res.cols ,pos / res.cols, *it) );
        }
        pos++;
    }

    cout<<max0<<"  "<<max1<<"  "<<max2<<"  "<<endl;

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

        fuentes.push_back( Punto( x, y, col) );

        line( markerMask, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        line( immg, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        prevPt = pt;
        imshow("image", immg);
    }
}

vector<Punto> manual(Mat & img)
{
    immg = img.clone();
    namedWindow( "image", 1 );

    imshow( "image", img );
    setMouseCallback( "image", onMouse, 0 );

    
    int c = waitKey(0);

    /*
    if( (char)c == 'w' || (char)c == ' ' )
    {
        
    }
    */
    return fuentes;
}