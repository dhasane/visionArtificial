
#include <iostream>
#include <vector>
#include <deque>
#include <stdio.h>
#include <cmath>

#include <math.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkForwardFFTImageFilter.h"
#include "itkMaskImageFilter.h"
#include "itkFFTShiftImageFilter.h"
#include "itkInverseFFTImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkMinimumMaximumImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkSignedDanielssonDistanceMapImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itksys/SystemTools.hxx"
#include <sstream>

typedef itk::Image<unsigned char, 2>  UnsignedCharImageType;
typedef itk::Image<float, 2>          FloatImageType;
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

    public:
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

    public:
        
        Area( Punto p , int id )
        {
            this->id = id;
            puntos = new vector<Punto>();
            posibles = new deque<Punto>();
            puntos->push_back(p);
				borde = new vector<Punto>();
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
            //Vec3b bla = Vec3b(255,255,255);
            //int ye =  p.color == bla ? 1 : 0;
            
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
            int vv = 255 *3 / cantidad * (this->id);
            r = vv % 256;
            vv -= 255;
            if ( vv > 0) g = vv % 256;
            vv -= 255;
            if ( vv > 0) b = vv % 256;
            Vec3b color = Vec3b(r,g,b);
            Mat imgFiltrada = img.clone();
            for (auto pt = puntos->begin(); pt != puntos->end(); ++pt)
            {
                img.at<Vec3b>( Point(pt->x, pt->y) ) = color;

                imgFiltrada.at<Vec3b>( Point(pt->x, pt->y) ) = Vec3b(255,255,255);;
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
					 //distAct = distanciaEuclidiana
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
				float val = distMin;
				val /= distMax;
				//cout << distMin << " " << distMax <<" "<< val <<endl;
				return val;
		  }
		  
		  // promedio del area -> consigue el centro del area 
		  void getCentro()
		  {
              float max = 0;
              int x;
              int y;
			   for (auto pt = puntos->begin(); pt != puntos->end(); ++pt)
            {
                if(distanciaDeBorde(*pt)>max){
                    max = distanciaDeBorde(*pt);
                    x = pt->x;
                    y = pt->y;
                }
            }
             cout<<max<<" "<<x<<" "<<y<<endl;
		  }
		  void areaADistancia( Mat &img )
		  {
			   int dst;
			   for (auto pt = puntos->begin(); pt != puntos->end(); ++pt)
            {
					 dst = int( distanciaDeBorde( *pt ) * 255 );
                img.at<Vec3b>( Point( pt->x, pt->y ) ) = Vec3b( dst,dst,dst) ;
            }
		  }

};

class Conjunto{
    public:
    vector<Area> *areas; 
	int tamx;
	int tamy;
    int maximo = 0;

    public:
        Conjunto(Mat &src , bool esquinas, float filtro)
        {
		    this->tamx = src.cols;
		    this->tamy = src.rows;

            areas = new vector<Area>();
            Recorrido rec(src.cols,src.rows);
            
            Area* area;
            MatIterator_< Vec3b > it;
            
            int pos = 0;
            int x, y;
            int tam=0;
            int i=0;


            it  = src.begin< Vec3b >( );
            cout<<src.cols<<" "<<src.rows<<endl;
            for(int i=0; i<src.rows; i++){
                for(int j=0; j<src.cols; j++){
                   // cout<<j<<" "<<i<<endl;
                    if ( (*it) == NEGRO )
                    {
                    rec.verPos( j, i );
                    }
                else if ( (*it) == BLANCO ) 
                    {
                    rec.verPos( j, i );
                    if(rec.visto(j,i)){
                        rec.noVer(j,i);
                        //cout<<j<<" "<<i<<endl;
                    }
                    area = new Area( Punto( j, i, BLANCO ), this->size() + 1 );  
                    conseguirArea( *area, src, esquinas, rec);
                    this->agregar( *area );   
                    }
                    it++;
                }

            }


            auto areait = areas->begin();
            int lim = maximo*filtro;
            cout<<"Area maxima = "<<maximo<<endl;
            vector<Area> *nuevasAreas = new vector<Area>(); 
            while(areait != areas->end()){
                if(lim<=areait->size()){
                    Area nArea = (*areait);
                    nuevasAreas->push_back(nArea);
                }
                areait++;
            }

            areas = nuevasAreas;
            areait = areas->begin(); 
            int n=0;
            while(areait != areas->end()){
                Area listaAreas = (*areait);
                cout<<"Area "<<n<<" = "<<listaAreas.size()<<endl;
                n++;
                areait++;
            }

            cout<<endl<<"N areas : "<<this->size()<<endl;
        }


        // verifica si un punto es viable 
        void mirarPunto(vector<Punto> &puntos, Mat &img, int x, int y, Recorrido rec)
        {
            if( rec.interno(x,y) && !rec.visto(x,y))
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

            int tamArea = 0;
            while( area.sizePosibles( ) > 0 )
            {
                Punto act = area.posiblesTop();
                if( area.insertar( act ) )
                {
                    //cout<< area.top().x<<" "<< area.top().y<<endl;
                    rec.verPos(area.top().x, area.top().y);
                    nuevos = new vector<Punto>(); 
                    adyacentes( *nuevos, img, area.top().x, area.top().y, esquinas, rec);
                    area.agregarPosibles(*nuevos);
                    tamArea++;
                }
                else
                {
                    rec.noVer( act.x, act.y);
                }
            } 
            if(maximo < tamArea){
                maximo = tamArea;
            }  
        }



        void agregar(Area area)
        {
            areas->push_back( area );
        }

        int size()
        {
            return areas->size();
        }
		  
		  void conjuntoADistancias( Mat &dest )
		  {
			   //dest = 
				//Mat M(2,2, CV_8UC3, Scalar(0,0,255));
				dest = cv::Scalar(0,0,0);

				cout<< this-> tamx << "   " << this->tamy << endl;
				//cv::resize(dest, dest, cv::Size( this->tamx, this->tamy ) );
		   	
				for (auto area = areas->begin(); area != areas->end(); ++area)
            {
                area->areaADistancia( dest );
            }

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

        int maximoTamArea(){
            return maximo;
        }
};

void binarizar(Mat & dest, Mat & hola , int umbral);
void distancia();
void otsu(Mat &src, Mat &AZ, Mat &VE, Mat &RO);
float encontrarUmbral(int color[]);
void var(int color[] ,int total,int liminf, int limsup,float & varianza, float & peso);


int main ( int argc, char** argv )
{

    if( argc < 4 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen) ( esquinas (0-1) ) (%)"<<endl;
      return -1;
    }
    const char* imageName =      argv[1];
    int         esquinas  = atoi(argv[2]);
    float filtro = atof(argv[3]);
    if ( esquinas != 0 && esquinas != 1 )
    {
        printf("esquinas debe ser 1 o 0\n");
        return -1;
    }

    if ( filtro < 0 || filtro > 1 )
    {
        printf("Filtro debe estar entre 0 y 1\n");
        return -1;
    }

    // se carga la imagen 
    Mat src = imread( imageName, IMREAD_COLOR ); // Load an image
    
    if( src.empty() )
    {
        printf(" Error opening image\n");
        return -1;
    }
    
    Mat matGris;
    cvtColor( src, matGris, COLOR_RGB2GRAY);

    string metodo;
    std::stringstream ss( argv[ 1 ] );
    std::string basename;
    getline( ss, basename, '.' );

    // se les da el mismo tamaño que la imagen original
    Mat AZ(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));
    Mat RO(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));
    Mat VE(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));

    otsu(matGris,AZ,VE,RO);
    imwrite("binaria.png",  matGris); 

    src = imread( "binaria.png", IMREAD_COLOR ); 
    if( src.empty() )
    {
        printf("error al abrir la imagen\n");
        return -1;
    }
    
    Mat dest = src.clone();

    cout<<"tamaño imagen : "<< src.cols<< " , "<<src.rows<<endl<< "total pixeles : "<<src.cols * src.rows<<endl;

    
    Conjunto conj( dest, (bool)esquinas , filtro);
    
    conj.conjuntoAImagen( dest , dest ); 
    cout<<"Pixeles maxima area "<<conj.maximoTamArea()<<endl;
	Mat res = src.clone();
    Mat filtrada = src.clone();
    conj.conjuntoADistancias(res);
    
    vector<Area> *areas = conj.areas; 
    int n=0;
    for (auto area = areas->begin(); area != areas->end(); ++area)
    {
        cout<<n<<" ";
        area->getCentro();
        cout<<endl;
        n++;
    }


    //cout<<conj.*areas.getCentro();

    imwrite( basename + "Areas.jpg" , dest );
    binarizar(dest,filtrada,2);
    imwrite("filtrada.png",  filtrada); 
    //distancia();


	imwrite( basename + "Distancias.jpg" , res ) ;
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


float encontrarUmbral(int color[])
{
    int totalcolor = 0;

    for ( int i  = 0 ; i <= 255 ; i++)
    {
        totalcolor += color[i];
    }
    cout<<totalcolor<<endl;

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
    cout<<"umbral optimo en : "<<minv<<" con "<<varmin<<endl;

    return minv;
}

void otsu(Mat &src, Mat &AZ, Mat &VE, Mat &RO)
{
    // vectores de aparicion de intensidades 
    int verde[256] = {0};
    int rojo [256] = {0};
    int azul [256] = {0};
    
    MatIterator_< Vec3b > it, end;
    it  = src.begin< Vec3b >( );
    end = src.end< Vec3b >( );

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        azul [(*it)[0]] += 1;
        verde[(*it)[1]] += 1;
        rojo [(*it)[2]] += 1;
    }

    //for (int a = 0 ; a < 256 ; a ++)
    //    cout<<azul[a]<<"   "<<rojo[a]<<"   "<<verde[a]<<endl;
    
    // se encuentran los umbrales 

    float umbAzul  = encontrarUmbral(azul);

    float umbVerde = encontrarUmbral(verde);

    float umbRojo  = encontrarUmbral(rojo);


    //cout<<umbAzul<<"   "<<umbRojo<<"   "<<umbVerde<<endl;

    // se crea una nueva imagen en la que se guardaran los valores 
    //Mat AZ(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));
    //Mat RO(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));
    //Mat VE(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));

    // se inicializan los iteradores 
    it = src.begin< Vec3b >( );
    end = src.end< Vec3b >( );

    MatIterator_< Vec3b > az, ro ,ve, eaz, ero , eve ;

    az = AZ.begin< Vec3b >();
    ro = RO.begin< Vec3b >();
    ve = VE.begin< Vec3b >();

    eaz = AZ.end< Vec3b >();
    ero = RO.end< Vec3b >();
    eve = VE.end< Vec3b >();

    // se recorre la imagen original y en caso de una intensidad ser menor al umbral especifico, se guarda 0, de lo contrario 255 
    for(  ; it != end, az != eaz, ro != ero, ve != eve; ++it, ++az, ++ro, ++ve)
    {
        if ( (*it)[0] > umbAzul  ) { (*az)[0] = 255; (*it)[0] = 255 ; } // azul
        else                       { (*az)[0] = 0;   (*it)[0] = 0   ; }

        if ( (*it)[1] > umbRojo  ) { (*ve)[1] = 255; (*it)[0] = 255 ; } // verde 
        else                       { (*ve)[1] = 0;   (*it)[0] = 0   ; }

        if ( (*it)[2] > umbVerde ) { (*ro)[2] = 255; (*it)[0] = 255 ; } // rojo 
        else                       { (*ro)[2] = 0;   (*it)[0] = 0   ; }
    }
}


void distancia(){
   UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();

    typedef itk::ImageFileReader<UnsignedCharImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("binaria.png");
    reader->Update();
    image = reader->GetOutput();

    typedef  itk::SignedMaurerDistanceMapImageFilter< UnsignedCharImageType, FloatImageType  > SignedMaurerDistanceMapImageFilterType;
   SignedMaurerDistanceMapImageFilterType::Pointer distanceMapImageFilter =
     SignedMaurerDistanceMapImageFilterType::New();
   distanceMapImageFilter->SetInput(image);

  using FilterType = itk::CastImageFilter <FloatImageType, UnsignedCharImageType>;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(distanceMapImageFilter->GetOutput());



  using WriterType = itk::ImageFileWriter<UnsignedCharImageType>;
  WriterType::Pointer writer= WriterType::New();
  writer->SetFileName( "resultado1.png" );
  writer->SetInput( filter->GetOutput() );
  try
  {
    writer->Update();
  }
  catch( itk::ExceptionObject & error )
  {
    std::cerr << "Error: " << error << std::endl;
  }
}


