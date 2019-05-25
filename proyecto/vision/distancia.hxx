
#include <iostream>
#include <vector>
#include <deque>
#include <stdio.h>
#include <cmath>


#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"

//using namespace cv;
//using namespace std;


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
            
            // return distMin;

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

        float distanciasDeBorde( Punto pt )
        {
            
            int distMax = 0; // distancia maxima al borde
            int distAct = 0; // distancia hasta el punto mirado actualmente 
            bool primero = true;
            
            for( auto bd = borde->begin() ; bd != borde->end(); ++bd)
            {
                //distAct = distanciaEuclidiana
                distAct = sqrt(pow((pt.x - bd->x), 2) + pow((pt.y - bd->y), 2));

                if ( primero ) 
                {
                    distMax = distAct;
                    primero = false;
                }
                else
                {
                    if ( distMax < distAct)
                    {
                        distMax = distAct;
                    }
                }
            }
            return distMax;
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
            // cout<<endl<<"areas : "<<this->size()<<endl;
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
		  
        // void conjuntoADistancias( Mat &dest )
        // {
        //     dest = cv::Scalar(0,0,0);
        //     cout<< this-> tamx << "   " << this->tamy << endl;
            
        //     for (auto area = areas->begin(); area != areas->end(); ++area)
        //     {
        //         area->areaADistancia( dest );
        //     }
        // }

        vector<float> conjuntoADistancias( Mat &dest )
        {
			float promedio = 0;

            vector<float> distancias;

            // for (auto area = areas->begin(); area != areas->end(); ++area)
			// {
			// 	promedio += area->size();
			// }
			// promedio /= this->size();

			// cout << " promedio " << promedio << endl;

            dest = cv::Scalar(0,0,0);
            // cout<< this-> tamx << "   " << this->tamy << endl;
            
            for (auto area = areas->begin(); area != areas->end(); ++area)
            {
				//if ( area->size( ) >= promedio )
				if ( area->size( ) > 2 )
				{
					// float val = area->areaADistancia( dest ) ;
                    distancias.push_back( area->areaADistancia( dest ) ) ;

					// if ( val != 0 ) 
					// {
					// 	cout << val << endl;
					// }
				}
            }
            return distancias;
        }

        void conjuntoAImagen(Mat &img, Mat &res)
        {
            res = img.clone();
            res.create( img.size(), img.type() );
            res = cv::Scalar(0,0,0);
            for (auto area = areas->begin(); area != areas->end(); ++area)
            {
                area->areaAImagen( res, this->size() );
            }
        }
};

