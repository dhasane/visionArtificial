#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

Mat backProjection(Mat src, int bins);

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
    
    
    // 65 es un muy buen valor
    int bins = 65;
    Mat res = backProjection(src, bins);

    string metodo;
    std::stringstream ss( argv[ 1 ] );
    std::string basename;
    getline( ss, basename, '.' );

    imwrite(basename+"bp.jpg", res);

    return 0;
}

Mat backProjection(Mat src, int bins)
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
    Mat backproj;
    calcBackProject( &hue, 1, 0, hist, backproj, &ranges, 1, true );
    imwrite("back.jpg", backproj );
    return backproj.clone();
}
