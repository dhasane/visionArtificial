#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;
int main ( int argc, char** argv )
{
    // Declare variables
    Mat src, dst,dst2;
    Mat kernel;
    Point anchor;
    double delta;
    int ddepth;
    int kernel_size;
    const char* window_name = "filter2D Demo";

    const char* imageName = argc >=2 ? argv[1]: "../512x512-Gaussian-Noise.jpg";
    // Loads an image
    src = imread( imageName, IMREAD_COLOR ); // Load an image
    if( src.empty() )
    {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default ../512x512-Gaussian-Noise.jpg] \n");
        return -1;
    }
    // Initialize arguments for the filter
    anchor = Point( -1, -1 );
    delta = 0;
    ddepth = -1;
    // Loop - Will filter the image with different kernel sizes each 0.5 seconds
    
    kernel_size = 3 ;
    kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
    medianBlur(src,dst2,3);
    for ( int a = 0 ; a < 4 ; a ++)
    {
        filter2D(dst, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
	    medianBlur(dst2,dst2,3);
    }
    imwrite("filter2d-3.jpg", dst);
    imwrite("mb-3.jpg",dst2);
    
    kernel_size = 9;
    filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
    imwrite("filter2d-7.jpg",dst);
    
    kernel_size = 11;
    kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    filter2D(src, dst, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
    medianBlur(src,dst2,11);
    imwrite("mb-11.jpg",dst2);
    imwrite("filter2d-11.jpg", dst);
    return 0;
}
