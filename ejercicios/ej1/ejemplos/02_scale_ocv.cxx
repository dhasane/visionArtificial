#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
  // Get command line arguments
  if( argc < 4 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " image_file x_factor y_factor" << std::endl;
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

  std::cout << "Image input size: " << image.size() << std::endl;

  // Rescale image
  Mat res_img;
  resize(image, res_img, Size(), atof( argv[ 2 ] ), atof( argv[ 3 ] ), INTER_LINEAR);

  std::cout << "Image output size: " << res_img.size() << std::endl;

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  imwrite( basename + "_scaled.png", res_img );

  return( 0 );
}

// eof - 02_scale.cxx
