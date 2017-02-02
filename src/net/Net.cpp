#include "Net.hpp"
#include <iostream>


namespace net
{


////////////////////////////////////////////////////////////////////
/// \brief Net::trainNet
///
///        Attempts to train the neural net by continually
///        feeding forward input values and back propagating
///        target values
///
////////////////////////////////////////////////////////////////////
void
Net::trainNet(
              TrainFun       inputFun,        ///< function to produce input values
              TrainFun       targetFun,       ///< function to produce target values
              const double   acceptableError, ///< lowest acceptable error value (defaults to 1.0e-4)
              const unsigned printFrequency   ///< number of iterations between informative print statements (defaults to -1 [no printing])
              )
{

  unsigned counter = printFrequency;

  while ( getAverageError( ) > acceptableError )
  {

    feedForward( inputFun( ) );
    backProp   ( targetFun( ) );

    if ( printFrequency > 0 && ++counter >= printFrequency )
    {

      counter = 0;
      std::cout << "Error: " << getAverageError( ) << std::endl;

    }

  }

} // Net::trainNet



}
