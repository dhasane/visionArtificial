#ifndef CLASIFCPP
#define CLASIFCPP

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <string>
#include <list>
#include <cstring>

#include "Pertenencia.cpp"

class Clasificacion
{
    private:
        std::vector<Pertenencia> pert ;
        float varMax ; 
        bool entrenamiento;
    public:
        Clasificacion( float varMax, bool entrenamiento );
        bool insertarPertenencia( float i, float it, float ft,float f );
        bool insertarPertenencia( float i, float it, float ft,float f, int cant );
        void imprimir();
        bool clasificar( float val );
        void guardar( std::string nombre );
        void cargar( std::string nombre );
        void clean( );
        int size( );
        std::vector<float> clasificarLista( std::vector<float> valores ) ; 
};

// pos crea el asunto 
Clasificacion::Clasificacion( float varMax, bool entrenamiento )
{
    this->varMax = varMax ;
    this->entrenamiento = entrenamiento;
}

int Clasificacion::size()
{
    return this->pert.size();
}

// compara con los otros valores, en caso de no existir un valido, se crea una nueva pertenencia 
bool Clasificacion::clasificar( float val )
{
    // std::cout << this->pert.size() << std::endl;
    bool valido = false;
    if( val != 0) // evitar formas vacias 
    {
        if ( this->pert.size() > 0 )
        {
            float ff = -1 ;
            float max = 0; 
            float eval;
            for (int a = 0; a < this->pert.size() ; a++)
            {
                eval = this->pert[a].evaluar(val);

                if ( max < eval )
                {
                    ff = a ;
                    max = eval;
                }
            }

            if ( ff == -1 && entrenamiento )
            {
                this->insertarPertenencia( val-varMax , val, val , val+varMax ) ; 
                valido = true;
            }
            else if ( entrenamiento )
            {
                this->pert[ff].agregar( val );
                valido = true;
            }
            else if( !entrenamiento && ff != -1 )
            {
                valido = true;
            }
        }
        else if ( entrenamiento )
        {
            this->insertarPertenencia( val-varMax , val, val , val+varMax ) ;
            valido = true; 
        }
    }
    return valido;
}

std::vector<float> Clasificacion::clasificarLista( std::vector<float> valores )
{
    std::vector<float> evals;
    
    for (int a = 0; a < this->pert.size() ; a++)
    {
        float val = 0;

        for (int v = 0; v < valores.size() ; v++)
        {
            val += this->pert[a].evaluarDe0( valores[ v ] ) ;
        }

        evals.push_back( val ) ;
    }

    return evals;
}

void Clasificacion::guardar( std::string nombre )
{
    std::ofstream myfile;
    myfile.open (nombre);
    for (int a = 0; a < this->pert.size() ; a++)
    {
        myfile<<" "<< this->pert[a].limites() << ", " << this->pert[a].getCantidad() <<"\n";
    }
    myfile.close();
}

// eliminar las pertenencias con menor cantidad de apariciones que el umbral de apariciones
void Clasificacion::clean( )
{
    float umbral = 0 ; 
    int metodo = 0 ; 

    // por promedio 
    // if ( metodo = 0 )
    // {
        for (int a = 0; a < this->pert.size() ; a++)
        {
            umbral += this->pert[a].getCantidad();
        }

        // std::cout << umbral << "\n";

        umbral /= this->pert.size();

        // std::cout << umbral << "\n";
    // }
    // else if ( metodo = 1 ) // 
    // {
    //     int max = 0 ; 
    //     int act;
    //     for (int a = 0; a < this->pert.size() ; a++)
    //     {
    //         act = this->pert[a].getCantidad();
    //         if ( act > max )
    //         {
    //             max = act;
    //         }
    //     }


    // }
    




    for (int a = 0; a < this->pert.size() ; a++)
    {
        if ( this->pert[a].getCantidad() < umbral )
        {
            this->pert.erase( this->pert.begin( ) + a );
            a--;
        }
    }

}

// B = borde S = superior I = inferior
// se lee cada linea guardada como BMinI BMinS BMaxS BMaxI 
void Clasificacion::cargar( std::string nombre )
{
    std::string line;
    std::ifstream myfile (nombre);
    char * temp = new char[50];
    char * token ;
    int pos;
    float BMinI, BMinS, BMaxS, BMaxI;

    int cantidad;

    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            strcpy( temp,line.c_str() );
            token = std::strtok(temp, ", ");
            pos = 0;

            while (token != NULL) {
                
                if (pos == 0 )
                {
                    BMinI = atof(token);
                }
                else if (pos == 1 )
                {
                    BMinS = atof(token);
                }
                else if (pos == 2 )
                {
                    BMaxS= atof(token);
                }
                else if (pos == 3 )
                {
                    BMaxI= atof(token);
                }
                else if (pos == 4 )
                {
                    cantidad= atoi(token);
                }
                token = std::strtok(NULL, ", ");
                pos ++;
            }
            this->insertarPertenencia( BMinI, BMinS, BMaxS, BMaxI, cantidad );
        }
        myfile.close();
    }
    else std::cout << "Unable to open file"; 
}

// crea e inserta una nueva Pertenencia
// retorna true, al haber insertado la Pertenencia
bool Clasificacion::insertarPertenencia( float i, float it, float ft,float f )
{
    this->pert.push_back( Pertenencia( i, it, ft, f ) );
    return true;
}

bool Clasificacion::insertarPertenencia( float i, float it, float ft,float f, int cant )
{
    this->pert.push_back( Pertenencia( i, it, ft, f, cant ) );
    return true;
}

// imprimir sigue imprimiendo :V
void Clasificacion::imprimir()
{
    for (int a = 0; a < this->pert.size() ; a++)
    {
        std::cout << " (" << this->pert[a].limites() << ") " << this->pert[a].getCantidad() ;
        std::cout<<"\n";
    }
}

#endif
