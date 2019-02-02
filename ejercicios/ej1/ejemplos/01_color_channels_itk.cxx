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

#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkImageRegionIteratorWithIndex.h>

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

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  // Get command line arguments
  if( argc < 2 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " image_file" << std::endl;
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
  TColorImage* img = reader->GetOutput( );

  // Create color channel images
  // red channel
  TColorImage::Pointer rImg = TColorImage::New( );
  rImg->SetSpacing( img->GetSpacing( ) );
  rImg->SetOrigin( img->GetOrigin( ) );
  rImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  rImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  rImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  rImg->Allocate( );

  // green channel
  TColorImage::Pointer gImg = TColorImage::New( );
  gImg->SetSpacing( img->GetSpacing( ) );
  gImg->SetOrigin( img->GetOrigin( ) );
  gImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  gImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  gImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  gImg->Allocate( );

  // blue channel
  TColorImage::Pointer bImg = TColorImage::New( );
  bImg->SetSpacing( img->GetSpacing( ) );
  bImg->SetOrigin( img->GetOrigin( ) );
  bImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  bImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  bImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  bImg->Allocate( );

  // composite image (RGB)
  TColorImage::Pointer rgbImg = TColorImage::New( );
  rgbImg->SetSpacing( img->GetSpacing( ) );
  rgbImg->SetOrigin( img->GetOrigin( ) );
  rgbImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  rgbImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  rgbImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  rgbImg->Allocate( );

  // Initialize created images in black
  TRGBPixel black;
  black.SetRed( 0 );
  black.SetGreen( 0 );
  black.SetBlue( 0 );
  rImg->FillBuffer( black );
  gImg->FillBuffer( black );
  bImg->FillBuffer( black );
  rgbImg->FillBuffer( black );

  // Fill color channel images
  TIterator it( img, img->GetLargestPossibleRegion( ) );
  TColorIterator crIt( rImg, rImg->GetLargestPossibleRegion( ) );
  TColorIterator cgIt( gImg, gImg->GetLargestPossibleRegion( ) );
  TColorIterator cbIt( bImg, bImg->GetLargestPossibleRegion( ) );

  it.GoToBegin( );
  crIt.GoToBegin( );
  cgIt.GoToBegin( );
  cbIt.GoToBegin( );
  for( ; !it.IsAtEnd( ) && !crIt.IsAtEnd( ) && !cgIt.IsAtEnd( ) && !cbIt.IsAtEnd( ); ++it, ++crIt, ++cgIt, ++cbIt )
  {
    TRGBPixel value, pixel;
    pixel = it.Get( );
    value.SetRed( pixel.GetRed( ) );
    value.SetGreen( 0 );
    value.SetBlue( 0 );
    crIt.Set( value );

    value.SetRed( 0 );
    value.SetGreen( pixel.GetGreen( ) );
    value.SetBlue( 0 );
    cgIt.Set( value );

    value.SetRed( 0 );
    value.SetGreen( 0 );
    value.SetBlue( pixel.GetBlue( ) );
    cbIt.Set( value );

  } // rof
  
  // From color channel images, reconstruct the original color image
  TColorIterator rgbIt( rgbImg, rgbImg->GetLargestPossibleRegion( ) );

  rgbIt.GoToBegin( );
  crIt.GoToBegin( );
  cgIt.GoToBegin( );
  cbIt.GoToBegin( );
  for( ; !rgbIt.IsAtEnd( ) && !crIt.IsAtEnd( ) && !cgIt.IsAtEnd( ) && !cbIt.IsAtEnd( ); ++rgbIt, ++crIt, ++cgIt, ++cbIt )
  {
    TRGBPixel value, pixel;
    value = crIt.Get( );
    pixel.SetRed( value.GetRed( ) );

    value = cgIt.Get( );
    pixel.SetGreen( value.GetGreen( ) );

    value = cbIt.Get( );
    pixel.SetBlue( value.GetBlue( ) );

    rgbIt.Set( pixel );

  } // rof

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  TWriter::Pointer writer = TWriter::New( );
  writer->SetInput( rImg );
  writer->SetFileName( basename + "_R.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( gImg );
  writer->SetFileName( basename + "_G.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( bImg );
  writer->SetFileName( basename + "_B.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( rgbImg );
  writer->SetFileName( basename + "_RGB.png" );
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

// eof - 01_color_channels.cxx
