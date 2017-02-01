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
/// \param inputFun - function to produce input values
/// \param targetFun - function to produce target values
/// \param acceptableError - lowest acceptable error value
/// \param printFrequency - number of iterations between
///                         informative print statements
////////////////////////////////////////////////////////////////////
void
Net::trainNet(
              TrainFun inputFun,
              TrainFun targetFun,
              double   acceptableError,
              unsigned printFrequency
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
