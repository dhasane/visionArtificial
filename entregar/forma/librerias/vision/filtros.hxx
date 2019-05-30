

// umbralizado 
// back projection
void backProjection(Mat src, Mat &dst , int bins);
// otsu solo tomando en cuenta grises
void otsu(Mat &src, Mat &dst);
// otsu tomando en cuenta rgb, los colores inferiores al umbral los binarisa, el resto quedan intactos 
void otsu2(Mat &src, Mat &res);
// valores superiores al umbral se les da el valor del umbral
void corteInferiorUmbral(Mat & src, Mat & dst, int umbral);

// suavizado 
// se hace un suavizado por un kernel de tamaño kernel_size promedio 
void suavisarPromedio( Mat &img, Mat &dst , int kernel_size);
// experimento, se ve chevere pero no fue muy util
void suavisarExp(Mat img, Mat &dst);



// se realiza el gradiente morfologico, primero erocion y luego dilatacion
void limpiar(Mat &img,Mat &res, int tipoero, int tipodil, int tam);
// retorna el histograma de la imagen src
void histograma(Mat src,Mat &histImg);
// ecualiza el histograma de la imagen src
void equalize( Mat &src , Mat &dst);


// supone agrupar colores en cantidad de cortes
void agruparColores( Mat &src, Mat &res, int cortes );
// igual pero con grises
void agruparGrises( Mat &src, Mat &res, int cortes );




void agruparColores( Mat &src, Mat &res, int cortes )
{
    res = src.clone();
    // vectores de aparicion de intensidades 
    int verde[256] = {0};
    int rojo [256] = {0};
    int azul [256] = {0};
    
    MatIterator_< Vec3b > it, end;
    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        azul [(*it)[0]] += 1;
        verde[(*it)[1]] += 1;
        rojo [(*it)[2]] += 1;
    }

    int tam = int(256/cortes);
    int salto = int (256/ cortes );

    float* corteAzul  = new float[salto];
    float* corteRojo  = new float[salto];
    float* corteVerde = new float[salto];

    int* totalAzul  = new int[salto];
    int* totalRojo  = new int[salto];
    int* totalVerde = new int[salto];

    for(int a = 0 ; a < cortes ; a ++)
    {
        corteAzul [a] = 0;
        corteRojo [a] = 0;
        corteVerde[a] = 0;

        totalAzul [a] = 0;
        totalRojo [a] = 0;
        totalVerde[a] = 0;
    }

    
    cout<<"salto : "<<salto<<endl;
    
    for( int a = 0 ; a < cortes; a++)
    {
        for (int b = salto*a ; b < salto*(a+1) && b < 256; b++)
        {
            totalAzul[a]  += azul [b];
            totalRojo[a]  += rojo [b];
            totalVerde[a] += verde[b]; 

            corteAzul [a] += b * azul [b];
            corteRojo [a] += b * rojo [b];
            corteVerde[a] += b * verde[b];
        }
    }

    for( int a = 0 ; a < cortes; a++)
    {
        corteAzul [a] /= totalAzul [a];
        corteRojo [a] /= totalRojo [a];
        corteVerde[a] /= totalVerde[a];

    }

    cout<<"azul  : ";
    for(int a = 0 ; a < cortes ; a ++)
    {
        cout<<corteAzul[a]<<" | ";
    }
    cout<<endl<<"rojo  : ";
    for(int a = 0 ; a < cortes ; a ++)
    {
        cout<<corteRojo[a]<<" | ";
    }
    cout<<endl<<"verde : ";
    for(int a = 0 ; a < cortes ; a ++)
    {
        cout<<corteVerde[a]<<" | ";
    }
    cout<<endl;

    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );
    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        (*it)[0] = corteAzul [ int((*it)[0]/salto) ];
        (*it)[1] = corteVerde[ int((*it)[1]/salto) ];
        (*it)[2] = corteRojo [ int((*it)[2]/salto) ];
    }

}

void agruparGrises( Mat &src, Mat &res, int cortes )
{
    
    cvtColor( src, res, COLOR_BGR2GRAY );
    // vectores de aparicion de intensidades 
    int azul [256] = {0};
    
    MatIterator_< Vec3b > it, end;
    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        azul [(*it)[0]] += 1;
    }

    int tam = int(256/cortes);
    int salto = int (256/ cortes );

    float* corteAzul  = new float[salto];

    int* totalAzul  = new int[salto];

    for(int a = 0 ; a < cortes ; a ++)
    {
        corteAzul [a] = 0;

        totalAzul [a] = 0;
    }

    
    cout<<"salto : "<<salto<<endl;

    for( int a = 0 ; a < cortes; a++)
    {
        for (int b = salto*a ; b < salto*(a+1) && b < 256; b++)
        {
            totalAzul[a]  += azul [b];

            corteAzul [a] += b * azul [b];
        }
    }

    for( int a = 0 ; a < cortes; a++)
    {
        corteAzul [a] /= totalAzul [a];
    }

    cout<<"azul  : ";
    for(int a = 0 ; a < cortes ; a ++)
    {
        cout<<corteAzul[a]<<" | ";
    }
    cout<<endl;

    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );
    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        (*it)[0] = corteAzul [ int((*it)[0]/salto) ];
    }

    cvtColor( res, res, COLOR_GRAY2BGR);
}

void corteInferiorUmbral(Mat & src, Mat & dst, int umbral)
{
    MatIterator_< Vec3b > it, end;
    it  = src.begin< Vec3b >( );
    end = src.end< Vec3b >( );

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        if( (*it)[0] > umbral ){ (*it)[0] = umbral; }
        if( (*it)[1] > umbral ){ (*it)[1] = umbral; }
        if( (*it)[2] > umbral ){ (*it)[2] = umbral; }
    }
}

void equalize( Mat &src , Mat &dst)
{
  /// Convert to grayscale
  cvtColor( src, dst, COLOR_BGR2GRAY );

  /// Apply Histogram Equalization
  equalizeHist( dst, dst );

  cvtColor( dst, dst, COLOR_GRAY2BGR);
}

void histograma(Mat src,Mat &histImg)
{
    int bins = 255;
    
    Mat hue, hsv;
    cvtColor( src, hsv, COLOR_BGR2HSV );
    hue.create(hsv.size(), hsv.depth());
    int ch[] = { 0, 0 };
    mixChannels( &hsv, 1, &hue, 1, ch, 1 );

    int histSize = MAX( bins, 2 );
    float hue_range[] = { 0, 255 };
    const float* ranges = { hue_range };
    Mat hist;
    calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );

    int w = 400, h = 400;
    int bin_w = cvRound( (double) w / histSize );
    histImg = Mat::zeros( h, w, CV_8UC3 );
    for (int i = 0; i < bins; i++)
    {
        rectangle( histImg, Point( i*bin_w, h ), Point( (i+1)*bin_w, h - cvRound( hist.at<float>(i)*h/255.0 ) ),
                   Scalar( 0, 0, 255 ), FILLED );
    }
}

void erocion( Mat &img, Mat &src, int erosion_elem, int erosion_size )
{
    int erosion_type;
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

    Mat element = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size ) );

    erode( img, src, element );
}

void dilatar( Mat &img,Mat &src, int dilation_elem, int dilation_size )
{
  int dilation_type;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  dilate( img , src, element );
}

void limpiar(Mat &img,Mat &res, int tipoero, int tipodil, int tam)
{
    erocion(img ,res , tipoero , tam );
    dilatar(res ,res , tipodil , tam );
}

// todo lo de aplicar kernels debe ir de aqui para abajo
void aplicarKernel(Mat img, Mat &dst, Mat kernel)
{
  Point anchor = Point( -1, -1 );
  double delta = 0;
  int ddepth = -1;

  filter2D(img, dst , ddepth , kernel , anchor, delta, BORDER_DEFAULT );
  //cout<<kernel<<endl;
}

void suavisarExp(Mat img, Mat &dst)
{
    Mat_<float> diferencia(3,3);
    int val = 70;
    //*
    diferencia <<   -val,  val, -val,
                     val,    1,  val,
                    -val,  val, -val;
    /*/
    
    diferencia <<   val,  val, val,
                    val, -val, val,
                    val,  val, val;
    diferencia /= val*7;
    //*/
    aplicarKernel( img,dst, diferencia );
}

void suavisarPromedio( Mat &img, Mat &dst , int kernel_size)
{
    Mat_<float> kernel(3,3);
    kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    aplicarKernel(img,dst,kernel);
}

void backProjection(Mat src, Mat &dst , int bins)
{
    Mat hue, hsv;
    cvtColor( src, hsv, COLOR_BGR2HSV );
    hue.create(hsv.size(), hsv.depth());
    int ch[] = { 0, 0 };
    mixChannels( &hsv, 1, &hue, 1, ch, 1 );
    
    int histSize = MAX( bins, 2 );
    float hue_range[] = { 0, 180 };
    const float* ranges = { hue_range };
    Mat hist;
    calcHist( &hue, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false );
    normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );
    
    calcBackProject( &hue, 1, 0, hist, dst, &ranges, 1, true );
    cvtColor( dst, dst, COLOR_GRAY2BGR);
}

/// ------------------------------------------- otsu 

// encuentra la varianza para un color y su peso respectivo 
void var(int color[] ,int total,int liminf, int limsup,float & varianza, float & peso)
{
    float promedio = 0;
    int tlocal     = 0;

    varianza = 0 ;
    peso     = 0;

    for ( int a = liminf ; a < limsup ; a ++)
    {
        peso     += color[a];
        promedio += a*color[a];
        tlocal   += color[a];
    }
    peso     /= total;
    promedio /= tlocal;
    for ( int a = liminf ; a < limsup ; a ++)
    {
        varianza += pow((a-promedio), 2 ) + color[a];
    }
    
    if (tlocal != 0)
    {
        varianza /= tlocal;
    }
    else
    {
        varianza  = 0;
    }
}

// encuentra el umbral optimo, a traves del metodo de otsu 
float encontrarUmbral(int color[])
{
    int totalcolor = 0;

    for ( int i  = 0 ; i <= 255 ; i++)
    {
        totalcolor += color[i];
    }

    float vA  = 0;
    float cvA = 0;

    float pA  = 0;
    float cpA = 0;

    float minv   = 0 ;
    float varmin = 0 ;

    float actv   = 0 ;
    float varact = 0 ;

    for ( int corte  = 1 ; corte <= 255 ; corte++)
    {
        var(color,totalcolor, 0, corte, vA, pA);

        var(color,totalcolor, corte+1, 256,cvA,cpA);
        
        actv   = corte;
        varact = pA*vA + cpA*cvA;

        if ( corte == 1 )
        {
            minv   = actv;
            varmin = varact;
        }
        else if ( varact < varmin)
        {
            minv   = actv;
            varmin = varact;
        }
    }
    return minv;
}

// solo sirve para un color, esto porque debe estar la imagen en blanco y negro
void otsu(Mat &src, Mat &dst)
{
    cvtColor( src, dst, COLOR_RGB2GRAY);

    // vectores de aparicion de intensidades 
    int azul [256] = {0};
    
    MatIterator_< Vec3b > it, end;
    it  = dst.begin< Vec3b >( );
    end = dst.end< Vec3b >( );

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        azul [(*it)[0]] += 1;
    }

    // se encuentran los umbrales 
    float umbral  = encontrarUmbral(azul);

    // se inicializan los iteradores 
    it  = dst.begin< Vec3b >( );
    end = dst.end< Vec3b >( );

    // se recorre la imagen original y en caso de una intensidad ser menor al umbral especifico, se guarda 0, de lo contrario 255 
    for(  ; it != end; ++it)
    {
        if ( (*it)[0] > umbral  ) { (*it)[0] = 255 ; } // azul
        else                      { (*it)[0] = 0   ; }

        if ( (*it)[1] > umbral  ) { (*it)[0] = 255 ; } // verde 
        else                      { (*it)[0] = 0   ; }

        if ( (*it)[2] > umbral )  { (*it)[0] = 255 ; } // rojo 
        else                      { (*it)[0] = 0   ; }
    }

    cvtColor( dst, dst, COLOR_GRAY2RGB);
}

// tres colores
void otsu2(Mat &src, Mat &res)
{
    res = src.clone();
    // vectores de aparicion de intensidades 
    int verde[256] = {0};
    int rojo [256] = {0};
    int azul [256] = {0};
    
    MatIterator_< Vec3b > it, end;
    it  = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );

    for(  ; it != end; ++it) // contar aparicion de cada tonalidad
    {
        azul [(*it)[0]] += 1;
        verde[(*it)[1]] += 1;
        rojo [(*it)[2]] += 1;
    }

    float umbAzul  = encontrarUmbral(azul);

    float umbVerde = encontrarUmbral(verde);

    float umbRojo  = encontrarUmbral(rojo);

    // se inicializan los iteradores 
    it = res.begin< Vec3b >( );
    end = res.end< Vec3b >( );

    // se recorre la imagen original y en caso de una intensidad ser menor al umbral especifico, se guarda 0, de lo contrario 255 
    for(  ; it != end; ++it)
    {
        if ( (*it)[0] > umbAzul  ) { /*(*az)[0] = 255; (*it)[0] = 255 ;*/ } // azul
        else                       { (*it)[0] = 0   ; }

        if ( (*it)[1] > umbRojo  ) { /*(*ve)[1] = 255; (*it)[0] = 255 ;*/ } // verde 
        else                       { (*it)[0] = 0   ; }

        if ( (*it)[2] > umbVerde ) { /*(*ro)[2] = 255; (*it)[0] = 255 ;*/ } // rojo 
        else                       { (*it)[0] = 0   ; }
    }

}


class Bordes{

    public:

    Bordes() {}

    Mat aplicarKernel(Mat img, Mat kernel)
    {
        cv::Mat dst;

        Point anchor = Point( -1, -1 );
        double delta = 0;
        int ddepth = -1;

        filter2D(img, dst , ddepth , kernel , anchor, delta, BORDER_DEFAULT );
        cout<<kernel<<endl;
        return dst.clone();
    }

    Mat gauss(Mat img)
    {
        Mat dst;

        Mat_<float> gauss(3,3);
        //gauss  << 3, 2, 3,
        //          2, 1, 2,
        //         3, 2, 3;
        //gauss /= 21;

        gauss  << 1, 1, 1,
                    1, 1, 1,
                    1, 1, 1;
        gauss /= 9;

        dst = aplicarKernel(img,gauss);
        // filter2D(img, dst , ddepth , gauss , anchor, delta, BORDER_DEFAULT );
        cout<<gauss<<endl;
        
        MatIterator_< Vec3b > it, end, it2, end2;
        
        it   = img.begin< Vec3b >( );
        end  = img.end< Vec3b >( );

        it2  = dst.begin< Vec3b >( );
        end2 = dst.end< Vec3b >( );

        for(  ; it != end, it2 != end2; ++it, ++it2)
        {
            *it2 -= *it ;
        }

        return dst.clone();
    }

    Mat difSeparada(Mat img)
    {
        cv::Mat dst;
        Mat_<float> diferencia(3,3);
        diferencia << 0, -1,  0,
                        1,  0, -1,
                        0,  1,  0;

        dst = aplicarKernel( img, diferencia );

        return dst.clone();
    }

    Mat roberts(Mat img)
    {
        cv::Mat dst;
        Mat_<float> diferencia(3,3);
        diferencia << -1,  0, -1,
                        0,  2,  0,
                        0,  0,  0;
        //diferencia /= 2;

        dst = aplicarKernel( img, diferencia );

        return dst.clone();
    }

    Mat difPixeles(Mat img)
    {
        cv::Mat dst;
        Mat_<float> diferencia(3,3);
        diferencia <<  0,  -1,   0,
                        0,   2,  -1,
                        0,   0,   0;

        dst = aplicarKernel( img, diferencia );

        return dst.clone();
    }

    Mat prueba(Mat img)
    {
        cv::Mat dst;

        //int tam = 7;
        int tam = 3;
        Mat_<float> diferencia(tam,tam);
        
        //*/

        /*/
        diferencia <<  1 , 0 , -1 ,
                        0 , 0 ,  0 ,
                        -1 , 0 ,  1 ;
        */

        /*
        diferencia << -1 , 0 , -1 ,
                        0 , 0 ,  0 ,
                        -1 , 0 ,  3 ;
        */

        /*
        diferencia << -1 , 1 , -1 ,
                        1 , 0 ,  1 ,
                        -1 , 1 , -1 ;
        */
        /*
        diferencia << -1 ,  2 , -1 ,
                        2 , -4 ,  2 ,
                        -1 ,  2 , -1 ;
        */
        diferencia <<  2 , -1 ,  2 ,
                      -1 , -4 , -1 ,
                       2 , -1 ,  2 ;

        //diferencia /= 2;
        dst = aplicarKernel( img, diferencia );

        return dst.clone();
    }

    Mat pruebaCirc(Mat img)
    {
        cv::Mat dst;

        int tam = 7;
        Mat_<float> diferencia(tam,tam);
        
        diferencia <<  4 ,  4 ,  2 ,  2  ,  2 ,  4 , 4 , // 22
                        4 ,  2 ,  2 , -2  ,  2 ,  2 , 4 , // 14     // 36
                        4 ,  2 , -3 , -4  , -3 ,  2 , 4 , // 3
                        2 , -2 , -4 , -10 , -4 , -2 , 2 , // -18    // -12       // total 60
                        4 ,  2 , -3 , -4  , -3 ,  2 , 4 , // 3
                        4 ,  2 ,  2 , -2  ,  2 ,  2 , 4 , // 14     // 36
                        4 ,  4 ,  2 ,  2  ,  2 ,  4 , 4 ; // 22

        diferencia /= 60;
        dst = aplicarKernel( img, diferencia );

        return dst.clone();
    }

    void canny(Mat &src, Mat &dst, int lowThreshold )
    {
        //int const max_lowThreshold = 100;
        int ratio = 3;
        int kernel_size = 3;

        Mat src_gray;
        Mat detected_edges;
        /// Convert the image to grayscale
        cvtColor( src, src_gray, COLOR_BGR2GRAY );

        /// Reduce noise with a kernel 3x3
        blur( src_gray, detected_edges, Size(3,3) );

        /// Canny detector
        Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

        /// Using Canny's output as a mask, we display our result
        dst = Scalar::all(0);

        src.copyTo( dst, detected_edges);
        //imshow( window_name, dst );
        // imwrite( "prueba.jpg" , dst );
    }

    // void todos( Mat src )
    // {
    //     string basename = "";
    //     string metodo = "canny";
    //     imwrite( basename + metodo + "resultado.jpg" , canny(src));

    //     metodo = "circular";
    //     imwrite( basename + metodo + "resultado.jpg" , pruebaCirc(src)-src );
        
    //     metodo = "gauss";
    //     imwrite( basename + metodo + "resultado.jpg" , gauss(src) );
        
    //     metodo = "difsep";
    //     imwrite( basename + metodo + "resultado.jpg" , difSeparada(src) );

    //     metodo = "roberts";
    //     imwrite( basename + metodo + "resultado.jpg" , roberts(src) );
        
    //     metodo = "difpixeles";
    //     imwrite( basename + metodo + "resultado.jpg" , difPixeles(src) );
        
    //     metodo = "experimental";
    //     imwrite( basename + metodo + "resultado.jpg" , prueba(src) );
    //     //imwrite( basename + metodo + ".jpg" , prueba(src) );
    // }

};

// tope y base, uno debe ser 0 y el otro 255, hecho asi para poder invertir el resultado mas facilmente 
void binarizar(Mat & dest, Mat & hola , int umbral, int tope, int base)
{
    cvtColor( dest, hola, COLOR_BGR2GRAY );
    MatIterator_< Vec3b > it, end;
    it  = hola.begin< Vec3b >( );
    end = hola.end< Vec3b >( );
    for(  ; it != end; ++it)
    {
        if( (*it)[0] < umbral ) { (*it)[0] = tope; }
        else                    { (*it)[0] = base; }
    }
    cvtColor( hola, hola, COLOR_GRAY2BGR );
}


// ero es si primero se hace erocion y luego dilatacion o al reves
void limpiar( bool ero, Mat &img,Mat &res, int tipo , int tam)
{
    if ( ero )
    {
        erocion(img ,res , tipo , tam );
    }
    dilatar(res ,res , tipo , tam );
    if (!ero)
    {
        erocion(img ,res , tipo , tam );
    }
}

