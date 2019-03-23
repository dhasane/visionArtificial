
#include "itkPoint.h"
#include "itkAzimuthElevationToCartesianTransform.h"


// Image type: 2-dimensional 1-byte rgb
const unsigned int Dim = 2;
typedef unsigned char                   TRGBResolution;
typedef itk::RGBPixel< TRGBResolution > TRGBPixel;
typedef itk::Image< TRGBPixel, Dim >    TColorImage;

// Types definition
typedef itk::ImageFileReader< TColorImage > TReader;
typedef itk::ImageRegionConstIteratorWithIndex< TColorImage > TIterator;
typedef itk::ImageRegionIteratorWithIndex< TColorImage > TColorIterator;
typedef itk::ImageFileWriter< TColorImage > TWriter;

// Types definition
typedef itk::IdentityTransform<double, 2> TransformType;
typedef itk::ResampleImageFilter< TColorImage, TColorImage > ResampleImageFilterType;
// guarda la imagen img con el nombre nom a traves de writer
int guardarImg(TWriter::Pointer writer, TColorImage::Pointer img, std::string nom);

int main( int argc, char* argv[] )
{
  // Get command line arguments
  if( argc < 3 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " val1 val2 val3 " << std::endl;
    return( -1 );

  } // fi

  // Review given command line arguments
  std::cout << "-------------------------" << std::endl;
  for( int a = 0; a < argc; a++ )
    std::cout << argv[ a ] << std::endl;
  std::cout << "-------------------------" << std::endl;

  typedef itk::Point<double, 3> PointType;
  PointType spherical;
  spherical[0] = int(argv[1]);
  spherical[1] = int(argv[2]); // set elevation to 45 degrees
  spherical[2] = int(argv[3]);
  std::cout << "spherical: " << spherical << std::endl;

  typedef itk::AzimuthElevationToCartesianTransform< double, 3 >
    AzimuthElevationToCartesian;
  AzimuthElevationToCartesian::Pointer azimuthElevation =
    AzimuthElevationToCartesian::New();

  std::cout << "Cartesian: " << azimuthElevation->TransformAzElToCartesian(spherical) << std::endl;

  

  return( 0 );
}
