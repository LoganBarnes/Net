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

#include "ConnectedNet.hpp"


namespace
{

////////////////////////////////////////////////////////////////////
/// \brief The Vec struct
////////////////////////////////////////////////////////////////////
struct Vec
{

  double x, y, z;

};


////////////////////////////////////////////////////////////////////
/// \brief dot
/// \param v1
/// \param v2
/// \return
////////////////////////////////////////////////////////////////////
inline
double
dot(
    const Vec &v1,
    const Vec &v2
    )
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}



////////////////////////////////////////////////////////////////////
/// \brief length
/// \param v
/// \return
////////////////////////////////////////////////////////////////////
inline
double
length( const Vec &v )
{
  return std::sqrt( dot( v, v ) );
}



////////////////////////////////////////////////////////////////////
/// \brief normalize
/// \param v
/// \return
////////////////////////////////////////////////////////////////////
inline
Vec
normalize( const Vec &v )
{
  double mag = length( v );

  Vec norm;

  norm.x = v.x / mag;
  norm.y = v.y / mag;
  norm.z = v.z / mag;

  return norm;

}



////////////////////////////////////////////////////////////////////
/// \brief operator *=
/// \param v
/// \param d
////////////////////////////////////////////////////////////////////
inline
void
operator*=(
           Vec          &v,
           const double &d
           )
{
  v.x *= d;
  v.y *= d;
  v.z *= d;
}



////////////////////////////////////////////////////////////////////
/// \brief operator +=
/// \param v
/// \param d
////////////////////////////////////////////////////////////////////
inline
void
operator+=(
           Vec          &v,
           const double &d
           )
{
  v.x += d;
  v.y += d;
  v.z += d;
}



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


protected:

private:

  std::default_random_engine gen_;
  std::uniform_real_distribution< double > dist_;

  std::vector< double > targetVals_;



};


////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::IntersectionApp
////////////////////////////////////////////////////////////////////
IntersectionApp::IntersectionApp( )
  : gen_       ( std::chrono::high_resolution_clock::now( ).time_since_epoch( ).count( ) )
  , dist_      ( -1.0, 1.0 )
  , targetVals_( 1 )
{}


////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::run
////////////////////////////////////////////////////////////////////
void
IntersectionApp::run( )
{

  net::ConnectedNet cnet( std::vector< unsigned >{ 6, 7, 4, 1 } );

  cnet.trainNet(
                std::bind( &IntersectionApp::inputFunction,  this ),
                std::bind( &IntersectionApp::targetFunction, this ),
                1.0e-3,
                100000
                );

  std::cout << "Done training" << std::endl;
  std::cout << "Error: " << cnet.getAverageError( ) << std::endl;


  std::string line;
  std::vector< double > resultVals;

  //
  // test and display trained neural net on new random input
  //
  while ( std::getline( std::cin, line ) )
  {

    if ( line == "q"
        || line == "quit"
        || line == "exit" )
    {
      break;
    }

    std::vector< double > inputVals = inputFunction( );

    std::cout << "Input:" << std::endl;

    for ( auto iter = std::begin( inputVals ); iter != std::end( inputVals ); ++iter )
    {

      std::cout << ( *iter - 0.5 ) * 2.0 << " ";

    }

    std::cout << std::endl;

    cnet.feedForward( inputVals );
    cnet.getResults ( &resultVals );

    Vec n, w_i;
    n.x   = inputVals[ 0 ];
    n.y   = inputVals[ 1 ];
    n.z   = inputVals[ 2 ];
    w_i.x = inputVals[ 3 ];
    w_i.y = inputVals[ 4 ];
    w_i.z = inputVals[ 5 ];

    n   += -0.5;
    w_i += -0.5;

    n   *= 2.0;
    w_i *= 2.0;

    double d = dot( n, w_i );

    std::cout << "Output: ";

    for ( auto iter = std::begin( resultVals ); iter != std::end( resultVals ); ++iter )
    {

      std::cout << ( *iter  + 1.0 ) * 0.5 << std::endl;
      std::cout << d << std::endl;

    }

  }

} // IntersectionApp::run



////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::inputFunction
/// \return
////////////////////////////////////////////////////////////////////
std::vector< double >
IntersectionApp::inputFunction( )
{

  Vec n, w_i;

  n.x   = dist_( gen_ );
  n.y   = dist_( gen_ );
  n.z   = dist_( gen_ );
  w_i.x = dist_( gen_ );
  w_i.y = dist_( gen_ );
  w_i.z = dist_( gen_ );

  // normalize
  n = normalize( n );

  w_i = normalize( w_i );

  if ( dot( n, w_i ) < 0.0 )
  {
    n *= -1.0;
  }

  n   *= 0.5;
  w_i *= 0.5;

  n   += 0.5;
  w_i += 0.5;

  std::vector< double > inputVals
  {
    n.x,
    n.y,
    n.z,
    w_i.x,
    w_i.y,
    w_i.z
  };

  targetVals_[ 0 ] = dot( n, w_i ) * 2.0 - 1.0;

  return inputVals;

} // IntersectionApp::inputFunction



////////////////////////////////////////////////////////////////////
/// \brief IntersectionApp::targetFunction
/// \return
////////////////////////////////////////////////////////////////////
std::vector< double >
IntersectionApp::targetFunction( )
{

  return targetVals_;

}



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
