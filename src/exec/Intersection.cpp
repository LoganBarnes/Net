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

constexpr double eyeDist = 2.0;
constexpr double eyeZoom = 0.9; // used in division (can't be exactly zero)

constexpr unsigned imgSize = 10;

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
                                  net::Net         *pNet
                                  ,
                                  bool              exact
                                  );


protected:

private:

  std::default_random_engine gen_;
  std::uniform_real_distribution< double >  realDist_;
  std::uniform_int_distribution< unsigned > intDist_;

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
  , inputVals_ ( 4 )
  , targetVals_( 1 )
{}


////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::run
////////////////////////////////////////////////////////////////////
void
IntersectionApp::run( )
{

  net::ConnectedNet cnet( std::vector< unsigned >{ 4, 1 } );

  cnet.trainNet(
                std::bind( &IntersectionApp::inputFunction,  this ),
                std::bind( &IntersectionApp::targetFunction, this ),
                1.0e-5,
                100000
                );

  std::cout << std::endl;
  std::cout << "Done training (Error: ";
  std::cout << cnet.getAverageError( ) << ")" << std::endl;
  std::cout << std::endl;

  std::cout << "Results: " << std::endl;
  std::cout << std::endl;


  std::string line;

  bool breakLoop = false;
  double camDist = eyeDist;

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
        camDist -= 0.1;
        camDist  = glm::max( camDist, eyeDist - eyeZoom );
        break;

      case 's':
        camDist += 0.1;
        camDist  = glm::min( camDist, eyeDist + eyeZoom );
        break;

      default:
        break;

      } // switch

      if ( breakLoop )
      {

        break;

      }

    }

    glm::dvec3 p ( 0.0, 0.0, camDist );

    std::cout << "Zoom: " << p.z << std::endl;

    std::cout << "Expected Output: " << std::endl;


    //
    // build calculated image
    //
    std::vector< char > image = buildImage( imgSize, imgSize, p, &cnet, true );

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


    std::cout << "Actual Output: " << std::endl;


    //
    // build expected image
    //
    image = buildImage( imgSize, imgSize, p, &cnet, false );

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
  double f      = eyeDist;                     // distance between eye and focal plane

  // camera basis
  glm::dvec3 w = glm::normalize( -p );
  glm::dvec3 u = glm::normalize( cross( w, up ) );
  glm::dvec3 v = glm::normalize( cross( u, w ) );

  // pixel space of the focal plane
  glm::dvec2 s = -1.0 + 2.0 * uv;
  s.x *= imgSize * 1.0 / imgSize;

  glm::dvec3 d = glm::normalize( s.x * u + s.y * v + f * w );


  inputVals_[ 0 ] = d.x;
  inputVals_[ 1 ] = d.y;
  inputVals_[ 2 ] = d.z;
  inputVals_[ 3 ] = realDist_( gen_ );

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

  // set eye point
  p *= inputVals_[ 3 ] * eyeZoom + eyeDist;

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
                            net::Net         *pNet,
                            bool              exact
                            )
{

  std::vector< char > image( width * height );

  double distToEye = ( glm::length( eye ) - eyeDist ) / eyeZoom;
  std::vector< double > result;

  for ( unsigned y = 0; y < height; ++y )
  {

    for ( unsigned x = 0; x < width; ++x )
    {

      glm::dvec2 uv = glm::dvec2( x * 1.0 / width, y * 1.0 / height );

      glm::dvec3 up = glm::dvec3( 0.0, 1.0, 0.0 ); // up axis of world
      double d      = eyeDist;                         // distance between eye and focal plane

      // camera basis
      glm::dvec3 w = glm::normalize( -eye );
      glm::dvec3 u = glm::normalize( cross( w, up ) );
      glm::dvec3 v = glm::normalize( cross( u, w ) );

      // pixel space of the focal plane
      glm::dvec2 p = -1.0 + 2.0 * uv;
      p.x *= width * 1.0 / height;

      glm::dvec3 dir = glm::normalize( p.x * u + p.y * v + d * w );

      inputVals_[ 0 ] = dir.x;
      inputVals_[ 1 ] = dir.y;
      inputVals_[ 2 ] = dir.z;
      inputVals_[ 3 ] = distToEye;

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
