#pragma once

#include <vector>
#include <functional>

namespace net
{

/// \brief TrainFun
typedef std::function< std::vector< double >( ) > TrainFun;


////////////////////////////////////////////////////////////////////
/// \brief The Net class
////////////////////////////////////////////////////////////////////
class Net
{

public:

  virtual ~Net( ) = default;


  ////////////////////////////////////////////////////////////////////
  /// \brief trainNet
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
  void trainNet (
                 TrainFun       inputFun,                 ///< function to produce input values
                 TrainFun       targetFun,                ///< function to produce target values
                 const double   acceptableError = 1.0e-4, ///< lowest acceptable error value (defaults to 1.0e-4)
                 const unsigned printFrequency = 0        ///< number of iterations between informative print statements (defaults to 0 [no printing])
                 );


  ////////////////////////////////////////////////////////////////////
  /// \brief feedForward
  /// \param inputVals
  ////////////////////////////////////////////////////////////////////
  virtual
  void feedForward ( const std::vector< double > &inputVals ) = 0;

  ////////////////////////////////////////////////////////////////////
  /// \brief backProp
  /// \param targetVals
  ////////////////////////////////////////////////////////////////////
  virtual
  void backProp ( const std::vector< double > &targetVals ) = 0;

  ////////////////////////////////////////////////////////////////////
  /// \brief getResults
  /// \param pResultVals
  ////////////////////////////////////////////////////////////////////
  virtual
  void getResults ( std::vector< double > *pResultVals ) const = 0;

  ////////////////////////////////////////////////////////////////////
  /// \brief getAverageError
  /// \return
  ////////////////////////////////////////////////////////////////////
  virtual
  double getAverageError ( ) = 0;

};


}  // namespace net
