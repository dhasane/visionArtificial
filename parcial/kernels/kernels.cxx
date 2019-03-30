#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstring>


using namespace cv;

using namespace std;

int main ( int argc, char** argv )
{

    if( argc < 2 )
    {
      cout<<" ingresar : "<<argv[0]<<" (nombre imagen)"<<endl;
      return -1;
    }
    Mat src, dst, dst2;
    const char* imageName = argv[1];

    // Loads an image
    src = imread( imageName, IMREAD_COLOR ); // Load an image
    if( src.empty() )
    {
        printf(" Error opening image\n");
        return -1;
    }
    Point anchor = Point( -1, -1 );
    double delta = 0;
    int ddepth = -1;
    
    
    float val = 0.3333;

    Mat_<float> kernel(3,3);
    kernel << -val,  1, -val,
              -val,  1, -val,
              -val,  1, -val;
    
    Mat_<float> kernel2(3,3);
    kernel2 << -val, -val, -val,
                 0,    0,    0,
               -val, -val, -val;

    

    cout<<kernel<<endl<<kernel2<<endl;

    filter2D(src, dst , ddepth , kernel , anchor, delta, BORDER_DEFAULT );
    filter2D(src, dst2, ddepth , kernel2, anchor, delta, BORDER_DEFAULT );

    imwrite("img1.jpg", dst );
    imwrite("img2.jpg", dst2 );
    
    return 0;
}
