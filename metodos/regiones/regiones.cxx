
#include <iostream>
#include <vector>
#include <deque>
#include <stdio.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

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

            vinf = (total0+total1+total2)/3 - cercania;
            vsup = (total0+total1+total2)/3 + cercania;
            if (vinf < 0)   vinf = 0;
            if (255 < vsup) vsup = 255;
            bool bo3 = vinf < ((p.color[0]+p.color[1]+p.color[2])/3) && ((p.color[0]+p.color[1]+p.color[2])/3) < vsup;


            if ( bo0 && bo1 && bo2 || bo3)
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

        void conjuntoAImagen(Mat &img)
        {
            for (auto area = areas->begin(); area != areas->end(); ++area)
            {
                area->areaAImagen(img);
            }
        }

        /*
        void unir()
        {
            struct {
                bool operator()(Area a , Area b) const
                {
                    return a.colorPromedio() < b.colorPromedio();
                }
            } customLess;
            std::sort(areas->begin(), areas->end(), customLess);
            //std::sort(areas->begin(), areas->end()); // sort simple
        }//*/
};

void regiones(Mat &src, Mat &res, bool esquinas, int distancia, vector<Punto> fuentes);
vector<Punto> intensidadColores( Mat &res );
vector<Punto> semillasManual(Mat &res);
void imprimirListaPuntos(vector<Punto> fuentes);

vector<Punto> todos( Mat &res );

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
    vector<Punto> fuentes;
    fuentes = todos ( dest );

    fuentes = semillasManual(dest);
    cout<<"tamaño imagen : "<< src.cols<< " , "<<src.rows<<endl<< "total pixeles : "<<src.cols * src.rows<<endl;
    cout<<"fuentes : "<<fuentes.size()<<endl;

    regiones( dest, dest, true, 100, fuentes);


    //imwrite( basename + + ".jpg" , prueba(src) );
    imwrite( "hola.jpg" , dest );


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
    conj->conjuntoAImagen(res);
}

// metodos para conseguir fuentes
vector<Punto> intensidadColores( Mat &res )
{

    MatIterator_< Vec3b > it, end;
    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );

    int maxAzul = 0;
    int maxVerde= 0;
    int maxRojo = 0;

    int minAzul = 255;
    int minVerde= 255;
    int minRojo = 255;

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

        if ( (*it)[0] < minAzul)// azul
        {
            minAzul  = (*it)[0];
        }
        if ( (*it)[1] < minVerde)// verde
        {
            minVerde = (*it)[1];
        }
        if ( (*it)[2] < minRojo)// rojo
        {
            minRojo  = (*it)[2];
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
        max = (*it)[0] == maxAzul || (*it)[1] == maxVerde || (*it)[2] == maxRojo;
        min = (*it)[0] == minAzul || (*it)[1] == minVerde || (*it)[2] == minRojo;
        if ( max || min )
        {
            fuentes.push_back( Punto( pos % res.cols ,pos / res.cols, *it) );
        }
        pos++;
    }

    cout<<maxAzul<<"  "<<maxVerde<<"  "<<maxRojo<<"  "<<endl;

    return fuentes;


}


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

vector<Punto> semillasManual(Mat &res)
{
  vector<Punto> semillas;
  int cantidadSemillas=0, x=0, y=0;
  cout<<"Cantidad semillas"<<endl;
  cin>>cantidadSemillas;
  for(int i=0; i<cantidadSemillas; i++){
      x=0;
      y=0;
      cout<<"Semilla "<<i+1<<endl;
      cout<<"Ingrese coordenada en x"<<endl;
      cin>>x;
      cout<<"Ingrese coordenada en y"<<endl;
      cin>>y;
      Punto semilla = Punto(x,y, res.at<Vec3b>(x, y));
      cout<<"crea semilla"<<endl;
      semillas.push_back(semilla);
  }
  return semillas;
}
