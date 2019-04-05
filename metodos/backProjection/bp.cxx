#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
Mat hue;
int bins = 25;
void Backproj(int, void* );
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
    Mat hsv;
    cvtColor( src, hsv, COLOR_BGR2HSV );
    hue.create(hsv.size(), hsv.depth());
    int ch[] = { 0, 0 };
    mixChannels( &hsv, 1, &hue, 1, ch, 1 );
    const char* window_image = "Source image";
    namedWindow( window_image );
    createTrackbar("* Hue  bins: ", window_image, &bins, 180, Backproj );


    
    Backproj(0, 0);
    imshow( window_image, src );
    // 65 es un muy buen valor

    // Wait until user exits the program
    waitKey();
    return 0;
}
void Backproj(int, void* )
{
    int histSize = MAX( bins, 2 );
    float hue_range[] = { 0, 180 };
    const float* ranges = { hue_range };
    Mat hist;
    calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
    Mat backproj;
    calcBackProject( &hue, 1, 0, hist, backproj, &ranges, 1, true );
    imwrite("back.jpg", backproj );
}