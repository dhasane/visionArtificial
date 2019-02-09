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
TColorImage::Pointer crearCanal(TColorImage* img);


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

  // creacion de canales
  TColorImage::Pointer rImg = crearCanal(img); // rojo
  TColorImage::Pointer gImg  = crearCanal(img); // verde 
  TColorImage::Pointer bImg  = crearCanal(img); // azul

  // iteradores
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
  }

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  TWriter::Pointer writer = TWriter::New( );

  // de pa bajo
  rImg = rescale(rImg,0.75,0.75);
  gImg = rescale(gImg,0.5,0.5);
  bImg = rescale(bImg,0.25,0.25);

  // de pa rriba
  rImg = rescale(rImg,1.3335,1.3335);
  gImg = rescale(gImg,2,2);
  bImg = rescale(bImg,4,4);

  std::cout << "---------------"<< std::endl;
  
  TColorImage::Pointer rgbImg   = crearCanal(img); // composite image (RGB)
  TColorImage::Pointer compImg  = crearCanal(img); // comparacion

  TColorIterator ccrIt(  rImg,       rImg->GetLargestPossibleRegion( ) );
  TColorIterator ccgIt(  gImg,       gImg->GetLargestPossibleRegion( ) );
  TColorIterator ccbIt(  bImg,       bImg->GetLargestPossibleRegion( ) );
  TColorIterator rgbIt(  rgbImg,   rgbImg->GetLargestPossibleRegion( ) );
  TColorIterator compIt( compImg, compImg->GetLargestPossibleRegion( ) );

  // unificacion a una sola imagen 
  rgbIt.GoToBegin( );
  compIt.GoToBegin( );
  ccrIt.GoToBegin( );
  ccgIt.GoToBegin( );
  ccbIt.GoToBegin( );

  TRGBPixel white;
  white.SetRed( 255 );
  white.SetGreen( 255 );
  white.SetBlue( 255 );

  std::cout<<"cargando imagenes comparacion y rgb"<<std::endl;
  // esta es la parte que no esta funcionando
  for( ; !rgbIt.IsAtEnd( ) && !compIt.IsAtEnd( ) && !ccrIt.IsAtEnd( ) && !ccgIt.IsAtEnd( ) && !ccbIt.IsAtEnd( ); ++rgbIt, ++compIt, ++ccrIt, ++ccgIt, ++ccbIt )
  {
    TRGBPixel valuer, valueg, valueb, pixel, pixel2;

    valuer = ccrIt.Get( );
    pixel.SetRed( valuer.GetRed( ) );

    valueg = ccgIt.Get( );
    pixel.SetGreen( valueg.GetGreen( ) );

    valueb = ccbIt.Get( );
    pixel.SetBlue( valueb.GetBlue( ) );

    


    rgbIt.Set( pixel );

    if(it.Get( ).GetRed() < valuer.GetRed())
    {
      pixel2.SetRed( it.Get( ).GetRed() - valuer.GetRed() );
    }
    else if(it.Get( ).GetRed() > valuer.GetRed())
    {
      pixel2.SetRed( valuer.GetRed() - it.Get( ).GetRed() );
    }

    if(it.Get( ).GetBlue() < valueb.GetBlue())
    {
      pixel2.SetBlue( it.Get( ).GetBlue() - valueb.GetBlue() );
    }
    else if(it.Get( ).GetBlue() > valueb.GetBlue())
    {
      pixel2.SetBlue( valueb.GetBlue() - it.Get( ).GetBlue() );
    }

    if(it.Get( ).GetGreen() < valueg.GetGreen())
    {
      pixel2.SetGreen( it.Get( ).GetGreen() - valueg.GetGreen() );
    }
    else if(it.Get( ).GetGreen() > valueg.GetGreen())
    {
      pixel2.SetGreen( valueg.GetGreen() - it.Get( ).GetGreen() );
    }

    if ( pixel == pixel2 )
    {
      compIt.Set( white );
    }
    else
    {
      pixel2 = pixel2 + pixel;
      compIt.Set( pixel2 );
    }

    // para la comparacion 

    
    
    
    
  }
  std::string extra = ".png";
  //extra = "_err.png";
  guardarImg( writer, compImg   , basename +  extra  );

  return( 0 );
}

TColorImage::Pointer crearCanal(TColorImage* img)
{
  TColorImage::Pointer compImg = TColorImage::New( );
  compImg->SetSpacing( img->GetSpacing( ) );
  compImg->SetOrigin( img->GetOrigin( ) );
  compImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  compImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  compImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  compImg->Allocate( );

  // Initialize created images in black
  TRGBPixel black;
  black.SetRed( 0 );
  black.SetGreen( 0 );
  black.SetBlue( 0 );
  compImg->FillBuffer( black );

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
