


#ifndef KNNCPP
#define KNNCPP

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <string>
#include <list>
#include <cstring>
#include <math.h> 

struct Datos{
    vector<float> vals;
    int apariciones;
};


class Knn
{
    private:
        std::vector< Datos > datos ;
        float varMax ; // distancia maxima posible entre dos puntos para tener la misma clase 

    public:
        Knn( int varMax );
        void imprimir();
        bool clasificar( vector<float> dato );
        void guardar( std::string nombre );
        void cargar( std::string nombre );
        float euclidiana( vector<float> d1, vector<float> d2 );
};

// pos crea el asunto 
Knn::Knn( int varMax )
{
    this->varMax = varMax ; // esto es para los nuevos vectores agregados, para tener una distancia maxima 
}

// compara con los otros valores, en caso de no existir un valido, se crea una nueva pertenencia 
bool Knn::clasificar( vector<float> dato )
{
    float distMin = 0; 
    float dist;
    int pos;
    bool primero = true;
    bool encontrado = false;
    for (int a = 0; a < this->datos.size() ; a++)
    {
        dist = euclidiana( this->datos[a].vals , dato );

        if ( dist < this->varMax && ( primero || dist < distMin ) )
        {
            distMin = dist;
            pos = a;
            primero = false; 
            encontrado = true;
        }
    }

    if ( !encontrado )
    {
        Datos dt;
        dt.vals = dato;
        dt.apariciones = 1;
        this->datos.push_back( dt );
    }
    else
    {
        this->datos[pos].apariciones ++;
    }
    
}

float Knn::euclidiana( vector<float> d1, vector<float> d2 )
{
    float dist = 0 ; 
    if ( d1.size( ) == d2.size( ) )
    {
        for (int a = 0; a < this->datos.size() ; a++)
            dist += pow ( d1[a] - d2[a] , 2);
        
        sqrt( dist );
    }
    return dist;
}

void Knn::guardar( std::string nombre )
{
    std::ofstream myfile;
    myfile.open (nombre);
    imprimir();

    std::cout  << this->datos.size() << "  ";

    for (int a = 0; a < this->datos.size() ; a++)
    {
        myfile << this->datos[a].apariciones ;
        
        for (int b = 0; b < this->datos[a].vals.size() ; b++)
        {
            myfile << ", " << this->datos[a].vals[b];
        }
        myfile <<"\n";
    }

    myfile.close();
}

// B = borde S = superior I = inferior
// se lee cada linea guardada como BMinI BMinS BMaxS BMaxI 
void Knn::cargar( std::string nombre )
{
    std::string line;
    std::ifstream myfile (nombre);
    char * temp = new char[50];
    char *token;
    int pos;

    int cantidad;

    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            strcpy( temp,line.c_str() );
            token = std::strtok(temp, ", ");
            pos = 0;

            Datos dt;

            dt.apariciones = atoi( token );
            
            // std::cout << ap << " | " << std::endl;
            token = std::strtok(NULL, ", ");

            while (token != NULL) {
                
                // std::cout <<  atof( token ) << " " << std::endl;

                dt.vals.push_back( atof( token ) );
                
                token = std::strtok(NULL, ", ");
                pos ++;
            }
            
            this->datos.push_back( dt );
        }
        myfile.close();
    }
    else std::cout << "archivo no encontrado"; 

    
}

// imprimir sigue imprimiendo :V
void Knn::imprimir()
{
    for (int a = 0; a < this->datos.size() ; a++)
    {
        for( auto bd = this->datos[a].vals.begin() ; bd != this->datos[a].vals.end(); ++bd)
        {
            std::cout << *bd << " ";
        }
        std::cout << " | " << this->datos[a].apariciones << endl;
    }
}

#endif
