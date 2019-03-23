#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

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

  cv::Mat tras(2,3,CV_64F, Scalar(0.0));

  tras.at<double>(0,0) = 0.656;
  tras.at<double>(0,1) = 0.456;
  tras.at<double>(0,2) = -10.68;

  tras.at<double>(1,0) = -0.456;
  tras.at<double>(1,1) = 0.656;
  tras.at<double>(1,2) = 43.424;
      
  cv::Mat dest;
  cv::Size size(image.cols,image.rows);
  warpAffine(image, dest, tras, size, INTER_LINEAR, BORDER_CONSTANT); // esto realiza todos los movimientos de golpe, de forma un poco mas limpia 
  
  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  imwrite( basename + "matriz.png", dest );

  cv::Mat esc(2,3,CV_64F, Scalar(0.0)); // escalar 
  esc.at<double>(0,0) = 0.8;
  esc.at<double>(1,1) = 0.8;
  warpAffine(image, dest, esc, size, INTER_LINEAR, BORDER_CONSTANT); 

  cv::Mat mov(2,3,CV_64F, Scalar(0.0)); // mover
  mov.at<double>(0,0) = 1;
  mov.at<double>(0,2) = -42;
  mov.at<double>(1,1) = 1;
  mov.at<double>(1,2) = 37;
  warpAffine(dest, dest, mov, size, INTER_LINEAR, BORDER_CONSTANT);

  cv::Mat rot(2,3,CV_64F, Scalar(0.0)); // rotar
  rot.at<double>(0,0) = 0.8191;
  rot.at<double>(0,1) = 0.5736;
  rot.at<double>(0,2) = 0;
  rot.at<double>(1,0) = -0.5736;
  rot.at<double>(1,1) = 0.8191;
  rot.at<double>(1,2) = 0;
  warpAffine(dest, dest, rot, size, INTER_LINEAR, BORDER_CONSTANT);




  // Write results
  imwrite( basename + "a_pasos.png", dest );

  return( 0 );
}

// eof - 03_rescale_intensity.cxx
