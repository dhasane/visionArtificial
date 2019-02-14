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

typedef itk::ImageRegionConstIteratorWithIndex< TColorImage > TIterator;
typedef itk::ImageRegionIteratorWithIndex< TColorImage > TColorIterator;

// Types definition
typedef itk::ImageFileReader< TColorImage > TReader;
typedef itk::RGBToLuminanceImageFilter< TColorImage, TImage > LuminanceFilterType;
typedef itk::ShiftScaleImageFilter< TImage, TImage > RescaleFilterType;
typedef itk::RescaleIntensityImageFilter< TImage, TImage > Rescale2FilterType;
typedef itk::ImageFileWriter< TImage > TWriter;


TColorImage::Pointer crearCanal(TColorImage* img, TRGBPixel col);
int guardarImg(TWriter::Pointer writer, TColorImage::Pointer* img, std::string nom);
TColorImage::Pointer* aByN(TColorImage::Pointer* Img,int res);

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
  
  TRGBPixel black;
  black.SetRed( 0 );
  black.SetGreen( 0 );
  black.SetBlue( 0 );

  TColorImage* img = reader->GetOutput( );

  
  // creacion de canales
  TColorImage::Pointer rImg = crearCanal(reader->GetOutput( ),black); // rojo
  TColorImage::Pointer gImg  = crearCanal(reader->GetOutput( ),black); // verde 
  TColorImage::Pointer bImg  = crearCanal(reader->GetOutput( ),black); // azul

  TIterator        it(   img,         img->GetLargestPossibleRegion( ) );
  TColorIterator crIt(   rImg,       rImg->GetLargestPossibleRegion( ) );
  TColorIterator cgIt(   gImg,       gImg->GetLargestPossibleRegion( ) );
  TColorIterator cbIt(   bImg,       bImg->GetLargestPossibleRegion( ) );

  it.GoToBegin( );
  crIt.GoToBegin( );
  cgIt.GoToBegin( );
  cbIt.GoToBegin( );
  for( ; !it.IsAtEnd( ) && !crIt.IsAtEnd( ) && !cgIt.IsAtEnd( ) && !cbIt.IsAtEnd( ); ++it, ++crIt, ++cgIt, ++cbIt )
  {
    TRGBPixel pixel;
    pixel = it.Get( );

    crIt.Set( pixel.GetRed( ) );
    cgIt.Set( pixel.GetGreen( ) );
    cbIt.Set( pixel.GetBlue( ) );
  }//*/


  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  
  
  TWriter::Pointer writer = TWriter::New( );

  int res = atoi (argv[2]);

  // Convert RGB image to grayscale
  LuminanceFilterType::Pointer luminanceFilter = LuminanceFilterType::New();
  luminanceFilter->SetInput( rImg );

  // Quantize into desired number of bits
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New( );
  rescaleFilter->SetInput( luminanceFilter->GetOutput( ) );
  rescaleFilter->SetScale( pow( 2, res) / 256.0 );

  // Rescale intensities to cover the full range
  Rescale2FilterType::Pointer rescale2Filter = Rescale2FilterType::New( );
  rescale2Filter->SetInput( rescaleFilter->GetOutput() );
  rescale2Filter->SetOutputMinimum( 0 );
  rescale2Filter->SetOutputMaximum( 255 );
  writer->SetInput( luminanceFilter->GetOutput( ) );
  writer->SetFileName( basename + "r.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt



  luminanceFilter = LuminanceFilterType::New();
  luminanceFilter->SetInput( gImg );

  // Quantize into desired number of bits
  rescaleFilter = RescaleFilterType::New( );
  rescaleFilter->SetInput( luminanceFilter->GetOutput( ) );
  rescaleFilter->SetScale( pow( 2, res) / 256.0 );

  // Rescale intensities to cover the full range
  rescale2Filter = Rescale2FilterType::New( );
  rescale2Filter->SetInput( rescaleFilter->GetOutput() );
  rescale2Filter->SetOutputMinimum( 0 );
  rescale2Filter->SetOutputMaximum( 255 );
  writer->SetInput( luminanceFilter->GetOutput( ) );
  writer->SetFileName( basename + "g.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt


  luminanceFilter = LuminanceFilterType::New();
  luminanceFilter->SetInput( bImg );

  // Quantize into desired number of bits
  rescaleFilter = RescaleFilterType::New( );
  rescaleFilter->SetInput( luminanceFilter->GetOutput( ) );
  rescaleFilter->SetScale( pow( 2, res) / 256.0 );

  // Rescale intensities to cover the full range
  rescale2Filter = Rescale2FilterType::New( );
  rescale2Filter->SetInput( rescaleFilter->GetOutput() );
  rescale2Filter->SetOutputMinimum( 0 );
  rescale2Filter->SetOutputMaximum( 255 );
  writer->SetInput( luminanceFilter->GetOutput( ) );
  writer->SetFileName( basename + "b.png" );
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

  

  //guardarImg( writer, rImg   , basename + "_r.png"   );
  //guardarImg( writer, gImg   , basename + "_g.png"   );
  //guardarImg( writer, bImg   , basename + "_b.png"   );

  
  
}

// eof - 03_rescale_intensity.cxx


TColorImage::Pointer crearCanal(TColorImage* img, TRGBPixel col)
{
  TColorImage::Pointer compImg = TColorImage::New( );
  compImg->SetSpacing( img->GetSpacing( ) );
  compImg->SetOrigin( img->GetOrigin( ) );
  compImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  compImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  compImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  compImg->Allocate( );

  // Initialize created images in black
  
  compImg->FillBuffer( col );

  return compImg;
}

