
#include <iostream>
#include <string>

#include "Clasificacion.cpp"

using namespace std;
main()
{
    Clasificacion clasif( 2 );
    clasif.cargar( "prueba" );
    clasif.clasificar( 7.6 );

    cout << " - - - - - - - " << endl;
    clasif.clasificar( 8.4 );

    cout << " - - - - - - - " << endl;
    clasif.clasificar( 9.59 );

    cout << " - - - - - - - " << endl;
    clasif.clasificar( 3.2 );

    cout << " - - - - - - - " << endl;


    

    clasif.guardar( "prueba" );

    
    clasif.imprimir( );
}