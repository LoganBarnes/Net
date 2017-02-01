// XOR.cpp

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



////////////////////////////////////////////////////////////////////
/// \brief The XORApp class
////////////////////////////////////////////////////////////////////
class XORApp
{

public:

  ////////////////////////////////////////////////////////////////////
  /// \brief XORApp
  ////////////////////////////////////////////////////////////////////
  XORApp( );

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
  std::uniform_int_distribution< unsigned > dist_;

  std::vector< double > targetVals_;



};


////////////////////////////////////////////////////////////////////
/// \brief XORApp::XORApp
////////////////////////////////////////////////////////////////////
XORApp::XORApp( )
  : gen_       ( std::chrono::high_resolution_clock::now( ).time_since_epoch( ).count( ) )
  , dist_      ( 0, std::numeric_limits< unsigned >::max( ) )
  , targetVals_( 1 )
{}


////////////////////////////////////////////////////////////////////
/// \brief XORApp::run
////////////////////////////////////////////////////////////////////
void
XORApp::run( )
{

  net::ConnectedNet cnet( std::vector< unsigned >{ 2, 3, 1 } );

  cnet.trainNet(
                std::bind( &XORApp::inputFunction,  this ),
                std::bind( &XORApp::targetFunction, this ),
                1.0e-4,
                10000
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

      std::cout << std::round( *iter ) << " ";

    }

    std::cout << std::endl;


    cnet.feedForward( inputVals );
    cnet.getResults ( &resultVals );

    std::cout << "Output: ";

    for ( auto iter = std::begin( resultVals ); iter != std::end( resultVals ); ++iter )
    {

      std::cout << std::abs( std::round( *iter ) ) << std::endl;

    }

  }

} // XORApp::run



////////////////////////////////////////////////////////////////////
/// \brief XORApp::inputFunction
/// \return
////////////////////////////////////////////////////////////////////
std::vector< double >
XORApp::inputFunction( )
{

  int x = dist_( gen_ ) % 2;
  int y = dist_( gen_ ) % 2;

  std::vector< double > inputVals
  {
    1.0 * x,
    1.0 * y
  };

  targetVals_[ 0 ] = 1.0 * ( x ^ y );

  return inputVals;

} // XORApp::inputFunction



////////////////////////////////////////////////////////////////////
/// \brief XORApp::targetFunction
/// \return
////////////////////////////////////////////////////////////////////
std::vector< double >
XORApp::targetFunction( )
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

    XORApp app;
    app.run( );

  }
  catch ( const std::exception &e )
  {

    std::cerr << "Program failed: " << e.what( ) << std::endl;
    return EXIT_FAILURE;

  }

  return EXIT_SUCCESS;

} // main
