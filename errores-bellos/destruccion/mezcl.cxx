#include <cmath>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <string>
#include <sstream>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */

#include <itkImage.h>
#include <itkRGBPixel.h>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>

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


void mezclar(std::string nom1,std::string nom2,std::string endname);
TColorImage* cargarImg(std::string nombre);

TColorImage::Pointer rescale(TColorImage* img, TColorImage* img2);

TRGBPixel black, white;

int main( int argc, char* argv[] )
{
  
  black.SetRed( 0 );
  black.SetGreen( 0 );
  black.SetBlue( 0 );

  white.SetRed( 255 );
  white.SetGreen( 255 );
  white.SetBlue( 255 );
  // Get command line arguments
  if( argc < 3 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " image_file" << std::endl;
    return( -1 );

  } // fi

  // Review given command line arguments
  /*std::cout << "-------------------------" << std::endl;
  for( int a = 0; a < argc; a++ )
    std::cout << argv[ a ] << std::endl;
  std::cout << "-------------------------" << std::endl;
  */

  std::string nom = argv[3]; 
  
  //TColorImage* img1 = cargarImg(argv[1]);
  //TColorImage* img2 = cargarImg(argv[2]);

  mezclar( argv[1] , argv[2] , nom );
  return( 0 );
}

TColorImage* cargarImg(std::string nombre)
{
  TReader::Pointer reader = TReader::New( );
  reader->SetFileName( nombre );
  try
  {
    reader->Update( );
  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
  } // yrt
  return reader->GetOutput( );
}

void mezclar(std::string nom1,std::string nom2,std::string endname)
{
  


  //std::cout<<nom1<<std::endl;
  TReader::Pointer reader = TReader::New( );
  reader->SetFileName( nom1 );
  try
  {
    reader->Update( );
  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
  } // yrt
  TColorImage* img1 = reader->GetOutput( );

  TColorImage::Pointer compImg  = crearCanal(img1,black); // comparacion

  

  compImg = img1;

  reader = TReader::New( );
  reader->SetFileName( nom2 );
  try
  {
    reader->Update( );
  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
  } // yrt
  TColorImage* img2 = reader->GetOutput( );

  TColorIterator compIt( compImg, compImg->GetLargestPossibleRegion( ) );
  TIterator         it2( img2,       img2->GetLargestPossibleRegion( ) );
  
  //las dos imagenes ya tendran el mismo tamaño, no tengo vida suficiente para ello, puta vida

  it2.GoToBegin( );
  compIt.GoToBegin( );

  srand (time(NULL));

  for( ; !it2.IsAtEnd( ) && !compIt.IsAtEnd( ); ++it2, ++compIt )
  {
    if( rand()%10 > 4.5)
    {
      compIt.Set( it2.Get( ) );
    }
  }
  std::stringstream ss( endname );
  //std::string basename;
  getline( ss, endname, '.' );
  TWriter::Pointer writer = TWriter::New( );  
  //std::cout<<"ola " <<endname<<std::endl;
  guardarImg( writer, compImg   , endname+".png"   );

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

  //std::cout << "Image input size: " << inputSize << std::endl;

  // Calculate output size and scaling
  TColorImage::SizeType outputSize;
  outputSize[ 0 ] = inputSize[ 0 ] * valx;
  outputSize[ 1 ] = inputSize[ 1 ] * valy;

  //std::cout << "Image output size: " << outputSize << std::endl;

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

TColorImage::Pointer rescale(TColorImage* img, TColorImage* img2)
{
  TColorImage::SizeType inputSize = img2->GetLargestPossibleRegion( ).GetSize( );

  //std::cout << "Image input size: " << inputSize << std::endl;

  // Calculate output size and scaling
  TColorImage::SizeType outputSize;
  //outputSize[ 0 ] = img2->GetSize( )[0];
  //outputSize[ 1 ] = img2->GetSize( )[1];

  outputSize[ 0 ] = inputSize[ 0 ] ;
  outputSize[ 1 ] = inputSize[ 1 ] ;

  //std::cout << "Image output size: " << outputSize << std::endl;

  TColorImage::SpacingType outputSpacing;
  outputSpacing[ 0 ] = img2->GetSpacing( )[0];
  //  img->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSize[ 0 ] ) / static_cast< double >( outputSize[ 0 ] ) );
  outputSpacing[ 1 ] = img2->GetSpacing( )[1];
  //  img->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSize[ 1 ] ) / static_cast< double >( outputSize[ 1 ] ) );
  
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
  }

  return 0;
}
