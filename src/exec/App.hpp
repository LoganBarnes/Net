// XOR.hpp
#pragma once

#include <vector>
#include <random>
#include <iostream>

#include "ConnectedNet.hpp"


////////////////////////////////////////////////////////////////////
/// \brief The App class
////////////////////////////////////////////////////////////////////
class App
{

public:

  ////////////////////////////////////////////////////////////////////
  /// \brief App
  ////////////////////////////////////////////////////////////////////
  App(
      const std::vector< unsigned > &netTopology,
      double                         errorSmoothing = 0.9
      );

  virtual
  ~App( ) = default;

  ////////////////////////////////////////////////////////////////////
  /// \brief run
  ////////////////////////////////////////////////////////////////////
  virtual void run ( );


protected:

  ////////////////////////////////////////////////////////////////////
  /// \brief inputFunction
  /// \return
  ////////////////////////////////////////////////////////////////////
  virtual std::vector< double > inputFunction ( ) = 0;

  ////////////////////////////////////////////////////////////////////
  /// \brief targetFunction
  /// \return
  ////////////////////////////////////////////////////////////////////
  virtual std::vector< double > targetFunction ( ) = 0;

  ////////////////////////////////////////////////////////////////////
  /// \brief onUserLoop
  /// \param line
  /// \return true if the user loop should continue, false otherwise
  ////////////////////////////////////////////////////////////////////
  virtual bool onUserLoop ( const std::string &line ) = 0;


  ////////////////////////////////////////////////////////////////////
  /// \brief printVector
  /// \param vec
  /// \param valFunc
  ////////////////////////////////////////////////////////////////////
  template< typename T, typename F >
  static
  void printVector (
                    const std::vector< T > &vec,
                    F                       valFunc = [ ]( auto v ) { return v; }
                    );


  std::unique_ptr< net::ConnectedNet > upNet_;

  std::default_random_engine gen_;
  std::uniform_int_distribution< unsigned > dist_;

  std::vector< double > inputVals_;
  std::vector< double > targetVals_;


private:

};



////////////////////////////////////////////////////////////////////
/// \brief printVector
/// \param vec
/// \param valFunc
////////////////////////////////////////////////////////////////////
template< typename T, typename F >
void
App::printVector(
                 const std::vector< T > &vec,
                 F                       valFunc
                 )
{

  for ( auto & val : vec )
  {

    std::cout << valFunc( val ) << " ";

  }

  std::cout << std::endl;

}
