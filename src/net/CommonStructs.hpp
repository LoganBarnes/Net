#pragma once

#include <vector>

namespace net
{

struct Connection
{

  double weight;
  double deltaWeight;

};

class Neuron;

/// \brief Layer
typedef std::vector< Neuron > Layer;

} // namespace net
