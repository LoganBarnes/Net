#include "Neuron.hpp"
#include <algorithm>
#include <chrono>


namespace net
{

//
// global static variables
//
namespace
{

constexpr double eta   = 0.15; // overall net training rate [0.0, 1.0]
constexpr double alpha = 0.5;  // momentum - multiplier of last weight change [0.0, n]

auto seed = std::chrono::high_resolution_clock::now( ).time_since_epoch( ).count( );
std::default_random_engine generator( static_cast< unsigned >( seed ) );
std::uniform_real_distribution< double > distribution( 0.0, 1.0 );


}


////////////////////////////////////////////////////////////////////
/// \brief Neuron::Neuron
////////////////////////////////////////////////////////////////////
Neuron::Neuron(
               unsigned numOutputs,
               unsigned myIndex
               )
  : myIndex_( myIndex )
{

  for ( unsigned c = 0; c < numOutputs; ++c )
  {

    outputWeights_.push_back( Connection( ) );

    outputWeights_.back( ).weight = Neuron::randomWeight( );

  }

}



////////////////////////////////////////////////////////////////////
/// \brief feedForward
/// \param prevLayer
////////////////////////////////////////////////////////////////////
void
Neuron::feedForward( const Layer &prevLayer )
{

  double sum = 0.0;

  std::for_each (
                 std::begin( prevLayer ),
                 std::end( prevLayer ),
                 [ this, &sum ]( const Neuron &neuron )
    {

      sum += neuron.getOutputVal( ) * neuron.outputWeights_[ myIndex_ ].weight;

    } );

  outputVal_ = Neuron::transferFunction( sum );

}



////////////////////////////////////////////////////////////////////
/// \brief Neuron::_sumDOW
/// \param nextLayer
/// \return
////////////////////////////////////////////////////////////////////
double
Neuron::_sumDOW( const Layer &nextLayer ) const
{

  double sum = 0.0;

  for ( unsigned n = 0; n < nextLayer.size( ) - 1; ++n )
  {

    sum += outputWeights_[ n ].weight * nextLayer[ n ].gradient_;

  }

  return sum;

} // Neuron::sumDOW



////////////////////////////////////////////////////////////////////
/// \brief Neuron::calcOutputGradients
/// \param targetVal
////////////////////////////////////////////////////////////////////
void
Neuron::calcOutputGradients( double targetVal )
{

  double delta = targetVal - outputVal_;

  gradient_ = delta * Neuron::transferFunctionDerivative( outputVal_ );

}



////////////////////////////////////////////////////////////////////
/// \brief Neuron::calcHiddenGradients
/// \param nextLayer
////////////////////////////////////////////////////////////////////
void
Neuron::calcHiddenGradients( const Layer &nextLayer )
{

  double dow = _sumDOW( nextLayer );

  gradient_ = dow * Neuron::transferFunctionDerivative( outputVal_ );

}



////////////////////////////////////////////////////////////////////
/// \brief Neuron::updateInputWeights
/// \param prevLayer
////////////////////////////////////////////////////////////////////
void
Neuron::updateInputWeights( Layer &prevLayer )
{

  std::for_each(
                std::begin( prevLayer ),
                std::end( prevLayer ),
                [ this ]( Neuron &neuron )
  {

    double oldDeltaWeight = neuron.outputWeights_[ myIndex_ ].deltaWeight;

    double newDeltaWeight =
      // individual input magnified by the gradient and train rate
      eta
      * neuron.getOutputVal( )
      * gradient_
      // momentum - a fraction of the previous delta weight
      + alpha
      * oldDeltaWeight;

    neuron.outputWeights_[ myIndex_ ].deltaWeight = newDeltaWeight;
    neuron.outputWeights_[ myIndex_ ].weight     += newDeltaWeight;

  } );

} // Neuron::updateInputWeights



////////////////////////////////////////////////////////////////////
/// \brief Neuron::randomWeight
/// \return
////////////////////////////////////////////////////////////////////
double
Neuron::randomWeight( )
{

  return distribution( generator );

}



////////////////////////////////////////////////////////////////////
/// \brief Neuron::transferFunction
/// \param x
/// \return
////////////////////////////////////////////////////////////////////
double
Neuron::transferFunction( double x )
{

  //
  // tanh - output range [ -1.0, 1.0 ]
  //
  return std::tanh( x );

}



////////////////////////////////////////////////////////////////////
/// \brief Neuron::transferFunctionDerivative
/// \param x
/// \return
////////////////////////////////////////////////////////////////////
double
Neuron::transferFunctionDerivative( double x )
{

  //
  // tanh derivative approximation
  //
  return 1.0 - x * x;

}



} // namespace net
