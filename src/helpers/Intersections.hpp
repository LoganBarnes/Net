#pragma once

#include <limits>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"


namespace hit
{


////////////////////////////////////////////////////////////////
/// \brief cosine_sample_hemisphere
/// \return
////////////////////////////////////////////////////////////////
template< typename T >
glm::tvec3< T > cosine_sample_hemisphere (
                                          const glm::tvec3< T > &dir, ///< normal of hemisphere plane
                                          const T                z1,  ///< random value between 0.0 and 1.0 inclusive
                                          const T                z2   ///< random value between 0.0 and 1.0 inclusive
                                          );


////////////////////////////////////////////////////////////////
/// \brief intersectSphere
///
///        If the given ray intersects with the sphere, the
///        normal at the point of intersection along with the
///        distance to the point of intersection is returned
///        in the form of a vec4. The xyz components are the
///        normal and the w component is the distance. If there
///        is no intersection the distance will be infinity.
///
/// \return
////////////////////////////////////////////////////////////////
template< typename T >
glm::tvec4< T > intersectSphere (
                                 glm::tvec3< T > p, ///< ray origin point
                                 glm::tvec3< T > d  ///< ray direction vector
                                 );



} // namespace hit
