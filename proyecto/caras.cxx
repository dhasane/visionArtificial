#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void backProjection(Mat src, Mat &dst , int bins);
void otsu(Mat &src, Mat &dst);
void suavisarPromedio( Mat &img, Mat &dst , int kernel_size);
void suavisarExp(Mat img, Mat &dst);

void limpiar(Mat &img,Mat &res, int tipoero, int tipodil, int tam);
void recorrerFondoPlano(Mat &img);

int main( int argc, char* argv[] )
{
    CommandLineParser parser( argc, argv, "{@input |  | input image}" );
    Mat src = imread( parser.get<String>( "@input" ) );
    
    if( src.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }
    
    
    
    Mat forma = src.clone();
    //suavisarPromedio( forma, forma , 9 );
    
    //suavisarExp(forma, forma);
    //GaussianBlur( forma, forma, Size( 7, 7), 0, 0 );//applying Gaussian filter 

    //*
    backProjection  ( forma, forma , 70);
    //limpiar(forma, forma, 0, 0, 1);
    //suavisarExp(forma, forma);
    otsu( forma, forma );

    cvtColor( forma, forma, COLOR_GRAY2BGR);
    //*/


    //recorrerFondoPlano(forma);
    
    // -----------------------------------------

    string metodo;
    std::stringstream ss( argv[ 1 ] );
    std::string basename;
    getline( ss, basename, '.' );

    //*
    Mat res = src + forma ;
    imwrite(basename+"proyecto.jpg", res );
    //*/
    
    imwrite(basename+"proyectoForma.jpg", forma );

    return 0;
}

void recorrerFondoPlano(Mat &img)
{
    MatIterator_< Vec3b > it, end;
    it  = img.begin< Vec3b >( );
    end = img.end  < Vec3b >( );

    bool ini = false ; 

    int val = 0 ;

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        if (!ini)
        {
            val = (*it)[0] + (*it)[1] + (*it)[2];
            ini = true;
        }

        if ( abs(val - ( (*it)[0] + (*it)[1] + (*it)[2])  ) < 10 )
        {
            (*it)[0] = 0;
            (*it)[1] = 0;
            (*it)[2] = 0;
        }
    }
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

/** @function Dilation */
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

// ambas comparten el tamaño
void limpiar(Mat &img,Mat &res, int tipoero, int tipodil, int tam)
{
    erocion(img ,res , tipoero , tam );
    dilatar(res ,res , tipodil , tam );
}

// todo lo de aplicar kernels debe ir de aqui para abajo
void aplicarKernel(Mat img, Mat &dst, Mat kernel)
{
  Point anchor = Point( -1, -1 );
  double delta = 0;
  int ddepth = -1;

  filter2D(img, dst , ddepth , kernel , anchor, delta, BORDER_DEFAULT );
  //cout<<kernel<<endl;
}

void suavisarExp(Mat img, Mat &dst)
{
    Mat_<float> diferencia(3,3);
    int val = 70;
    //*
    diferencia <<   -val,  val, -val,
                     val,    1,  val,
                    -val,  val, -val;
    /*/
    
    diferencia <<   val,  val, val,
                    val, -val, val,
                    val,  val, val;
    diferencia /= val*7;
    //*/
    aplicarKernel( img,dst, diferencia );
}


void suavisarPromedio( Mat &img, Mat &dst , int kernel_size)
{
    Mat_<float> kernel(3,3);
    kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    aplicarKernel(img,dst,kernel);
}

void backProjection(Mat src, Mat &dst , int bins)
{
    Mat hue, hsv;
    cvtColor( src, hsv, COLOR_BGR2HSV );
    hue.create(hsv.size(), hsv.depth());
    int ch[] = { 0, 0 };
    mixChannels( &hsv, 1, &hue, 1, ch, 1 );
    
    int histSize = MAX( bins, 2 );
    float hue_range[] = { 0, 180 };
    const float* ranges = { hue_range };
    Mat hist;
    calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
    //Mat dst;
    calcBackProject( &hue, 1, 0, hist, dst, &ranges, 1, true );
    //imwrite("back.jpg", backproj );
    //return backproj.clone();
}

/// ------------------------------------------- otsu 

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
    //cout<<totalcolor<<endl;

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
    //cout<<"umbral optimo en : "<<minv<<" con "<<varmin<<endl;

    return minv;
}

// solo sirve para un color, esto porque debe estar la imagen en blanco y negro
void otsu(Mat &src, Mat &dst)
{
    //cvtColor( src, dst, COLOR_RGB2GRAY);

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
}
