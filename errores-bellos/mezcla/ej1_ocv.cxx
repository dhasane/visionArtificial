#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
//#include "diffFilterWidget.hpp"

using namespace cv;
//using namespace std;
void escalarImagen(std::string basename, std::string termina, std::string terminaN, float tamano);

int main(int argc, char** argv )
{
  
  // Comrpueba el numero de argumentos
  if ( argc < 2 )
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

  Mat image;
  image = imread( argv[1], 1 );
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );
  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi

  // Crea imagenes de un solo canal
  // red channel
  Mat rImg = Mat::zeros( image.size( ), CV_8UC1 );
  // green channel
  Mat gImg = Mat::zeros( image.size( ), CV_8UC1 );
  // blue channel
  Mat bImg = Mat::zeros( image.size( ), CV_8UC1 );
  // composite image (RGB)
  //Mat rgbImg = Mat::zeros( image.size( ), CV_8UC3 );
   
  // Fill color channel images
  MatIterator_< Vec3b > it, crIt, cgIt, cbIt, rgbIt, end, endr, endg, endb, endrgb;
  it = image.begin< Vec3b >( );
  crIt = rImg.begin< Vec3b >( );
  cgIt = gImg.begin< Vec3b >( );
  cbIt = bImg.begin< Vec3b >( );
  end = image.end< Vec3b >( );
  endr = rImg.end< Vec3b >( );
  endg = gImg.end< Vec3b >( );
  endb = bImg.end< Vec3b >( );
  for(  ; it != end, crIt != endr, cgIt != endg, cbIt != endb; ++it, ++crIt, ++cgIt, ++cbIt)
  {
    //Color blanco existe color, negro asuencia de calor
    (*crIt)[0] = 0;
  	(*crIt)[1] = 0;
  	(*crIt)[0] = (*it)[2];

  	(*cgIt)[0] = 0;
  	(*cgIt)[0] = (*it)[1];
  	(*cgIt)[2] = 0;
  	
  	(*cbIt)[0] = (*it)[0];
  	(*cbIt)[1] = 0;
  	(*cbIt)[2] = 0;


    
  } // rof

  //Hasta aca se generan las imagenes de un solo canal
  imwrite( basename + "_R.png", rImg );
  imwrite( basename + "_G.png", gImg );
  imwrite( basename + "_B.png", bImg );
  //Se escalan las imagenes
  escalarImagen(basename,"_R.png", "_sR.png", 0.75);
  escalarImagen(basename,"_G.png", "_sG.png", 0.50);
  escalarImagen(basename,"_B.png", "_sB.png", 0.25);
  //Ya que al poner 4/3 queda 1 y no 1.333
  float a=4,b=3;
  escalarImagen(basename,"_sR.png", "_ssR.png", a/b);
  escalarImagen(basename,"_sG.png", "_ssG.png", 2);
  escalarImagen(basename,"_sB.png", "_ssB.png", 4);
  
  

  // Create color channel images
  // red channel
  Mat image2 = imread(basename+"_ssR.png", -1 );
  Mat image3 = imread(basename+"_ssG.png", -1 );
  Mat image4 = imread(basename+"_ssB.png", -1 );
  Mat rgbImg2 = Mat::zeros( image2.size( ), CV_8UC3 );



  //Canal azul
  Mat bImg4= Mat::zeros( image4.size( ), CV_8UC3 );
  MatIterator_< Vec3b > it4, cbIt4, rgbIt4, end4, endb4, endrgb4;
  it4 = image4.begin< Vec3b >( );
  cbIt4 = bImg4.begin< Vec3b >( );
  end4 = image4.end< Vec3b >( );
  endb4 = bImg4.end< Vec3b >( );
  //Se leen los colores de la imagen.
  for(  ; it4 != end4, cbIt4 != endb4 ; ++it4, ++cbIt4)
  {
  	(*cbIt4)[0] = (*it4)[0];
  	(*cbIt4)[1] = 0;
  	(*cbIt4)[2] = 0; 
  } // rof

  // From color channel images, reconstruct the original color image
  cbIt4 = bImg4.begin< Vec3b >( );
  rgbIt4 = rgbImg2.begin< Vec3b >( );
  endb4 = bImg4.end< Vec3b >( );
  endrgb4 = rgbImg2.end< Vec3b >( );
  //Se guarda la imagen en la imagen completa
  for(  ; cbIt4 != endb4, rgbIt4 != endrgb4; ++cbIt4, ++rgbIt4)
  {
  	(*rgbIt4)[0] = (*cbIt4)[0];
  } // rof
  //imwrite( basename + "1RGB.png", rgbImg2 );
  




  //Canal rojo
  Mat rImg2= Mat::zeros( image2.size( ), CV_8UC3 );
  MatIterator_< Vec3b > it2, crIt2, rgbIt2, end2, endr2, endrgb2;
  it2 = image2.begin< Vec3b >( );
  crIt2 = rImg2.begin< Vec3b >( );
  end2 = image2.end< Vec3b >( );
  endr2 = rImg2.end< Vec3b >( );
  //Se leen los colores de la imagen
  for(  ; it2 != end2, crIt2 != endr2 ; ++it2, ++crIt2)
  {
  	(*crIt2)[0] = 0;
  	(*crIt2)[1] = 0;
  	(*crIt2)[2] = (*it2)[2];
  } // rof

  // From color channel images, reconstruct the original color image
  crIt2 = rImg2.begin< Vec3b >( );
  rgbIt2 = rgbImg2.begin< Vec3b >( );
  endr2 = rImg2.end< Vec3b >( );
  endrgb2 = rgbImg2.end< Vec3b >( );
  //Se guarda la imagen en la imagen completa
  for(  ; crIt2 != endr2, rgbIt2 != endrgb2; ++crIt2, ++rgbIt2)
  {
  	(*rgbIt2)[2] = (*crIt2)[2];
  } // rof
 //imwrite( basename + "2RGB.png", rgbImg2 );




  



  //Canal verde
  Mat gImg3= Mat::zeros( image3.size( ), CV_8UC3 );
  MatIterator_< Vec3b > it3, cgIt3, rgbIt3, end3, endg3, endrgb3;
  it3 = image3.begin< Vec3b >( );
  cgIt3 = gImg3.begin< Vec3b >( );
  end3 = image3.end< Vec3b >( );
  endg3 = gImg3.end< Vec3b >( );

  for(  ; it3 != end3, cgIt3 != endg3 ; ++it3, ++cgIt3)
  {
  	(*cgIt3)[0] = 0;
  	(*cgIt3)[1] = (*it3)[1];
  	(*cgIt3)[2] = 0; 
/*
*/
  } // rof

  // From color channel images, reconstruct the original color image
  cgIt3 = gImg3.begin< Vec3b >( );
  rgbIt3 = rgbImg2.begin< Vec3b >( );
  endg3 = gImg3.end< Vec3b >( );
  endrgb3 = rgbImg2.end< Vec3b >( );

  for(  ; cgIt3 != endg3, rgbIt3 != endrgb3; ++cgIt3, ++rgbIt3)
  {
  	(*rgbIt3)[1] = (*cgIt3)[1];
  } // rof imwrite( basename + "_RGB.png", rgbImg );
  imwrite( basename + "_rRGB.png", rgbImg2 );






  Mat diff = Mat::zeros( image.size( ), CV_8UC1 );
  Mat resultado = Mat::zeros( image.size( ), CV_8UC1 );
  //Se encuentra la diferencias
  absdiff(rgbImg2, image, diff);
  
  cvtColor(diff, resultado, COLOR_RGB2GRAY);
  imwrite( basename + "_diff.png", resultado );
  
  return( 0 );
}


void escalarImagen(std::string basename, std::string termina, std::string terminaN, float tamano){
  // Read an image
  Mat image;
  image = imread( basename + termina, 1 );
  std::cout<<"tam"<<tamano<<std::endl;
  Mat res_img;
  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return;
  }
  std::cout << "Image input size: " << image.size() << std::endl;
  resize(image, res_img, Size(), tamano, tamano, INTER_LINEAR);
  std::cout << "Image output size: " << res_img.size() << std::endl;
  
  imwrite( basename + terminaN, res_img );

}
