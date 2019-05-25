

#ifndef PERTNCPP
#define PERTCPP

#include <iostream>
#include <string>

class Pertenencia
{
    private:
        int cantidad; // cantidad de valores agregados a esta pertenencia

        float ini;  // soft start 
        float iniT; // hard start
        float finT; // hard end
        float fin;  // soft end

    public:
        Pertenencia( float i, float it, float ft,float f );
        Pertenencia( float i, float it, float ft,float f, int cant );
        float evaluar( float eval );
        std::string limites( );
        float centro( );
        bool agregar( float val );
        int getCantidad();
};

// constructor :v
Pertenencia::Pertenencia( float i, float it, float ft,float f )
{
    this->cantidad = 1;

    this->ini = i;
    this->iniT= it;
    this->finT= ft;
    this->fin = f;
}

Pertenencia::Pertenencia( float i, float it, float ft,float f, int cant )
{
    this->cantidad = cant;

    this->ini = i;
    this->iniT= it;
    this->finT= ft;
    this->fin = f;
}

// retorna un string con los valores de la Pertenencia / bordes
std::string Pertenencia::limites()
{
    std::string ret;
    ret = std::to_string(this->ini)+" , "+std::to_string(this->iniT)+" , "+std::to_string(this->finT)+" , "+std::to_string(this->fin);
    return ret;
}

// evalua el porcentaje de pertenencia de un valor dado a Pertenencia
float Pertenencia::evaluar(float eval)
{
    float porc;

    if(eval < this->ini)
    {
        porc = 0;
    }
    else if(eval >= this->ini && eval < this->iniT)
    {
        porc = (eval-this->ini)/(this->iniT-this->ini);
    }
    else if (eval >= this->iniT && eval < this->finT)
    {
        porc = 1;
    }
    else if (eval >= this->finT && eval <= this->fin)
    {
        porc = 1 - (eval-this->finT)/(this->fin-this->finT);
    }
    else if(eval > this->fin)
    {
        porc = 0;
    }

    return porc;
}

// retorna el centro de la Pertenencia 
float Pertenencia::centro()
{
    // pensandolo, hay una forma mas facil, pero ya se quedo asi
    return ((this->fin-this->ini)/ 2) + this->ini;
}

int Pertenencia::getCantidad()
{
    return this->cantidad;
}

// retorna el centro de la Pertenencia 
bool Pertenencia::agregar( float val )
{
    std::cout << "agregado " << val << std::endl;
    // aqui se hace toda la magia graciosa
    // estoy pensando en algo relacionado a la varianza 
    this->cantidad ++ ; 
    return true;
}


#endif
