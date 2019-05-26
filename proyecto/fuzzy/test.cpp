
#include <iostream>
#include <string>

#include "Clasificacion.cpp"

using namespace std;
main()
{
    Clasificacion clasif( 2, false );
    clasif.cargar( "valores1" );


    clasif.imprimir( );


    // clasif.clasificar( 7.6 );

    // cout << " - - - - - - - " << endl;
    // clasif.clasificar( 8.4 );

    // cout << " - - - - - - - " << endl;
    // clasif.clasificar( 9.59 );

    // cout << " - - - - - - - " << endl;
    // clasif.clasificar( 3.2 );

    // cout << " - - - - - - - " << endl;

    clasif.clean( );
    // clasif.clean( );
    clasif.clean( );
    

    clasif.guardar( "pruebainternet" );

    
}