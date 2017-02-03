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

#include "glm/gtx/string_cast.hpp"
#include "Intersections.hpp"

#include "ConnectedNet.hpp"



namespace
{

constexpr double eyeDist   = 3.0;
constexpr double focalDist = eyeDist * 0.5;
constexpr double focalZoom = 1.0;

constexpr unsigned imgSize = 8;

}


////////////////////////////////////////////////////////////////////
/// \brief The IntersectionApp class
////////////////////////////////////////////////////////////////////
class IntersectionApp
{

public:

  ////////////////////////////////////////////////////////////////////
  /// \brief IntersectionApp
  ////////////////////////////////////////////////////////////////////
  IntersectionApp( );

  ////////////////////////////////////////////////////////////////////
  /// \brief run
  ////////////////////////////////////////////////////////////////////
  void run ( );

  ////////////////////////////////////////////////////////////////////
  /// \brief inputFunction
  /// \return
  ////////////////////////////////////////////////////////////////////
  std::vector< double > inputFunction ( );

  ////////////////////////////////////////////////////////////////////
  /// \brief targetFunction
  /// \return
  ////////////////////////////////////////////////////////////////////
  std::vector< double > targetFunction ( );


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
                                  const double      fDist,
                                  net::Net         *pNet,
                                  bool              exact
                                  );


protected:

private:

  std::default_random_engine gen_;
  std::uniform_real_distribution< double >  realDist_;
  std::uniform_int_distribution< unsigned > intDist_;

  double focalDist_;

  std::vector< double > inputVals_;
  std::vector< double > targetVals_;



};


////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::IntersectionApp
////////////////////////////////////////////////////////////////////
IntersectionApp::IntersectionApp( )
  : gen_       ( std::chrono::high_resolution_clock::now( ).time_since_epoch( ).count( ) )
  , realDist_  ( -1.0, 1.0 )
  , intDist_   ( 0, imgSize )
  , focalDist_ ( focalDist )
  , inputVals_ ( 4, 0.0 )
  , targetVals_( 1 )
{}


////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::run
////////////////////////////////////////////////////////////////////
void
IntersectionApp::run( )
{

  net::ConnectedNet cnet( std::vector< unsigned >{ 4, 5, 1 }, 0.99 );

  cnet.trainNet(
                std::bind( &IntersectionApp::inputFunction,  this ),
                std::bind( &IntersectionApp::targetFunction, this ),
                1.0e-4,
                10000
                );

  std::cout << std::endl;
  std::cout << "Done training (Error: ";
  std::cout << cnet.getAverageError( ) << ")" << std::endl;
  std::cout << std::endl;

  std::cout << "Results: " << std::endl;
  std::cout << std::endl;


  std::string line;

  bool breakLoop = false;
  double fDist   = focalDist;

  //
  // test and display trained neural net on new random input
  //
  do
  {

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
        fDist += 0.1;
        fDist  = glm::min( fDist, focalDist + focalZoom );
        break;

      case 's':
        fDist -= 0.1;
        fDist  = glm::max( fDist, focalDist - focalZoom );
        break;

      default:
        break;

      } // switch

      if ( breakLoop )
      {

        break;

      }

    }

    glm::dvec3 p ( 0.0, 0.0, eyeDist );

    std::cout << "Zoom factor: " << fDist << std::endl;

    std::cout << "Expected Output: " << std::endl;


    //
    // build calculated image
    //
    std::vector< char > image = buildImage( imgSize, imgSize, p, fDist, &cnet, true );

    for ( unsigned y = 0; y < imgSize; ++y )
    {

      for ( unsigned x = 0; x < imgSize; ++x )
      {

        char c = image[ y * imgSize + x ];
        std::cout << c << ' ';

      }

      std::cout << std::endl;

    }

    std::cout << std::endl;

    std::cout << "Neural Net Output: " << std::endl;


    //
    // build expected image
    //
    image = buildImage( imgSize, imgSize, p, fDist, &cnet, false );

    for ( unsigned y = 0; y < imgSize; ++y )
    {

      for ( unsigned x = 0; x < imgSize; ++x )
      {

        char c = image[ y * imgSize + x ];
        std::cout << c << ' ';

      }

      std::cout << std::endl;

    }

    std::cout << std::endl;

    std::cout << "'w' : zoom in, 's' : zoom out, 'q' : quit" << std::endl;

  }
  while ( std::getline( std::cin, line ) );


} // IntersectionApp::run



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
  double f      = focalDist;                   // distance between eye and focal plane

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
                            const double      fDist,
                            net::Net         *pNet,
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
      double f      = fDist;                       // distance between eye and focal plane

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

      pNet->feedForward( inputVals_ );

      if ( exact )
      {

        result = targetFunction( );

      }
      else
      {

        pNet->getResults ( &result );

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
