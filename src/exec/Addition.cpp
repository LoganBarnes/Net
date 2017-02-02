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

#include "ConnectedNet.hpp"


namespace
{

constexpr double mult = 0.5;
constexpr double sub  = 1.0;

}



////////////////////////////////////////////////////////////////////
/// \brief The AdditionApp class
////////////////////////////////////////////////////////////////////
class AdditionApp
{

public:

  ////////////////////////////////////////////////////////////////////
  /// \brief AdditionApp
  ////////////////////////////////////////////////////////////////////
  AdditionApp( );

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

  std::vector< double > inputVals_;
  std::vector< double > targetVals_;



};


////////////////////////////////////////////////////////////////////
/// \brief AdditionApp::AdditionApp
////////////////////////////////////////////////////////////////////
AdditionApp::AdditionApp( )
  : gen_       ( std::chrono::high_resolution_clock::now( ).time_since_epoch( ).count( ) )
  , dist_      ( 0, std::numeric_limits< unsigned >::max( ) )
  , inputVals_ ( 4 )
  , targetVals_( 1 )
{}


////////////////////////////////////////////////////////////////////
/// \brief AdditionApp::run
////////////////////////////////////////////////////////////////////
void
AdditionApp::run( )
{

  net::ConnectedNet cnet( std::vector< unsigned >{ 4, 5, 3, 1 } );

  cnet.trainNet(
                std::bind( &AdditionApp::inputFunction,  this ),
                std::bind( &AdditionApp::targetFunction, this ),
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

      std::cout << std::round( ( *iter + sub ) / mult ) << std::endl;

    }

    std::cout << std::endl;
    std::cout << "'Enter' : new random input, 'q' : quit" << std::endl;

  }
  while ( std::getline( std::cin, line ) );

} // AdditionApp::run



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
