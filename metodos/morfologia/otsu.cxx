#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;


void var(int azul[] ,int totalAzul,int liminf, int limsup,float & varianzaAzul, float & pesoAzul)
{
    float promedioAzul = 0;
    int tlocal = 0;

    varianzaAzul = 0 ;
    pesoAzul = 0;

    for ( int a = liminf ; a < limsup ; a ++)
    {
        pesoAzul     += azul[a];
        promedioAzul += a*azul[a];
        tlocal       += azul[a];
    }
    pesoAzul /= totalAzul;
    promedioAzul /= tlocal;
    for ( int a = liminf ; a < limsup ; a ++)
    {
        varianzaAzul += pow((a-promedioAzul), 2 ) + azul[a];
    }
    
    if (tlocal != 0)
    {
        varianzaAzul /= tlocal;
    }
    else
    {
        varianzaAzul = 0;
    }
}

/*
void grafHist()
{
    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat hist ; //, g_hist, r_hist;

    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );
    //alcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    //calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    //normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    //normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                        Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                        Scalar( 255, 0, 0), 2, 8, 0  );
        /*line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                        Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                        Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                        Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                        Scalar( 0, 0, 255), 2, 8, 0  );*
    }

    /// Display
    namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
    imshow("calcHist Demo", histImage );

    waitKey(0);


} */

float encontrarUmbral(int color[])
{
    int totalcolor = 0;

    for ( int i  = 0 ; i <= 255 ; i++)
    {
        totalcolor += color[i];
    }
    cout<<totalcolor<<endl;

    float vA = 0;
    float cvA = 0;

    float pA = 0;
    float cpA = 0;

    float minv = 0 ;
    float varmin = 0 ;

    float actv = 0 ;
    float varact = 0 ;

    for ( int corte  = 1 ; corte <= 255 ; corte++)
    {

        var(color,totalcolor, 0, corte, vA, pA);

        var(color,totalcolor, corte+1, 256,cvA,cpA);
        
        
        actv = corte;
        varact = pA*vA + cpA*cvA;

        if ( corte == 1 )
        {
            minv = actv;
            varmin = varact;
        }
        else if ( varact < varmin)
        {
            minv = actv;
            varmin = varact;
        }
    }
    cout<<"umbral optimo en : "<<minv<<" con "<<varmin<<endl;

    return minv;
}

void guardarUmbral()
{

}

int main ( int argc, char** argv )
{
    const char* window_name = "filter2D Demo";

    const char* imageName = argc >=2 ? argv[1]: "../512x512-Gaussian-Noise.jpg";
    // Loads an image
    Mat src = imread( imageName, IMREAD_COLOR ); // Load an image
    if( src.empty() )
    {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default ../512x512-Gaussian-Noise.jpg] \n");
        return -1;
    }
    
    int verde[256] = {0};
    int rojo [256] = {0};
    int azul [256] = {0};
    
    // Fill color channel images
    MatIterator_< Vec3b > it, end;
    it = src.begin< Vec3b >( );
    end = src.end< Vec3b >( );

    for(  ; it != end; ++it)
    {
        azul [(*it)[0]] += 1;
        verde[(*it)[1]] += 1;
        rojo [(*it)[2]] += 1;
    }

    float umbAzul  = encontrarUmbral(azul);

    float umbVerde = encontrarUmbral(verde);

    float umbRojo  = encontrarUmbral(rojo);

    Mat AZ(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));
    Mat RO(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));
    Mat VE(src.rows, src.cols, CV_8UC3, Scalar( 0 , 0 , 0 ));

    it = src.begin< Vec3b >( );
    end = src.end< Vec3b >( );


    MatIterator_< Vec3b > az, ro ,ve, eaz, ero , eve ;

    az = AZ.begin< Vec3b >();
    ro = RO.begin< Vec3b >();
    ve = VE.begin< Vec3b >();


    eaz = AZ.end< Vec3b >();
    ero = RO.end< Vec3b >();
    eve = VE.end< Vec3b >();

    for(  ; it != end, az != eaz, ro != ero, ve != eve; ++it, ++az, ++ro, ++ve)
    {
        if ( (*it)[0] > umbAzul  ) { (*az)[0] = 255; (*it)[0] = 255 ; } // azul
        else                       { (*az)[0] = 0;   (*it)[0] = 0   ; }

        if ( (*it)[1] > umbRojo  ) { (*ve)[1] = 255; (*it)[0] = 255 ; } // verde 
        else                       { (*ve)[1] = 0;   (*it)[0] = 0   ; }

        if ( (*it)[2] > umbVerde ) { (*ro)[2] = 255; (*it)[0] = 255 ; } // rojo 
        else                       { (*ro)[2] = 0;   (*it)[0] = 0   ; }
    }

    imwrite("azul.jpg",  AZ);
    imwrite("rojo.jpg",  RO);
    imwrite("verde.jpg", VE);

    imwrite("union.jpg", src);
    
    return 0;
}
