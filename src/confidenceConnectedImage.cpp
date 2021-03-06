#include <exception>
#include <algorithm>
#include <vector>
#include <string>
#include <ants.h>
#include "itkConfidenceConnectedImageFilter.h"
#include "itkImage.h"
#include "RcppANTsR.h"

template< class ImageType >
SEXP confidenceConnectedImageHelper( 
    SEXP r_inimg,
    SEXP r_outimg,
    Rcpp::IntegerVector seed,
    Rcpp::NumericVector radius,
    Rcpp::NumericVector multiplier,
    Rcpp::IntegerVector iterations
    )
{
  typedef typename ImageType::Pointer ImagePointerType;
  typename ImageType::Pointer inimg =
    Rcpp::as< ImagePointerType >( r_inimg );
  typename ImageType::Pointer outimg =
    Rcpp::as< ImagePointerType >( r_outimg );
  typedef itk::ConfidenceConnectedImageFilter< ImageType, ImageType >
    ConfidenceConnectedFilterType; 
  typename ConfidenceConnectedFilterType::Pointer filter =
    ConfidenceConnectedFilterType::New();

  filter->SetInitialNeighborhoodRadius(radius[0]);
  filter->SetMultiplier(multiplier[0]);
  filter->SetNumberOfIterations(iterations[0]);

  typename ImageType::IndexType seed2;
  for( int i = 0 ; i < seed.size(); ++i )
    {
    seed2[i] = seed[i] - 1; //0 indexed
  }
  filter->SetReplaceValue(1);
  filter->SetSeed(seed2);
  filter->SetInput( inimg );
  filter->Update();
  outimg = filter->GetOutput();
  r_outimg = Rcpp::wrap( outimg );
  return( r_outimg );
}

RcppExport SEXP confidenceConnectedImage( 
    SEXP r_inimg,
    SEXP r_outimg,
    SEXP r_seed,
    SEXP r_radius,
    SEXP r_multiplier,
    SEXP r_iterations )
{
  try
  {
    Rcpp::S4 antsImage( r_inimg );
    std::string pixeltype = Rcpp::as< std::string >( antsImage.slot( "pixeltype" ));
    unsigned int dimension = Rcpp::as< int >( antsImage.slot( "dimension" ) );
    Rcpp::IntegerVector seed( r_seed );
    Rcpp::NumericVector radius( r_radius );
    Rcpp::NumericVector multiplier( r_multiplier );
    Rcpp::IntegerVector iterations( r_iterations );

    if ( (pixeltype == "float") & ( dimension == 2 ) )
    {
      typedef float PixelType;
      const unsigned int dim = 2;
      typedef itk::Image< PixelType, dim > ImageType;
      SEXP outimg = confidenceConnectedImageHelper< ImageType >(
          r_inimg, r_outimg, seed, multiplier, radius, iterations);
      return( outimg );
    }
    else if ( ( pixeltype == "float" ) & ( dimension == 3 ) )
    {
      typedef float PixelType;
      const unsigned int dim = 3;
      typedef itk::Image< PixelType, dim > ImageType;
      SEXP outimg = confidenceConnectedImageHelper< ImageType >(
          r_inimg, r_outimg, seed, multiplier, radius, iterations);
      return( outimg );
    }
    else if ( ( pixeltype == "float" ) & ( dimension  == 4 ) )
    {
      typedef float PixelType;
      const unsigned int dim = 4;
      typedef itk::Image< PixelType, dim > ImageType;
      SEXP outimg = confidenceConnectedImageHelper< ImageType >(
          r_inimg, r_outimg, seed, multiplier, radius, iterations);
      return (outimg);
    }
    else
    {
      Rcpp::stop("Unsupported image dimension or pixel type.");
    }
  }

  catch( itk::ExceptionObject & err )
  {
    Rcpp::Rcout << "ITK ExceptionObject caught!" << std::endl;
    forward_exception_to_r( err );
  }
  catch( const std::exception& exc )
  {
    Rcpp::Rcout << "STD ExceptionObject caught!" << std::endl;
    forward_exception_to_r( exc );
  }
  catch( ... )
  {
    Rcpp::stop( "C++ exception (unknown reason)");
  }
  return Rcpp::wrap(NA_REAL); // should not be reached
}
