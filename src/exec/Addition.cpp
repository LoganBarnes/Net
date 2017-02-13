// Addition.cpp

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


namespace
{

constexpr double mult = 0.5;
constexpr double sub  = 1.0;

}



////////////////////////////////////////////////////////////////////
/// \brief The AdditionApp class
////////////////////////////////////////////////////////////////////
class AdditionApp : public App
{

public:

  ////////////////////////////////////////////////////////////////////
  /// \brief AdditionApp
  ////////////////////////////////////////////////////////////////////
  AdditionApp( );

  ~AdditionApp( ) = default;

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
/// \brief AdditionApp::AdditionApp
////////////////////////////////////////////////////////////////////
AdditionApp::AdditionApp( )
  : App( std::vector< unsigned >{ 4, 5, 3, 1 } )
{}


////////////////////////////////////////////////////////////////////
/// \brief AdditionApp::inputFunction
/// \return
////////////////////////////////////////////////////////////////////
std::vector< double >
AdditionApp::inputFunction( )
{

  inputVals_.assign( 4, 0.0 );

  //
  // randomly choose the desired sum then
  // randomly select <sum> indices and change
  // them to one.
  //
  // This method produces less bias than
  // randomly choosing four binary numbers
  //
  std::unordered_set< unsigned > indices;
  unsigned sum = dist_( gen_ ) % 5;

  while ( indices.size( ) < sum )
  {

    unsigned index = dist_( gen_ ) % 4;
    indices.insert( index );
    inputVals_[ index ] = 1.0;

  }

  targetVals_[ 0 ] = mult * sum - sub;

  return inputVals_;

} // AdditionApp::inputFunction



////////////////////////////////////////////////////////////////////
/// \brief AdditionApp::targetFunction
/// \return
////////////////////////////////////////////////////////////////////
std::vector< double >
AdditionApp::targetFunction( )
{

  return targetVals_;

}



////////////////////////////////////////////////////////////////////
/// \brief AdditionApp::onUserLoop
/// \param line
/// \return true if the user loop should continue, false otherwise
////////////////////////////////////////////////////////////////////
bool
AdditionApp::onUserLoop( const std::string &line )
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

  App::printVector( resultVals, []( auto v ) { return std::abs( std::round( ( v + sub ) / mult ) ); } );

  std::cout << "\n'Enter' : new random input, 'q' : quit" << std::endl;

  return true;

} // AdditionApp::onUserLoop



////////////////////////////////////////////////////////////////////
/// \brief main
/// \return
////////////////////////////////////////////////////////////////////
int
main( )
{

  try
  {

    AdditionApp app;
    app.run( );

  }
  catch ( const std::exception &e )
  {

    std::cerr << "Program failed: " << e.what( ) << std::endl;
    return EXIT_FAILURE;

  }

  return EXIT_SUCCESS;

} // main
