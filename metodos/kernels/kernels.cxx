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
    Mat src, dst;
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
    
    
    float val = 0.11111;

    Mat_<float> kernel(3,3);
    kernel << val, val, val,
              val, val, val,
              val, val, val;

    cout<<kernel<<endl;

    filter2D(src, dst , ddepth , kernel , anchor, delta, BORDER_DEFAULT );

    imwrite("img1.jpg", dst );
    
    return 0;
}
