#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
  // Get command line arguments
  if ( argc < 2 )
  {
  	std::cerr << "Usage: " << argv[ 0 ] << " image_file " << std::endl;
    return( -1 );

  } // fi

  // Review given command line arguments
  std::cout << "-------------------------" << std::endl;
  for( int a = 0; a < argc; a++ )
    std::cout << argv[ a ] << std::endl;
  std::cout << "-------------------------" << std::endl;

  // Read an image
  Mat image;
  image = imread( argv[1], 1 );

  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  }

  cv::Size size(image.cols,image.rows);
  
  cv::Mat dest;
  //*
  Mat_<float> kernel(2,3);
  kernel << -1.1905,  0.2667, 24.5728,
            -0.3810, -0.8333, -7.8851;
  
  cout<<kernel<<std::endl;
  /*/
  cv::Mat tras(2,3,CV_64F, Scalar(0.0));

  tras.at<double>(0,0) = -1.1905;
  tras.at<double>(0,1) =  0.2667;
  tras.at<double>(0,2) = 24.5728;

  tras.at<double>(1,0) = -0.3810;
  tras.at<double>(1,1) = -0.8333;
  tras.at<double>(1,2) = -7.8851;
  
  std::cout<<tras<<std::endl;
  warpAffine(image, dest, tras, size, INTER_LINEAR, BORDER_CONSTANT); 
  /*/
  warpAffine(image, dest, kernel, size, INTER_LINEAR, BORDER_CONSTANT); 
  //*/
  imwrite( "resultado.jpg", dest );

  return( 0 );
}

