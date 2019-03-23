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



#include "itkMatrixOffsetTransformBase.h"
#include "itkRigid2DTransform.hxx"

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

// -------------------------------------------------------------------------

// modifica el tamaño de la imagen img con los () valx para x y valy para y
TColorImage::Pointer rescale(TColorImage* img, float valx , float valy);

// guarda la imagen img con el nombre nom a travez de writer
int guardarImg(TWriter::Pointer writer, TColorImage::Pointer img, std::string nom);

// crea un canal en base a la imagen img
TColorImage::Pointer crearCanal(TColorImage* img,TRGBPixel col);


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
  TColorImage* img = reader->GetOutput( );

  itk::itkRigid2DTransform()


  
  guardarImg( writer, compImg   , basename + "itk2drigid.png"   );

  return( 0 );
}

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

TColorImage::Pointer rescale(TColorImage* img, float valx , float valy)
{
  TColorImage::SizeType inputSize = img->GetLargestPossibleRegion( ).GetSize( );

  std::cout << "Image input size: " << inputSize << std::endl;

  // Calculate output size and scaling
  TColorImage::SizeType outputSize;
  outputSize[ 0 ] = inputSize[ 0 ] * valx;
  outputSize[ 1 ] = inputSize[ 1 ] * valy;

  std::cout << "Image output size: " << outputSize << std::endl;

  TColorImage::SpacingType outputSpacing;
  outputSpacing[ 0 ] = 
    img->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSize[ 0 ] ) / static_cast< double >( outputSize[ 0 ] ) );
  outputSpacing[ 1 ] = 
    img->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSize[ 1 ] ) / static_cast< double >( outputSize[ 1 ] ) );
  
  // Rescale image
  ResampleImageFilterType::Pointer resampleFilter = ResampleImageFilterType::New( );
  resampleFilter->SetTransform( TransformType::New( ) );
  resampleFilter->SetInput( img );
  resampleFilter->SetSize( outputSize );
  resampleFilter->SetOutputSpacing( outputSpacing );
  resampleFilter->UpdateLargestPossibleRegion( );

  return resampleFilter->GetOutput( );
}

int guardarImg(TWriter::Pointer writer, TColorImage::Pointer img, std::string nom)
{
  writer->SetInput( img );
  writer->SetFileName( nom );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );
  }

  return 0;
}
