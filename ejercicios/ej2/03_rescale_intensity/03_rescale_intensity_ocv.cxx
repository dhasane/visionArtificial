#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
  // Get command line arguments
  if ( argc < 3 )
  {
  	std::cerr << "Usage: " << argv[ 0 ] << " image_file number_of_bits" << std::endl;
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

  // Convert RGB image to grayscale
  Mat gray_image;
  cvtColor( image, gray_image, CV_BGR2GRAY );

  // Quantize into desired number of bits
  uchar table[ 256 ];
  int scale = pow( 2, 8 - atoi( argv[ 2 ] ) );
  for (int i = 0; i < 256; ++i)
    table[ i ] = ( uchar )( scale * ( i / scale ) );

  Mat resint_img = Mat::zeros( image.size( ), CV_8UC1 );
  MatIterator_< uchar > it, end;
  for( it = resint_img.begin< uchar >( ), end = resint_img.end< uchar >( ); it != end; ++it)
    *it = table[*it];

  // Rescale intensities to cover the full range
  Mat lookUpTable(1, 256, CV_8U);
  uchar* p = lookUpTable.data;
  for( int i = 0; i < 256; ++i)
    p[i] = table[i];
  LUT(gray_image, lookUpTable, resint_img);

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  imwrite( basename + "_intensity.png", gray_image );
  imwrite( basename + "_rescaleInt.png", resint_img );

  return( 0 );
}

// eof - 03_rescale_intensity.cxx
