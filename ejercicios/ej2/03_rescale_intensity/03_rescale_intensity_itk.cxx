#include <cmath>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <string>
#include <sstream>

#include <itkImage.h>
#include <itkRGBPixel.h>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkShiftScaleImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkRGBToLuminanceImageFilter.h>

// Output image type: 2-dimensional 1-byte grayscale
const unsigned int Dim = 2;
typedef unsigned char             TPixel;
typedef itk::Image< TPixel, Dim > TImage;
// Input image type: 2-dimensional 1-byte rgb
typedef unsigned char                   TRGBResolution;
typedef itk::RGBPixel< TRGBResolution > TRGBPixel;
typedef itk::Image< TRGBPixel, Dim >    TColorImage;

// Types definition
typedef itk::ImageFileReader< TColorImage > TReader;
typedef itk::RGBToLuminanceImageFilter< TColorImage, TImage > LuminanceFilterType;
typedef itk::ShiftScaleImageFilter< TImage, TImage > RescaleFilterType;
typedef itk::RescaleIntensityImageFilter< TImage, TImage > Rescale2FilterType;
typedef itk::ImageFileWriter< TImage > TWriter;

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  // Get command line arguments
  if( argc < 3 )
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
  TReader::Pointer reader = TReader::New( );
  reader->SetFileName( argv[ 1 ] );
  try
  {
    reader->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  
  // Convert RGB image to grayscale
  LuminanceFilterType::Pointer luminanceFilter = LuminanceFilterType::New();
  luminanceFilter->SetInput( reader->GetOutput( ) );

  // Quantize into desired number of bits
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New( );
  rescaleFilter->SetInput( luminanceFilter->GetOutput( ) );
  rescaleFilter->SetScale( pow( 2, atoi( argv[ 2 ] ) ) / 256.0 );

  // Rescale intensities to cover the full range
  Rescale2FilterType::Pointer rescale2Filter = Rescale2FilterType::New( );
  rescale2Filter->SetInput( rescaleFilter->GetOutput() );
  rescale2Filter->SetOutputMinimum( 0 );
  rescale2Filter->SetOutputMaximum( 255 );

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  TWriter::Pointer writer = TWriter::New( );
  writer->SetInput( luminanceFilter->GetOutput( ) );
  writer->SetFileName( basename + "_intensity.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( rescale2Filter->GetOutput( ) );
  writer->SetFileName( basename + "_rescaleInt.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  return( 0 );
}

// eof - 03_rescale_intensity.cxx
