#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

void CannyThreshold(Mat &src, Mat &dst, int lowThreshold )
{
    //int const max_lowThreshold = 100;
    int ratio = 3;
    int kernel_size = 3;

    Mat src_gray;
    Mat detected_edges;
    /// Convert the image to grayscale
    cvtColor( src, src_gray, CV_BGR2GRAY );

    /// Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, Size(3,3) );

    /// Canny detector
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

    /// Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);

    src.copyTo( dst, detected_edges);
    //imshow( window_name, dst );
    imwrite( "prueba.jpg" , dst );
}


int main( int argc, char** argv )
{
    Mat src, dst;
    /// Load an image
    src = imread( argv[1] );

    if( !src.data )
    { return -1; }
    /// Create a matrix of the same type and size as src (for dst)
    dst.create( src.size(), src.type() );
    CannyThreshold( src, dst, 25 );
    return 0;
}
