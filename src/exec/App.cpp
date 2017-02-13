// XOR.cpp
#include "App.hpp"

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


////////////////////////////////////////////////////////////////////
/// \brief App::App
////////////////////////////////////////////////////////////////////
App::App(
         const std::vector< unsigned > &netTopology,
         double                         errorSmoothing
         )
  : upNet_      ( new net::ConnectedNet( netTopology, errorSmoothing ) ) // will throw error if topology is empty
  , gen_        ( static_cast< unsigned >( std::chrono::high_resolution_clock::now( ).
                                          time_since_epoch( ).count( ) ) )
  , dist_       ( 0, std::numeric_limits< unsigned >::max( ) )
  , inputVals_  ( netTopology.front( ) )
  , targetVals_ ( netTopology.back( ) )
{}


////////////////////////////////////////////////////////////////////
/// \brief App::run
////////////////////////////////////////////////////////////////////
void
App::run( )
{

  upNet_->trainNet(
                   std::bind( &App::inputFunction,  this ),
                   std::bind( &App::targetFunction, this ),
                   1.0e-4,
                   10000
                   );

  std::cout << std::endl;
  std::cout << "Done training (Error: ";
  std::cout << upNet_->getAverageError( ) << ")" << std::endl;
  std::cout << std::endl;

  std::cout << "Results: " << std::endl;
  std::cout << std::endl;


  std::string line{ "" };

  //
  // test and display trained neural net on new random input
  //
  while ( onUserLoop( line ) && std::getline( std::cin, line ) );

} // App::run
