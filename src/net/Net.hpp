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

  virtual ~Net( ) {}


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
                 TrainFun inputFun,
                 TrainFun targetFun,
                 double   acceptableError = 1.0e-4,
                 unsigned printFrequency  = 0
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
