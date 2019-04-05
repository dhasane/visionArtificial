#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void backProjection(Mat src, Mat &dst , int bins);
void otsu(Mat &src, Mat &dst);
void suavisarGauss( Mat &img, Mat &dst , int kernel_size);

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
    suavisarGauss ( forma, forma , 9 );
    backProjection( forma, forma , 80);

    otsu( forma, forma );

    Mat dst;
    cvtColor( src, dst, COLOR_RGB2GRAY);

    Mat res = dst - forma ;
    // -----------------------------------------

    string metodo;
    std::stringstream ss( argv[ 1 ] );
    std::string basename;
    getline( ss, basename, '.' );

    imwrite(basename+"proyecto.jpg", res );
    imwrite(basename+"proyecto2.jpg", forma );

    return 0;
}

// todo lo de aplicar kernels debe ir de aqui para abajo
Mat aplicarKernel(Mat img, Mat kernel)
{
  cv::Mat dst;

  Point anchor = Point( -1, -1 );
  double delta = 0;
  int ddepth = -1;

  filter2D(img, dst , ddepth , kernel , anchor, delta, BORDER_DEFAULT );
  cout<<kernel<<endl;
  return dst.clone();
}

void suavisarGauss( Mat &img, Mat &dst , int kernel_size)
{
    Mat_<float> gauss(3,3);
    //gauss  << 1, 1, 1,
    //          1, 1, 1,
    //          1, 1, 1;
    //gauss /= 9;

    //kernel_size = 3 ;
    gauss = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    
    dst = aplicarKernel(img,gauss);
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
