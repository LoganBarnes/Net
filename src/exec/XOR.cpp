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
#include <chrono>
#include <string>

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
  void run( );

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

  std::vector< double > inputVals_;
  std::vector< double > targetVals_;



};


////////////////////////////////////////////////////////////////////
/// \brief XORApp::XORApp
////////////////////////////////////////////////////////////////////
XORApp::XORApp( )
  : gen_       ( static_cast< unsigned >( std::chrono::high_resolution_clock::now( ).time_since_epoch( ).count( ) ) )
  , dist_      ( 0, std::numeric_limits< unsigned >::max( ) )
  , inputVals_ ( 2 )
  , targetVals_( 1 )
{}



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
/// \brief XORApp::inputFunction
/// \return
////////////////////////////////////////////////////////////////////
std::vector< double >
XORApp::inputFunction( )
{

  int x = dist_( gen_ ) % 2;
  int y = dist_( gen_ ) % 2;

  inputVals_[ 0 ] = 1.0 * x;
  inputVals_[ 1 ] = 1.0 * y;

  targetVals_[ 0 ] = 1.0 * ( x ^ y );

  return inputVals_;

} // XORApp::inputFunction



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

  std::cout << std::endl;
  std::cout << "Done training (Error: ";
  std::cout << cnet.getAverageError( ) << ")" << std::endl;
  std::cout << std::endl;

  std::cout << "Results: " << std::endl;
  std::cout << std::endl;


  std::string line;
  std::vector< double > resultVals;

  //
  // test and display trained neural net on new random input
  //
  do
  {

    if ( line == "q" )
    {

      break;

    }

    std::vector< double > inputVals = inputFunction( );

    std::cout << "Input:" << std::endl;

    for ( auto &val : inputVals )
    {

      std::cout << std::round( val ) << " ";

    }

    std::cout << std::endl;


    cnet.feedForward( inputVals );
    cnet.getResults ( &resultVals );

    std::cout << "Output: ";

    for ( auto &val : resultVals )
    {

      std::cout << std::abs( std::round( val ) ) << std::endl;

    }

    std::cout << std::endl;
    std::cout << "'Enter' : new random input, 'q' : quit" << std::endl;

  }
  while ( std::getline( std::cin, line ) );

} // XORApp::run



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
