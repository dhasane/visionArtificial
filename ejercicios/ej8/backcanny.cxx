#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
Mat hue;
int bins = 25;
void Hist_and_Backproj(int, void* );
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
    createTrackbar("* Hue  bins: ", window_image, &bins, 180, Hist_and_Backproj );
    Hist_and_Backproj(0, 0);
    imshow( window_image, src );


    // sobel
  
    // First we declare the variables we are going to use
    Mat src_gray;
    Mat grad;
    const String window_name = "Sobel Demo - Simple Edge Detector";
    
    
    int ksize = 1;
    int scale = 1;
    int delta = 1;
    int ddepth = CV_16S;

    
    
    for (;;)
    {
        // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
        GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
        // Convert the image to grayscale
        cvtColor(src, src_gray, COLOR_BGR2GRAY);
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
        Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
        // converting back to CV_8U
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
        imshow(window_name, grad);
        char key = (char)waitKey(0);
        if(key == 27)
        {
        return 0;
        }
        if (key == 'k' || key == 'K')
        {
        ksize = ksize < 30 ? ksize+2 : -1;
        }
        if (key == 's' || key == 'S')
        {
        scale++;
        }
        if (key == 'd' || key == 'D')
        {
        delta++;
        }
        if (key == 'r' || key == 'R')
        {
        scale =  1;
        ksize = -1;
        delta =  0;
        }
    }


    // Wait until user exits the program
    waitKey();
    return 0;
}
void Hist_and_Backproj(int, void* )
{
    int histSize = MAX( bins, 2 );
    float hue_range[] = { 0, 180 };
    const float* ranges = { hue_range };
    Mat hist;
    calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
    Mat backproj;
    calcBackProject( &hue, 1, 0, hist, backproj, &ranges, 1, true );
    imshow( "BackProj", backproj );
    imwrite("back.jpg", backproj );

    Mat src, src_gray;
    Mat dst, detected_edges;
    int lowThreshold = 0;
    const int max_lowThreshold = 100;
    const int ratio = 3;
    const int kernel_size = 3;
    const char* window_name = "Edge Map";
    
    int w = 400, h = 400;
    int bin_w = cvRound( (double) w / histSize );
    Mat histImg = Mat::zeros( h, w, CV_8UC3 );
    for (int i = 0; i < bins; i++)
    {
        rectangle( histImg, Point( i*bin_w, h ), Point( (i+1)*bin_w, h - cvRound( hist.at<float>(i)*h/255.0 ) ),
                   Scalar( 0, 0, 255 ), FILLED );
    }
    imshow( "Histogram", histImg );
}