


// Run with:
// ./WatershedImageFilter threshold level
// e.g.
// ./WatershedImageFilter 0.005 .5
// (A rule of thumb is to set the Threshold to be about 1 / 100 of the Level.)

typedef itk::Image<unsigned char, 2>       UnsignedCharImageType;
typedef itk::Image<float, 2>               FloatImageType;
typedef itk::RGBPixel<unsigned char>       RGBPixelType;
typedef itk::Image<RGBPixelType, 2>        RGBImageType;
typedef itk::Image<itk::IdentifierType, 2> LabeledImageType;

typedef itk::Image< unsigned char, 2 >  ImageType;

static void PerformSegmentation(FloatImageType::Pointer image, const float threshold, const float level);

int main( int argc, char *argv[] )
{
  // Verify arguments
  if (argc < 3 )
    {
    std::cerr << "Parameters " << std::endl;
    std::cerr << " threshold level" << std::endl;
    return 1;
    }

  std::string inputFileName = argv[1];
  // Parse arguments
  std::string strThreshold = argv[2];
  float threshold = 0.0;
  std::stringstream ssThreshold;
  ssThreshold << strThreshold;
  ssThreshold >> threshold;
  
  std::string strLevel = argv[3];
  float level = 0.0;
  std::stringstream ssLevel;
  ssLevel << strLevel;
  ssLevel >> level;
  
  // Output arguments
  std::cout << "Running with:" << std::endl
            << "Threshold: " << threshold << std::endl
            << "Level: " << level << std::endl;
  
  UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
  //CreateImage(image);

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName.c_str());
  reader->Update();

  image = reader->GetOutput();

  typedef itk::GradientMagnitudeImageFilter<
    UnsignedCharImageType, FloatImageType >  GradientMagnitudeImageFilterType;
  GradientMagnitudeImageFilterType::Pointer gradientMagnitudeImageFilter = GradientMagnitudeImageFilterType::New();
  gradientMagnitudeImageFilter->SetInput(image);
  gradientMagnitudeImageFilter->Update();

  // Custom parameters
  PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), threshold, level);
  
  // Fixed parameters
  //PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .0025, .25);
  //PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .005, .5);
  //PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .0075, .75);
  //PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .009, .9);
  
  return EXIT_SUCCESS;
}

void PerformSegmentation(FloatImageType::Pointer image, const float threshold, const float level)
{
  typedef itk::WatershedImageFilter<FloatImageType> WatershedFilterType;
  WatershedFilterType::Pointer watershed = WatershedFilterType::New();
  watershed->SetThreshold(threshold);
  watershed->SetLevel(level);
  watershed->SetInput(image);
  watershed->Update();

  typedef itk::ScalarToRGBColormapImageFilter<LabeledImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
  colormapImageFilter->SetInput(watershed->GetOutput());
  colormapImageFilter->SetColormap( RGBFilterType::Jet );
  colormapImageFilter->Update();

  std::stringstream ss;
  ss << "output_" << threshold << "_" << level << ".png";
  
  typedef itk::ImageFileWriter<RGBImageType> FileWriterType;
  FileWriterType::Pointer writer = FileWriterType::New();
  writer->SetFileName(ss.str());
  writer->SetInput(colormapImageFilter->GetOutput());
  writer->Update();
    
}
