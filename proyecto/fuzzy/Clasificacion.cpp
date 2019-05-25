#ifndef CLASIFCPP
#define CLASIFCPP

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "Pertenencia.cpp"

class Clasificacion
{
    private:
        std::vector<Pertenencia> pert;
    public:
        Clasificacion( );
        bool insertarPertenencia( float i, float it, float ft,float f );
        void imprimir();
        float fuzzy(float val);
        float centro(std::string val);
};

// pos crea el asunto 
Clasificacion::Clasificacion()
{
    // aqui seria chevere cargar los datos que ya se tienen desde un archivo

    // para al final guardar todo otra vez en un archivo :v 
}

// crea e inserta una nueva Pertenencia
// retorna true, al haber insertado la Pertenencia
bool Clasificacion::insertarPertenencia( float i, float it, float ft,float f )
{
    Pertenencia npert( i, it, ft, f );
    this->pert.push_back(npert);
    return true;
}



// imprimir sigue imprimiendo :V
void Clasificacion::imprimir()
{
    for (int a = 0; a < this->pert.size() ; a++)
    {
        std::cout<<" "<<this->pert[a].limites();
        std::cout<<"\n";
    }
}

// fuzzyfica los datos
// debe retornar lista de floats/ por ahora retorna el centro de masa supuesto e imprime los datos
float Clasificacion::fuzzy(float val)
{
    float eval = 0;
    float cm = 0; // centro de masa

    for (int a = 0; a < this->pert.size() ; a++)
    {
        eval = this->pert[a].evaluar(val);

        // centro de masa
        float cct = (this->pert[a].centro())*eval;

        //std::cout<<" :"<<cct<<"\n";
        cm += cct;
        std::cout<<" "<<eval <<"\n";

        //std::cout<<"\n";
    }

    std::cout << "centro de masa supuesto en : "<< cm<< std::endl;

    return cm;
}

#endif
