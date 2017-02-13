// Intersection.cpp

#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <cmath>
#include <unordered_set>
#include <limits>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <string>

#include "glm/gtx/string_cast.hpp"
#include "Intersections.hpp"

#include "App.hpp"



namespace
{

constexpr double eyeDist    = 3.0;
constexpr double focalPlane = eyeDist * 0.5;
constexpr double focalZoom  = 1.0;

constexpr unsigned imgSize = 8;

}


////////////////////////////////////////////////////////////////////
/// \brief The IntersectionApp class
////////////////////////////////////////////////////////////////////
class IntersectionApp : public App
{

public:

  ////////////////////////////////////////////////////////////////////
  /// \brief IntersectionApp
  ////////////////////////////////////////////////////////////////////
  IntersectionApp( );

  ~IntersectionApp( ) = default;

  ////////////////////////////////////////////////////////////////////
  /// \brief inputFunction
  /// \return
  ////////////////////////////////////////////////////////////////////
  virtual std::vector< double > inputFunction ( ) final;

  ////////////////////////////////////////////////////////////////////
  /// \brief targetFunction
  /// \return
  ////////////////////////////////////////////////////////////////////
  virtual std::vector< double > targetFunction ( ) final;

  ////////////////////////////////////////////////////////////////////
  /// \brief onUserLoop
  /// \param line
  /// \return true if the user loop should continue, false otherwise
  ////////////////////////////////////////////////////////////////////
  virtual bool onUserLoop ( const std::string &line ) final;


  ////////////////////////////////////////////////////////////////////
  /// \brief buildImage
  /// \param w
  /// \param h
  /// \return
  ////////////////////////////////////////////////////////////////////
  std::vector< char > buildImage (
                                  unsigned          width,
                                  unsigned          height,
                                  const glm::dvec3 &eye,
                                  const double      fPlane,
                                  bool              exact
                                  );


protected:

private:

  std::uniform_real_distribution< double >  realDist_;
  std::uniform_int_distribution< unsigned > intDist_;

  double focalPlane_;

};


////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::IntersectionApp
////////////////////////////////////////////////////////////////////
IntersectionApp::IntersectionApp( )
  : App( std::vector< unsigned >{ 4, 5, 1 }, 0.99 )
  , realDist_  ( -1.0, 1.0 )
  , intDist_   ( 0, imgSize )
  , focalPlane_( focalPlane )
{}



////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::inputFunction
/// \return
////////////////////////////////////////////////////////////////////
std::vector< double >
IntersectionApp::inputFunction( )
{

  glm::dvec3 p( 0.0, 0.0, 1.0 );

  p = glm::normalize( p );

  glm::dvec2 uv = glm::dvec2(
                             intDist_( gen_ ) * 1.0 / imgSize,
                             intDist_( gen_ ) * 1.0 / imgSize
                             );

  glm::dvec3 up = glm::dvec3( 0.0, 1.0, 0.0 ); // up axis of world
  double f      = focalPlane;                   // distance between eye and focal plane

  // camera basis
  glm::dvec3 w = glm::normalize( -p );
  glm::dvec3 u = glm::normalize( cross( w, up ) );
  glm::dvec3 v = glm::normalize( cross( u, w ) );

  // pixel space of the focal plane
  glm::dvec2 s = -1.0 + 2.0 * uv;
  s.x *= imgSize * 1.0 / imgSize;

  glm::dvec3 d = glm::normalize( s.x * u + s.y * v + f * w );

  d = d * 0.5 + 0.5;

  inputVals_[ 0 ] = d.x;
  inputVals_[ 1 ] = d.y;
  inputVals_[ 2 ] = d.z;

  return inputVals_;

} // IntersectionApp::inputFunction



////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::targetFunction
/// \return
////////////////////////////////////////////////////////////////////
std::vector< double >
IntersectionApp::targetFunction( )
{

  glm::dvec3 p ( 0.0, 0.0, 1.0 );

  glm::dvec3 d (
                inputVals_[ 0 ],
                inputVals_[ 1 ],
                inputVals_[ 2 ]
                );

  d = ( d - 0.5 ) * 2.0;

  // set eye point
  p *= eyeDist;

  glm::vec4 n = hit::intersectSphere( p, d );

  targetVals_[ 0 ] = n.w == std::numeric_limits< double >::infinity( ) ? -1.0 : 1.0;

  return targetVals_;

} // IntersectionApp::targetFunction



////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::onUserLoop
/// \param line
/// \return true if the user loop should continue, false otherwise
////////////////////////////////////////////////////////////////////
bool
IntersectionApp::onUserLoop( const std::string &line )
{

  bool breakLoop = false;

  //
  // check input
  //
  if ( line.length( ) > 0 )
  {

    switch ( line[ 0 ] )
    {

    case 'q':
      breakLoop = true;
      break;

    case 'w':
      focalPlane_ += 0.1;
      focalPlane_  = glm::min( focalPlane_, focalPlane + focalZoom );
      break;

    case 's':
      focalPlane_ -= 0.1;
      focalPlane_  = glm::max( focalPlane_, focalPlane - focalZoom );
      break;

    default:
      break;

    } // switch

    if ( breakLoop )
    {

      return false;

    }

  }

  glm::dvec3 p ( 0.0, 0.0, eyeDist );

  std::cout << "Zoom factor: " << focalPlane_ << std::endl;

  std::cout << "Expected Output: " << std::endl;

  // 2d to 1d index calculation
  auto index = [ ]( auto &x, auto &y ) -> auto {
                 return y * imgSize + x;
               };

  //
  // build calculated image
  //
  std::vector< char > image = buildImage( imgSize, imgSize, p, focalPlane_, true );

  for ( unsigned y = 0; y < imgSize; ++y )
  {

    for ( unsigned x = 0; x < imgSize; ++x )
    {

      char c = image[ index( x, y ) ];
      std::cout << c << ' ';

    }

    std::cout << std::endl;

  }

  std::cout << std::endl;

  std::cout << "Neural Net Output: " << std::endl;


  //
  // build expected image
  //
  image = buildImage( imgSize, imgSize, p, focalPlane_, false );

  for ( unsigned y = 0; y < imgSize; ++y )
  {

    for ( unsigned x = 0; x < imgSize; ++x )
    {

      char c = image[ index( x, y ) ];
      std::cout << c << ' ';

    }

    std::cout << std::endl;

  }

  std::cout << std::endl;

  std::cout << "'w' : zoom in, 's' : zoom out, 'q' : quit" << std::endl;

  return true;

} // IntersectionApp::onUserLoop



////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::buildImage
/// \param w
/// \param h
/// \param eye
/// \param pNet
/// \return
////////////////////////////////////////////////////////////////////
std::vector< char >
IntersectionApp::buildImage(
                            const unsigned    width,
                            const unsigned    height,
                            const glm::dvec3 &eye,
                            const double      fPlane,
                            bool              exact
                            )
{

  std::vector< char > image( width * height );

  std::vector< double > result;

  for ( unsigned y = 0; y < height; ++y )
  {

    for ( unsigned x = 0; x < width; ++x )
    {

      glm::dvec2 uv = glm::dvec2( x * 1.0 / width, y * 1.0 / height );

      glm::dvec3 up = glm::dvec3( 0.0, 1.0, 0.0 ); // up axis of world
      double f      = fPlane;                      // distance between eye and focal plane

      // camera basis
      glm::dvec3 w = glm::normalize( -eye );
      glm::dvec3 u = glm::normalize( cross( w, up ) );
      glm::dvec3 v = glm::normalize( cross( u, w ) );

      // pixel space of the focal plane
      glm::dvec2 p = -1.0 + 2.0 * uv;
      p.x *= width * 1.0 / height;

      glm::dvec3 d = glm::normalize( p.x * u + p.y * v + f * w );

      d = d * 0.5 + 0.5;

      inputVals_[ 0 ] = d.x;
      inputVals_[ 1 ] = d.y;
      inputVals_[ 2 ] = d.z;

      upNet_->feedForward( inputVals_ );

      if ( exact )
      {

        result = targetFunction( );

      }
      else
      {

        upNet_->getResults ( &result );

      }

      image[ y * width + x ] = ( result[ 0 ] > 0.0 ? '0' : ' ' );

    }

  }

  return image;

} // IntersectionApp::buildImage



////////////////////////////////////////////////////////////////////
/// \brief main
/// \return
////////////////////////////////////////////////////////////////////
int
main( )
{

  try
  {

    IntersectionApp app;
    app.run( );

  }
  catch ( const std::exception &e )
  {

    std::cerr << "Program failed: " << e.what( ) << std::endl;
    return EXIT_FAILURE;

  }

  return EXIT_SUCCESS;

} // main
