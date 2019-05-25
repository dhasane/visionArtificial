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
    public:
        Clasificacion( float varMax );
        bool insertarPertenencia( float i, float it, float ft,float f );
        bool insertarPertenencia( float i, float it, float ft,float f, int cant );
        void imprimir();
        bool clasificar( float val );
        void guardar( std::string nombre );
        void cargar( std::string nombre );
};

// pos crea el asunto 
Clasificacion::Clasificacion( float varMax )
{
    this->varMax = varMax ;
}


// compara con los otros valores, en caso de no existir un valido, se crea una nueva pertenencia 
bool Clasificacion::clasificar( float val )
{
    // std::cout << this->pert.size() << std::endl;
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

            if ( ff == -1 )
            {
                this->insertarPertenencia( val-varMax , val, val , val+varMax ) ; 
            }
            else 
            {
                this->pert[ff].agregar( val );
            }
        }
        else
        {
            this->insertarPertenencia( val-varMax , val, val , val+varMax ) ; 
        }
    }
    
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

// B = borde S = superior I = inferior
// se lee cada linea guardada como BMinI BMinS BMaxS BMaxI 
void Clasificacion::cargar( std::string nombre )
{
    std::string line;
    std::ifstream myfile (nombre);
    char * temp = new char[50];
    char *token;
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
