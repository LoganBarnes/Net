#include "ConnectedNet.hpp"

#include <random>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <chrono>

#include "Neuron.hpp"

namespace net
{


////////////////////////////////////////////////////////////////////
/// \brief The NetImpl class
////////////////////////////////////////////////////////////////////
class NetImpl : public Net
{

public:

  ////////////////////////////////////////////////////////////////////
  /// \brief NetImpl
  /// \param topology
  ////////////////////////////////////////////////////////////////////
  NetImpl(
          const std::vector< unsigned > &topology,
          double                         errorSmoothing
          );

  ////////////////////////////////////////////////////////////////////
  /// \brief feedForward
  /// \param inputVals
  ////////////////////////////////////////////////////////////////////
  virtual
  void feedForward ( const std::vector< double > &inputVals ) final;

  ////////////////////////////////////////////////////////////////////
  /// \brief backProp
  /// \param targetVals
  ////////////////////////////////////////////////////////////////////
  virtual
  void backProp ( const std::vector< double > &targetVals ) final;

  ////////////////////////////////////////////////////////////////////
  /// \brief getResults
  /// \param pResultVals
  ////////////////////////////////////////////////////////////////////
  virtual
  void getResults ( std::vector< double > *pResultVals ) const final;

  ////////////////////////////////////////////////////////////////////
  /// \brief getAverageError
  /// \return
  ////////////////////////////////////////////////////////////////////
  virtual
  double getAverageError ( ) final;


protected:

private:

  /// \brief m_layers
  std::vector< Layer > m_layers; // m_layers[ layerNum ][ neuronNum ]

  double m_error;
  double m_recentAverageError;
  double m_recentAverageSmoothingFactor;

};



////////////////////////////////////////////////////////////////////
/// \brief NetImpl::NetImpl
////////////////////////////////////////////////////////////////////
NetImpl::NetImpl(
                 const std::vector< unsigned > &topology,
                 double                         errorSmoothing
                 )
  : m_error( 0.0 )
  , m_recentAverageError( 1.0 )
  , m_recentAverageSmoothingFactor( errorSmoothing )
{

  unsigned numLayers = topology.size( );

  for ( unsigned layerNum = 0; layerNum < numLayers; ++layerNum )
  {

    m_layers.push_back( Layer( ) );
    unsigned numOutputs = ( layerNum == topology.size( ) - 1 ? 0 : topology[ layerNum + 1 ] );

    // fill layer with neurons and add bias neuron to the layer
    for ( unsigned neuronNum = 0; neuronNum <= topology[ layerNum ]; ++neuronNum )
    {

      Neuron neuron( numOutputs, neuronNum );
      m_layers.back( ).push_back( std::move( neuron ) );

    }

    //
    // force the bias node's output value to 1.0
    //
    m_layers.back( ).back( ).setOutputVal( 1.0 );

  }

}



////////////////////////////////////////////////////////////////////
/// \brief NetImpl::feedForward
/// \param inputVals
////////////////////////////////////////////////////////////////////
void
NetImpl::feedForward( const std::vector< double > &inputVals )
{

  assert( inputVals.size( ) == m_layers[ 0 ].size( ) - 1 );

  //
  // assign (latch) the input values into the input neurons
  //
  for ( unsigned i = 0; i < inputVals.size( ); ++i )
  {

    m_layers[ 0 ][ i ].setOutputVal( inputVals[ i ] );

  }

  //
  // forward propogate
  //
  // start at one because first layer already set
  for ( unsigned layerNum = 1; layerNum < m_layers.size( ); ++layerNum )
  {

    Layer &prevLayer = m_layers[ layerNum - 1 ];

    for ( unsigned n = 0; n < m_layers[ layerNum ].size( ) - 1; ++n )
    {

      m_layers[ layerNum ][ n ].feedForward( prevLayer );

    }

  }

} // NetImpl::feedForward



////////////////////////////////////////////////////////////////////
/// \brief NetImpl::backProp
/// \param targetVals
////////////////////////////////////////////////////////////////////
void
NetImpl::backProp( const std::vector< double > &targetVals )
{

  //
  // calculate overall net error (RMS of output neuron errors)
  //
  Layer &outputLayer = m_layers.back( );

  assert( targetVals.size( ) == outputLayer.size( ) - 1 );

  // root mean square error
  m_error = 0.0;

  for ( unsigned n = 0; n < outputLayer.size( ) - 1; ++n )
  {

    double delta = targetVals[ n ] - outputLayer[ n ].getOutputVal( );
    m_error += delta * delta;

  }

  m_error /= outputLayer.size( ) - 1;
  m_error  = std::sqrt( m_error );

  // recent average measurement
  m_recentAverageError = ( m_recentAverageError * m_recentAverageSmoothingFactor )
                         + ( m_error * ( 1.0 - m_recentAverageSmoothingFactor ) );


  //
  // calculate output layer gradients
  //
  for ( unsigned n = 0; n < outputLayer.size( ) - 1; ++n )
  {

    outputLayer[ n ].calcOutputGradients( targetVals[ n ] );

  }

  //
  // calculate gradients on hidden layers
  //
  for ( unsigned layerNum = m_layers.size( ) - 2; layerNum > 0; --layerNum )
  {

    Layer &hiddenLayer = m_layers[ layerNum     ];
    Layer &nextLayer   = m_layers[ layerNum + 1 ];

    for ( unsigned n = 0; n < hiddenLayer.size( ); ++n )
    {

      hiddenLayer[ n ].calcHiddenGradients( nextLayer );

    }

  }

  //
  // update connection weights
  //
  for ( unsigned layerNum = m_layers.size( ) - 1; layerNum > 0; --layerNum )
  {

    Layer &layer     = m_layers[ layerNum     ];
    Layer &prevLayer = m_layers[ layerNum - 1 ];

    for ( unsigned n = 0; n < layer.size( ) - 1; ++n )
    {

      layer[ n ].updateInputWeights( prevLayer );

    }

  }

} // NetImpl::backProp



////////////////////////////////////////////////////////////////////
/// \brief NetImpl::getResults
/// \param resultVals - vector to be filled with output values
///                     computed by the neural net
////////////////////////////////////////////////////////////////////
void
NetImpl::getResults( std::vector< double > *pResultVals ) const
{

  pResultVals->clear( );

  for ( unsigned n = 0; n < m_layers.back( ).size( ) - 1; ++n )
  {

    pResultVals->push_back( m_layers.back( )[ n ].getOutputVal( ) );

  }

}



////////////////////////////////////////////////////////////////////
/// \brief NetImpl::getAverageError
/// \return - smoothed current average error for neural net
////////////////////////////////////////////////////////////////////
double
NetImpl::getAverageError( )
{

  return m_recentAverageError;

}



////////////////////////////////////////////////////////////////////
/// \brief ConnectedNet::ConnectedNet
///
///        Simple API wrapper around actual implementation class
///
////////////////////////////////////////////////////////////////////
ConnectedNet::ConnectedNet(
                           const std::vector< unsigned > &topology,
                           double                         errorSmoothing
                           )
  : netImpl_( new NetImpl( topology, errorSmoothing ) )
{}



////////////////////////////////////////////////////////////////////
/// \brief ConnectedNet::feedForward
///
///        Simple API wrapper around actual implementation class
///
/// \param inputVals
////////////////////////////////////////////////////////////////////
void
ConnectedNet::feedForward( const std::vector< double > &inputVals )
{

  netImpl_->feedForward( inputVals );

} // ConnectedNet::feedForward



////////////////////////////////////////////////////////////////////
/// \brief ConnectedNet::backProp
///
///        Simple API wrapper around actual implementation class
///
/// \param targetVals
////////////////////////////////////////////////////////////////////
void
ConnectedNet::backProp( const std::vector< double > &targetVals )
{

  netImpl_->backProp( targetVals );

} // ConnectedNet::backProp



////////////////////////////////////////////////////////////////////
/// \brief ConnectedNet::getResults
///
///        Simple API wrapper around actual implementation class
///
/// \param pResultVals
////////////////////////////////////////////////////////////////////
void
ConnectedNet::getResults( std::vector< double > *pResultVals ) const
{

  netImpl_->getResults( pResultVals );

}



////////////////////////////////////////////////////////////////////
/// \brief ConnectedNet::getAverageError
///
///        Simple API wrapper around actual implementation class
///
/// \return
////////////////////////////////////////////////////////////////////
double
ConnectedNet::getAverageError( )
{

  return netImpl_->getAverageError( );

}



} // namespace net
