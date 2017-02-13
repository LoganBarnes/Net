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

#include "App.hpp"



////////////////////////////////////////////////////////////////////
/// \brief The XORApp class
////////////////////////////////////////////////////////////////////
class XORApp : public App
{

public:

  ////////////////////////////////////////////////////////////////////
  /// \brief XORApp
  ////////////////////////////////////////////////////////////////////
  XORApp( );

  ~XORApp( ) = default;

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

};


////////////////////////////////////////////////////////////////////
/// \brief XORApp::XORApp
////////////////////////////////////////////////////////////////////
XORApp::XORApp( )
  : App( std::vector< unsigned >{ 2, 3, 1 } )
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
/// \brief XORApp::onUserLoop
/// \param line
/// \return true if the user loop should continue, false otherwise
////////////////////////////////////////////////////////////////////
bool
XORApp::onUserLoop( const std::string &line )
{

  if ( line == "q" )
  {

    return false;

  }

  //
  // display input
  //
  std::vector< double > inputVals = inputFunction( );

  std::cout << "Input:" << std::endl;

  App::printVector( inputVals, []( auto v ) { return std::round( v ); } );

  //
  // propogate
  //
  std::vector< double > resultVals;

  upNet_->feedForward( inputVals );
  upNet_->getResults ( &resultVals );

  //
  // display output
  //
  std::cout << "Output: ";

  App::printVector( resultVals, []( auto v ) { return std::abs( std::round( v ) ); } );

  std::cout << "\n'Enter' : new random input, 'q' : quit" << std::endl;

  return true;

} // XORApp::onUserLoop



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
