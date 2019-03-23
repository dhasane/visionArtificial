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
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>



#include "itkRigid2DTransform.h"



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


const     unsigned int   Dimension = 2;
typedef   unsigned char  PixelType;
typedef   itk::Image< PixelType, Dimension > ImageType;

// Types definition
typedef itk::IdentityTransform<double, 2> TransformType;
typedef itk::ResampleImageFilter< TColorImage, TColorImage > ResampleImageFilterType;

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

  TRGBPixel black, white;
  black.SetRed( 0 );
  black.SetGreen( 0 );
  black.SetBlue( 0 );

  white.SetRed( 255 );
  white.SetGreen( 255 );
  white.SetBlue( 255 );


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
  ImageType::ConstPointer inpt = reader->GetOutput();

  TColorImage::Pointer img = TColorImage::New( );
  img->SetSpacing( inpt->GetSpacing( ) );
  img->SetOrigin( inpt->GetOrigin( ) );
  img->SetLargestPossibleRegion( inpt->GetLargestPossibleRegion( ) );
  img->SetRequestedRegion( inpt->GetRequestedRegion( ) );
  img->SetBufferedRegion( inpt->GetBufferedRegion( ) );
  img->Allocate( );

  typedef itk::Rigid2DTransform< double > Rigid2DTransformType;

  Rigid2DTransformType::Pointer transform = Rigid2DTransformType::New();
  transform->SetIdentity();


  typedef itk::ResampleImageFilter<ImageType, ImageType, double >    ResampleFilterType;
  ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New();
  resampleFilter->SetInput( img->GetOutput( ) );
  resampleFilter->SetTransform( transform );
  resampleFilter->SetSize( img->GetOutput( )->GetLargestPossibleRegion().GetSize() );
  resampleFilter->SetOutputOrigin(  img->GetOrigin() );
  resampleFilter->SetOutputSpacing( img->GetSpacing() );
  resampleFilter->SetOutputDirection( img->GetDirection() );
  resampleFilter->SetDefaultPixelValue( 200 );

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  TWriter::Pointer writer = TWriter::New( );


  writer->SetInput( resampleFilter->GetOutput() );
  writer->SetFileName( basename + "itk2drigid.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );
  }

  return( 0 );
}


