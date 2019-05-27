#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace cv;
using namespace std;

void buscarCara(Mat &imagen, Mat &prueba, int &ancho, int &alto, int &cambiarX);
int cantidadSectores(Mat &prueba);
int cantidadSectores2(Mat &prueba);
/** @function main */
int main( int argc, char** argv )
{

  Mat src = imread(argv[1],1);
  if(src.rows!=3968 || src.cols !=2976){
    resize(src,src,Size(2976,3968));
  }
  
  Mat blur;
  GaussianBlur(src, blur, Size(9,9), 1);
  namedWindow( "elipse", WINDOW_NORMAL);
  imshow("elipse",blur);
  waitKey(0);


  
  
  Mat gris;
  gris = blur.clone();
  cvtColor(gris, gris, COLOR_RGB2GRAY);
  Mat gris2 = gris.clone();
  Mat prueba = src.clone();
  Mat pruebaR = src.clone();


  int ancho=1, alto=1, cambiarX = pruebaR.cols/2;

  buscarCara(gris, prueba, ancho, alto, cambiarX);  
  cout<<"ancho"<<ancho<<" "<<"alto"<<alto<<"cambiarX"<<cambiarX<<endl;
  ellipse( pruebaR, Point( cambiarX, (pruebaR.rows/2)-200 ), Size( 300.0+(75.0*ancho),200.0+(100.0*alto)), 0, 0, 360, Scalar( 0, 0, 0 ),-1, 8,0 );


  Mat cara = src-pruebaR;
  namedWindow( "elipse", WINDOW_NORMAL);
  imshow("elipse",cara);


  Mat img3 = cara;
  imwrite( "resultado.jpg" , img3);
  Mat img4 = cara;
  int f=img4.rows, c=img4.cols;
  int sale =0;


  int iniciox = 0, inicioy = 0; 
  Vec3b nPixel = img4.at<Vec3b>(iniciox,inicioy);
  if(iniciox == 0 && inicioy == 0 && nPixel[0]!=0 && nPixel[1]!=0 && nPixel[2]!=0){
        for(int a=0; a<img4.rows && sale == 0; a++){
          for(int b=0; b<img4.cols && sale == 0; b++){
              nPixel = img4.at<Vec3b>(b,a);

              if(nPixel[0]==0.0 && nPixel[1]==0.0 && nPixel[2]==0.0){
                iniciox=a;
                cout<<a<<" "<<b<<endl;
                cout<<nPixel<<endl;
                sale++;
              }
            } 
        }   
        iniciox=iniciox+6;
        cout<<"iniciox"<<iniciox<<endl;
        sale = 0;
        for(int a=iniciox; a<img4.cols && sale == 0; a++){
          for(int b=0; b<img4.rows && sale == 0; b++){
              nPixel = img4.at<Vec3b>(b,a);
              if(nPixel[0]==0.0 && nPixel[1]==0.0 && nPixel[2]==0.0){
                inicioy=b;
                sale++;
              }
            } 
        }
        inicioy=inicioy+6; 
        cout<<"inicioy"<<inicioy<<endl;
  }
     


  int inicioCx=0;
  int inicioCy=0;
  //Izquierda a derecha
  for(int i=inicioy; i<img4.rows; i++){
    for(int j=iniciox; j<img4.cols; j++){
      Vec3b pixel = img4.at<Vec3b>(j,i);
      if(pixel[0]>5 && pixel[1]>5 && pixel[2]>5){
        if(c>j){
          c=j;
          inicioCx=j;
        }
      }
    }
  }
  cout<<"c"<<inicioCx<<endl;

  
  //Inicia Y
  sale = 0;
  for(int i=iniciox; i<img4.cols; i++){
    for(int j=inicioy; j<img4.rows; j++){
      Vec3b pixel = img4.at<Vec3b>(i,j);
      if(pixel[0]>5 && pixel[1]>5 && pixel[2]>5){
        if(f>j){
          f=j;
          inicioCy=j;
        }
      }
    }
  }
  cout<<"f"<<inicioCy<<endl;


   //Termina y
  int finy=0;
  for(int i=img4.rows-1; i>=0; i--){
      Vec3b pixel = img4.at<Vec3b>(inicioCx,i);
      if(pixel[0]>5 && pixel[1]>5 && pixel[2]>5){
          if(finy<i){
            finy=i;
          }
      } 
  }

  
  


   //Termina x
  int finx=0;
  for(int i=img4.cols-1; i>=0; i--){
      Vec3b pixel = img4.at<Vec3b>(i,inicioCy);
      if(pixel[0]>5 && pixel[1]>5 && pixel[2]>5){
          if(finx<i){
            finx=i;
          }
      } 
  }

  cout<<"->>"<<inicioCy<<" "<<inicioCx<<" "<<finx<<" "<<finy<<endl;
  finx = img4.cols-finx;
  finy = img4.rows-finy-inicioCy;
  //finy = finy+inicioCx;
  cout<<endl<<c<<" "<<f<<" "<<finx<<" "<<finy<<endl;
  Rect rec(inicioCy,inicioCx, finx, finy);
  Mat crop = img3(rec);
  imwrite("recorte.jpg",crop);

  
  return(0);
}

void buscarCara(Mat &imagen, Mat &prueba, int &ancho, int &alto, int &cambiarX){
  int suma=0;
  int anterior = 0, diferencia = 0;
  int nuevaDiferencia=0;
  threshold(imagen, prueba, 70, 255,1);
  imwrite("aaa.jpg",prueba);

  Mat prueba1 = prueba.clone();
  Mat prueba2 = prueba.clone();
  Mat prueba3 = prueba.clone();
  cout<<"Buscando ancho"<<endl;
  for(int k = 0; k<20; k++){
    suma=0;
    ellipse( prueba1, Point( prueba1.cols/2, (prueba1.rows/2)-200 ), Size( 300.0+(75.0*k), 700.0 ), 0, 0, 360, Scalar( 0, 0, 0 ),-1, 8,0 );
    suma = cantidadSectores(prueba1);
    if(k<=1){
      diferencia = 10000;
      anterior = suma;
    }
    else{
      nuevaDiferencia = anterior-suma;
      if(diferencia < nuevaDiferencia+(150*k)){
          //ellipse( prueba1, Point( prueba1.cols/2, (prueba1.rows/2)-200 ), Size( 300.0+(75.0*k), 700.0 ), 0, 0, 360, Scalar( 0, 0, 0 ),-1, 8,0 );
          ancho = k;
          break;
      }
      else{
          diferencia = anterior-suma;
          anterior = suma;
      }
    }
  }

  //--------------------------------------------------------------------------------------------------

  cout<<"Buscando alto"<<endl;
  anterior = 0, diferencia = 0;
  for(int k = 0; k<20; k++){
    suma=0;
    ellipse( prueba2, Point( prueba2.cols/2, (prueba2.rows/2)-200 ), Size(ancho*10, 200.0+(100.0*k) ), 0, 0, 360, Scalar( 0, 0, 0 ),-1, 8,0 );
    suma = cantidadSectores2(prueba2);
    if(k<1){
      diferencia = 10000;
      anterior = suma;
    }
    else{
      nuevaDiferencia = anterior-suma;
      if(diferencia < nuevaDiferencia+(150*k)){
          //ellipse( prueba2, Point( prueba2.cols/2, (prueba2.rows/2)-200 ), Size( ancho*10, 200.0+(100.0*k) ), 0, 0, 360, Scalar( 0, 0, 0 ),-1, 8,0 );
          alto = k;
          break;
      }
      else{
          diferencia = anterior-suma+800;
          anterior = suma;
      }
   }
  }


//-------------------------------------------------------------------------------------------------

  cout<<"Buscando posición"<<endl;
  anterior = 0;
  diferencia = 0;
  int x=prueba3.cols/2;
  int y=prueba3.rows/2;
  int menor = 100000;

  for(int k = -10; k<20; k++){
    prueba3 = prueba.clone();
    suma=0;
    x = prueba3.cols/2 + 100*k;
    if(x >  prueba3.cols){
      x = prueba3.cols;
    }
    ellipse( prueba3, Point( x, (prueba3.rows/2)-200 ), Size( 300.0+(75.0*ancho),200.0+(100.0*alto)), 0, 0, 360, Scalar( 0, 0, 0 ),-1, 8,0 );
    suma = cantidadSectores(prueba3);

    if(k<=-10){
      diferencia = 10000;
      anterior = suma;
      cambiarX = 100000;
    }
    else{
      nuevaDiferencia = anterior-suma;
      if(menor > suma){
        menor = suma;
        cambiarX = x + 30;
      }

    }

  }
  cout<<cambiarX<<endl;
  //threshold( prueba, prueba, 90, 255,1);
}


int cantidadSectores(Mat &prueba){
  int sectores = 0;
  int color=0;
  int suma=0;

  
  for(int i=0; i<prueba.rows; i++){
    for(int j=0; j<prueba.cols; j++){
      Vec3b nPixel = prueba.at<Vec3b>(j,i);
      if(j==0){
        color = nPixel[0];
        sectores = 1;
      }
      if(nPixel[0]!=color && nPixel[1]!=color && nPixel[2]!=color ){
        sectores++;
        if(color==255){
          color =0;
        }
        else{
          color=255;
        }
      }
    }
    suma+=sectores;
  }
  return suma;
}


int cantidadSectores2(Mat &prueba){
  int sectores = 0;
  int color=0;
  int suma=0;

  
  for(int i=0; i<prueba.rows; i++){
    for(int j=0; j<prueba.cols; j++){
      Vec3b nPixel = prueba.at<Vec3b>(i,j);
      if(j==0){
        color = nPixel[0];
        sectores = 1;
      }
      if(nPixel[0]!=color && nPixel[1]!=color && nPixel[2]!=color ){
        sectores++;
        if(color==255){
          color =0;
        }
        else{
          color=255;
        }
      }
    }
    suma+=sectores;
  }
  return suma;
}



/*
int x=prueba3.cols/2;
  
  int menor = 100000;
  //for(int h = 0; h<200; h = h+5){
    namedWindow( "sinelipse", WINDOW_NORMAL);
        imshow("sinelipse",prueba);
        waitKey(0);

      for(int k = -10; k<25; k++){
        prueba3 = prueba.clone();
        suma=0;
        x = prueba3.cols/2 + 100*k;
        if(x >  prueba3.cols){
          x = prueba3.cols;
        }
        ellipse( prueba3, Point( x, (prueba3.rows/2)-200 ), Size( 300.0+(75.0*ancho),200.0+(100.0*alto)), 0, 0, 360, Scalar( 0, 0, 0 ),-1, 8,0 );
        for(int i=0; i<prueba.rows; i++){
          for(int j=0; j<prueba.cols; j++){
            Vec3b nPixel = prueba3.at<Vec3b>(j,i);
            if(j==0){
              color = nPixel[0];
              sectores = 1;
            }
            if(nPixel[0]!=color && nPixel[1]!=color && nPixel[2]!=color ){
              sectores++;
              if(color==255){
                color =0;
              }
              else{
                color=255;
              }
            }
          }
          suma+=sectores;
        }
        cout<<"con elipse("<<k<<")"<<suma<<" ";

        if(k<=-10){
          diferencia = 10000;
          anterior = suma;
          cout<<endl;
          cambiarX = 100000;
        }
        else{
          int nuevaDiferencia = anterior-suma;
          cout<<"anterior "<<anterior<<"||";
          cout<<"diferencia "<<diferencia<<"||";
          cout<<"nueva diferencia "<<nuevaDiferencia<<endl;
          if(menor > suma){
            menor = suma;
            cambiarX = x + 20;
          }

        }
        cout<<"x"<<cambiarX<<endl;
        namedWindow( "conelipse", WINDOW_NORMAL);
        imshow("conelipse",prueba3);
        //waitKey(0);
      }

*/