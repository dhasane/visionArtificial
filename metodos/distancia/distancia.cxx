
#include <iostream>
#include <vector>
#include <deque>
#include <stdio.h>
#include <cmath>


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
	vector<Punto> *borde;   // puntos del borde
    int id;
    Punto *centroPromedio;

    public:
        
        Area( Punto p , int id )
        {
            this->id = id;
            puntos = new vector<Punto>();
            posibles = new deque<Punto>();
            puntos->push_back(p);
			borde = new vector<Punto>();

            centroPromedio=NULL;
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
            if ( p.color == BLANCO )
            {
                puntos->push_back( p );
                
                return true;
            }
				else
				{
					 borde->push_back( p );
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

        float distanciaDeBorde( Punto pt )
        {
            int distMin = 0; // distancia minima al borde
            int distMax = 0; // distancia maxima al borde
            int distAct = 0; // distancia hasta el punto mirado actualmente 
            bool primero = true;
            
            for( auto bd = borde->begin() ; bd != borde->end(); ++bd)
            {
                    distAct = sqrt(pow((pt.x - bd->x), 2) + pow((pt.y - bd->y), 2));

                    if ( primero ) 
                    {
                        distMin = distAct;
                        distMax = distAct;
                        primero = false;
                    }
                    else
                    {
                        if ( distMax < distAct)
                        {
                            distMax = distAct;
                        }
                        if ( distAct < distMin )
                        {
                            distMin = distAct;
                        }
                    }

            }
            // esto podria ser mejor cambiarlo a que divida por la distancia desde el centro
            
			if ( distMin == 0 || distMax == 0 )
			{
				return 0;
			}

			float val = distMin;
            val /= distMax;
				
            return val ;
        }
		  
        // promedio del area -> consigue el centro del area 
        void getCentro( )
        {
            float px;
            float py;
            for (auto pt = puntos->begin(); pt != puntos->end(); ++pt)
            {
                px += pt->x ; 
                py += pt->y ;
            }
            px /= puntos->size();
            py /= puntos->size();
            
            this->centroPromedio = new Punto( px , py , (0,0,0 ) );
        }
		        
		float areaADistancia( Mat &img )
        {
            getCentro();
            float dst;
            int idst;

			float forma = 0;

            if (centroPromedio != NULL)
            {
				cout << this->id << "   " << this->size() << endl;
                for (auto pt = puntos->begin(); pt != puntos->end(); ++pt)
                {
                    dst = distanciaDeBorde( *pt ) ;
                    dst *= 255;
                    idst = int(dst);
                    img.at<Vec3b>( Point( pt->x, pt->y ) ) = Vec3b( idst,idst,idst ) ;

					dst /= puntos->size();
                    forma += dst;
                }
                //img.at<Vec3b>( Point( centroPromedio->x, centroPromedio->y) ) = Vec3b( 0 ,0 ,255);
            	
			}
			
			return forma;
        }
};

class Conjunto{
    vector<Area> *areas;
	 
	 int tamx;
	 int tamy;

    public:
        Conjunto(Mat &src , bool esquinas)
        {
			   this->tamx = src.cols;
				this->tamy = src.rows;

            areas = new vector<Area>();
            Recorrido rec(src.cols,src.rows);
            
            Area* area;
            MatIterator_< Vec3b > it;
            
            int pos = 0;
            int x, y;
            for( it  = src.begin< Vec3b >( ) ; it != src.end< Vec3b >( ); ++it) 
            {
                x = pos % src.cols;
                y = pos / src.cols;
                if ( (*it) == NEGRO )
                {
                    rec.verPos( x, y );
                }
                else if ( !rec.visto( x, y) && (*it) == BLANCO) 
                {
                    rec.verPos( x, y );
                    area = new Area( Punto( x, y, *it ), this->size() + 1 );  
                    conseguirArea( *area, src, esquinas, rec );
                    this->agregar( *area );
                }
                pos++;
            }
            cout<<endl<<"areas : "<<this->size()<<endl;
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

        void agregar(Area area)
        {
            if ( area.size() > 0)
            {
                areas->push_back( area );
            }
            
        }

        int size()
        {
            return areas->size();
        }
		  
        void conjuntoADistancias( Mat &dest )
        {
            dest = cv::Scalar(0,0,0);
            cout<< this-> tamx << "   " << this->tamy << endl;
            
            for (auto area = areas->begin(); area != areas->end(); ++area)
            {
				if ( area->size( ) > 2 )
				{
					float val = area->areaADistancia( dest ) ;

					if ( val != 0 ) 
					{
						cout << val << endl;
					}
				}
            }
        }

        void conjuntoAImagen(Mat &img, Mat &res)
        {
            res = img.clone();
            res = cv::Scalar(0,0,0);
            for (auto area = areas->begin(); area != areas->end(); ++area)
            {
                area->areaAImagen( res, this->size() );
            }
        }
};

void binarizar(Mat & dest, Mat & hola , int umbral, int tope, int base );

void otsu(Mat &src, Mat &dst);

void limpiar(Mat &img,Mat &res, int tipo , int tam);

int main ( int argc, char** argv )
{
    if( argc < 4 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen) ( esquinas (0-1) ) ( tamaño minimo ) "<<endl;
      return -1;
    }
    const char* imageName =      argv[1];
    int         esquinas  = atoi(argv[2]);
    int         tam       = atoi(argv[3]);

    if ( esquinas != 0 && esquinas != 1 )
    {
        printf("esquinas debe ser 1 o 0\n");
        return -1;
    }

    if ( tam <= 0 )
    {
        printf(" el tamaño del area minima debe ser mayor a 0\n");
        return -1;
    }

    Mat src;
    src = imread( imageName , IMREAD_COLOR ); 
	//src = imread( imageName , IMREAD_GRAYSCALE);
    if( src.empty() )
    {
        printf("error al abrir la imagen\n");
        return -1;
    }
 

    cout<<"tamaño imagen : "<< src.cols<< " , "<<src.rows<<endl<< "total pixeles : "<<src.cols * src.rows<<endl;
    
    //cvtColor( src, src, COLOR_BGR2GRAY );
	binarizar( src , src , 70, 0 , 255);
    //otsu( src , src );
    
	imwrite( "byn.jpg" , src );

	limpiar(src, src , 2 , tam );

	imwrite( "bynlimpia.jpg" , src );

	//return 0;
	

    Conjunto conj( src, (bool)esquinas );
	
	Mat img1;
    conj.conjuntoAImagen( src, img1 );
	
	imwrite( "prueba.jpg" , img1 ) ;

	
    cout <<endl << " distancias " << endl << endl;
    Mat res = src.clone();
    
    conj.conjuntoADistancias( res );
	
	imwrite( "dist.jpg" , res ) ; 

	return 0;
	/*
	cout << " holaaa " << endl;
    std::string basename = "";
    
    std::stringstream ss( argv[ 1 ] );
    getline( ss, basename, '.' );

    imwrite( basename + "Areas.jpg" , dest );

    imwrite( basename + "Distancias.jpg" , res ) ;
	// */
    return 0;
}

void binarizar(Mat & dest, Mat & hola , int umbral, int tope, int base )
{
    cvtColor( dest, hola, COLOR_BGR2GRAY );
    MatIterator_< Vec3b > it, end;
    it  = hola.begin< Vec3b >( );
    end = hola.end< Vec3b >( );
    for(  ; it != end; ++it)
    {
        if( (*it)[0] < umbral ) { (*it)[0] = tope; }
        else                    { (*it)[0] = base; }
    }
    cvtColor( hola, hola, COLOR_GRAY2BGR );
}

// encuentra la varianza para un color y su peso respectivo 
void var(int color[] ,int total,int liminf, int limsup,float & varianza, float & peso)
{
    float promedio = 0;
    int tlocal     = 0;

    varianza = 0 ;
    peso     = 0;

    for ( int a = liminf ; a < limsup ; a ++)
    {
        peso     += color[a];
        promedio += a*color[a];
        tlocal   += color[a];
    }
    peso     /= total;
    promedio /= tlocal;
    for ( int a = liminf ; a < limsup ; a ++)
    {
        varianza += pow((a-promedio), 2 ) + color[a];
    }
    
    if (tlocal != 0)
    {
        varianza /= tlocal;
    }
    else
    {
        varianza  = 0;
    }
}

// encuentra el umbral optimo, a traves del metodo de otsu 
float encontrarUmbral(int color[])
{
    int totalcolor = 0;

    for ( int i  = 0 ; i <= 255 ; i++)
    {
        totalcolor += color[i];
    }

    float vA  = 0;
    float cvA = 0;

    float pA  = 0;
    float cpA = 0;

    float minv   = 0 ;
    float varmin = 0 ;

    float actv   = 0 ;
    float varact = 0 ;

    for ( int corte  = 1 ; corte <= 255 ; corte++)
    {
        var(color,totalcolor, 0, corte, vA, pA);

        var(color,totalcolor, corte+1, 256,cvA,cpA);
        
        actv   = corte;
        varact = pA*vA + cpA*cvA;

        if ( corte == 1 )
        {
            minv   = actv;
            varmin = varact;
        }
        else if ( varact < varmin)
        {
            minv   = actv;
            varmin = varact;
        }
    }
    return minv;
}

void erocion( Mat &img, Mat &src, int erosion_elem, int erosion_size )
{
    int erosion_type;
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

    Mat element = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size ) );

    erode( img, src, element );
}

void dilatar( Mat &img,Mat &src, int dilation_elem, int dilation_size )
{
  int dilation_type;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  dilate( img , src, element );
}

void limpiar(Mat &img,Mat &res, int tipo , int tam)
{
    erocion(img ,res , tipo , tam );
    dilatar(res ,res , tipo , tam );
}

// solo sirve para un color, esto porque debe estar la imagen en blanco y negro
void otsu(Mat &src, Mat &dst)
{
    cvtColor( src, dst, COLOR_RGB2GRAY);

    // vectores de aparicion de intensidades 
    int azul [256] = {0};
    
    MatIterator_< Vec3b > it, end;
    it  = dst.begin< Vec3b >( );
    end = dst.end< Vec3b >( );

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        azul [(*it)[0]] += 1;
    }

    // se encuentran los umbrales 
    float umbral  = encontrarUmbral(azul);

    // se inicializan los iteradores 
    it  = dst.begin< Vec3b >( );
    end = dst.end< Vec3b >( );

    // se recorre la imagen original y en caso de una intensidad ser menor al umbral especifico, se guarda 0, de lo contrario 255 
    for(  ; it != end; ++it)
    {
        if ( (*it)[0] > umbral  ) { (*it)[0] = 255 ; } // azul
        else                      { (*it)[0] = 0   ; }

        if ( (*it)[1] > umbral  ) { (*it)[0] = 255 ; } // verde 
        else                      { (*it)[0] = 0   ; }

        if ( (*it)[2] > umbral )  { (*it)[0] = 255 ; } // rojo 
        else                      { (*it)[0] = 0   ; }
    }

    cvtColor( dst, dst, COLOR_GRAY2RGB);
}
