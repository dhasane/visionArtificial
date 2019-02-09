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
TColorImage::Pointer crearCanal(TColorImage* img,TRGBPixel col);


void destruir(std::string basename,std::string newname);

int main( int argc, char* argv[] )
{
  // Get command line arguments
  if( argc < 2 )
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

  std::string nom = argv[1]; 
  
  destruir(nom, argv[2]);
  return( 0 );
}

void destruir(std::string basename,std::string newname)
{

  TRGBPixel black, white;
  black.SetRed( 0 );
  black.SetGreen( 0 );
  black.SetBlue( 0 );

  white.SetRed( 255 );
  white.SetGreen( 255 );
  white.SetBlue( 255 );


  // Read an image
  TReader::Pointer reader = TReader::New( );
  reader->SetFileName( basename );
  try
  {
    reader->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    

  } // yrt
  TColorImage* img = reader->GetOutput( );

  // creacion de canales
  TColorImage::Pointer rImg = crearCanal(img,black); // rojo
  TColorImage::Pointer gImg  = crearCanal(img,black); // verde 
  TColorImage::Pointer bImg  = crearCanal(img,black); // azul

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
  std::stringstream ss( basename );
  //std::string basename;
  getline( ss, basename, '.' );

  TWriter::Pointer writer = TWriter::New( );

  // de pa bajo
  // le hace resize a las imagenes para reducirles el tamaño, y las guarda
  //std::cout << "rojo: "<< std::endl;
  rImg = rescale(rImg,0.75,0.75);

  //std::cout << "verde: "<< std::endl;
  gImg = rescale(gImg,0.5,0.5);

  //std::cout << "azul: "<< std::endl;
  bImg = rescale(bImg,0.25,0.25);

  // de pa rriba
  //std::cout << "---------------"<< std::endl;
  // le hace resize a las imagenes para aumentarles el tamaño, y las guarda
  
  //std::cout << "rojo: "<< std::endl;
  rImg = rescale(rImg,1.3335,1.3335); 

  //std::cout << "verde: "<< std::endl;
  gImg = rescale(gImg,2,2);

  //std::cout << "azul: "<< std::endl;
  bImg = rescale(bImg,4,4);

  //std::cout << "---------------"<< std::endl;
  
  TColorImage::Pointer rgbImg   = crearCanal(img,black); // composite image (RGB)
  TColorImage::Pointer compImg  = crearCanal(img,white); // comparacion

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
  it.GoToBegin( );

  

  //std::cout<<"cargando imagenes comparacion y rgb"<<std::endl;
  // esta es la parte que no esta funcionando
  for( ; !rgbIt.IsAtEnd( ) && !compIt.IsAtEnd( ) && !ccrIt.IsAtEnd( ) && !ccgIt.IsAtEnd( ) && !ccbIt.IsAtEnd( ) && !it.IsAtEnd() ; ++rgbIt, ++compIt, ++ccrIt, ++ccgIt, ++ccbIt, ++it )
  {
    TRGBPixel valuer, valueg, valueb, pixel, pixel2;

    valuer = ccrIt.Get( );
    pixel.SetRed( valuer.GetRed( ) );

    valueg = ccgIt.Get( );
    pixel.SetGreen( valueg.GetGreen( ) );

    valueb = ccbIt.Get( );
    pixel.SetBlue( valueb.GetBlue( ) );

    rgbIt.Set( pixel );

    pixel2.SetRed( ( it.Get( ).GetRed() + rgbIt.Get().GetRed()  ) );

    pixel2.SetBlue( ( it.Get( ).GetBlue() + rgbIt.Get().GetBlue() ) );
  
    pixel2.SetGreen( ( it.Get( ).GetGreen() + rgbIt.Get().GetGreen() ) );
  
    // para la comparacion 

    
    compIt.Set( pixel2 );
    
    
  }

  //guardarImg( writer, rgbImg    , basename + ".png"   );
  guardarImg( writer, compImg   , newname+".png"   );

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
