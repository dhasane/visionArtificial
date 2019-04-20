#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <cstdio>
#include <iostream>
using namespace cv;
using namespace std;

Mat markerMask, img;
Point prevPt(-1, -1);

static void onMouse( int event, int x, int y, int flags, void* )
{
    if( x < 0 || x >= img.cols || y < 0 || y >= img.rows )
        return;
    if( event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON) )
        prevPt = Point(-1,-1);
    else if( event == EVENT_LBUTTONDOWN )
        prevPt = Point(x,y);
    else if( event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON) )
    {
        Point pt(x, y);
        if( prevPt.x < 0 )
            prevPt = pt;
        line( markerMask, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        line( img, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        prevPt = pt;
        imshow("image", img);
    }
}

void extremos(Mat &img)
{
    int maxAzul;
    int minAzul;

    int maxVerde;
    int minVerde;

    int maxRojo;
    int minRojo;

    MatIterator_< Vec3b > it, end;
    it  = img.begin< Vec3b >( );
    end = img.end< Vec3b >( );

    bool prim = true;

    for(  ; it != end; ++it) 
    {
        if (prim)
        {
            maxAzul = (*it)[0];
            minAzul = (*it)[0];

            maxVerde = (*it)[1];
            minVerde = (*it)[1];

            maxRojo = (*it)[2];
            minRojo = (*it)[2];
            prim = false;

        }
        else
        {
            if ( maxAzul < (*it)[0])
            {
                maxAzul = (*it)[0];
            }
            if ( minAzul > (*it)[0])
            {
                minAzul = (*it)[0];
            }

            if ( maxVerde < (*it)[1])
            {
                maxVerde = (*it)[1];
            }
            if ( minVerde > (*it)[1])
            {
                minVerde = (*it)[1];
            }

            if ( maxRojo < (*it)[2])
            {
                maxRojo = (*it)[2];
            }
            if ( minRojo > (*it)[2])
            {
                minRojo = (*it)[2];
            }
        }
    }
    cout<<"azul : "<<maxAzul<<" | "<<minAzul<<endl;
    cout<<"verde : "<<maxVerde<<" | "<<minVerde<<endl;
    cout<<"rojo : "<<maxRojo<<" | "<<minRojo<<endl;
    
    it  = img.begin< Vec3b >( );
    end = img.end< Vec3b >( );
    for(  ; it != end; ++it) 
    {
        Point pt(x, y);
        if ( maxAzul == (*it)[0] || minAzul == (*it)[0] )
        {
            line( markerMask, prevPt, pt, Scalar::all(255), 5, 8, 0 );
            line( img, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        }

        if ( maxVerde == (*it)[1] || minVerde == (*it)[1] )
        {
            line( markerMask, prevPt, pt, Scalar::all(255), 5, 8, 0 );
            line( img, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        }

        if ( maxRojo == (*it)[2] || minRojo == (*it)[2] )
        {
            line( markerMask, prevPt, pt, Scalar::all(255), 5, 8, 0 );
            line( img, prevPt, pt, Scalar::all(255), 5, 8, 0 );
        }
    }

}

int main( int argc, char** argv )
{
    cv::CommandLineParser parser(argc, argv, "{help h | | }{ @input | ../data/fruits.jpg | }");

    string filename = parser.get<string>("@input");
    Mat img0 = imread(filename, 1), imgGray;
    if( img0.empty() )
    {
        cout << "Couldn'g open image " << filename << ". Usage: watershed <image_name>\n";
        return 0;
    }
    
    namedWindow( "image", 1 );
    img0.copyTo(img);
    cvtColor(img, markerMask, COLOR_BGR2GRAY);
    cvtColor(markerMask, imgGray, COLOR_GRAY2BGR);
    markerMask = Scalar::all(0);
    imshow( "image", img );
    
    extremos(img);
    ///*
    setMouseCallback( "image", onMouse, 0 );
    int c = waitKey(0);

    int i, j, compCount = 0;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(markerMask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

    Mat markers(markerMask.size(), CV_32S);
    markers = Scalar::all(0);
    int idx = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0], compCount++ )
        drawContours(markers, contours, idx, Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);

    vector<Vec3b> colorTab;
    for( i = 0; i < compCount; i++ )
    {
        int b = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int r = theRNG().uniform(0, 255);
        colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    double t = (double)getTickCount();
    watershed( img0, markers );
    t = (double)getTickCount() - t;
    printf( "execution time = %gms\n", t*1000./getTickFrequency() );
    Mat wshed(markers.size(), CV_8UC3);

    // paint the watershed image
    for( i = 0; i < markers.rows; i++ )
        for( j = 0; j < markers.cols; j++ )
        {
            int index = markers.at<int>(i,j);
            if( index == -1 )
                wshed.at<Vec3b>(i,j) = Vec3b(255,255,255);
            else if( index <= 0 || index > compCount )
                wshed.at<Vec3b>(i,j) = Vec3b(0,0,0);
            else
                wshed.at<Vec3b>(i,j) = colorTab[index - 1];
        }
    wshed = wshed*0.5 + imgGray*0.5;
    imwrite( "watah.jpg", wshed );
    //*/
    
    return 0;
}
