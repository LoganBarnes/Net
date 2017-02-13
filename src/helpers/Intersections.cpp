#include "Intersections.hpp"

#include <limits>
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"


namespace hit
{



namespace
{



////////////////////////////////////////////////////////////////
/// \brief createONB
///
///        Create Orthonormal Basis from normalized vector
////////////////////////////////////////////////////////////////
template< typename T >
void
createONB(
          const glm::tvec3< T > &n,  ///< normal
          glm::tvec3< T >       *pU, ///< output U vector
          glm::tvec3< T >       *pV  ///< output V vector
          )
{

  glm::tvec3< T > &U = *pU;

  U = glm::cross( n, glm::tvec3< T >( 0.0, 1.0, 0.0 ) );

  if ( glm::dot( U, U ) < T( 1.e-3 ) )
  {

    U = glm::cross( n, glm::tvec3< T >( 1.0, 0.0, 0.0 ) );

  }

  *pU = glm::normalize( U );
  *pV = glm::cross( n, U );

} // createONB



////////////////////////////////////////////////////////////////
/// \brief cosine_sample_hemisphere_z
/// \param u1
/// \param u2
/// \return
////////////////////////////////////////////////////////////////
template< typename T >
glm::tvec3< T >
cosine_sample_hemisphere_z(
                           const T u1, ///< random value between 0.0 and 1.0 inclusive
                           const T u2  ///< random value between 0.0 and 1.0 inclusive
                           )
{

  glm::tvec3< T > p;

  // Uniformly sample disk.
  const T r   = glm::sqrt( u1 );
  const T phi = T( 2.0 ) * glm::pi< T >( ) * u2;
  p.x = r * glm::cos( phi );
  p.y = r * glm::sin( phi );

  // Project up to hemisphere.
  p.z = glm::sqrt( glm::max( T( 0.0 ), T( 1.0 ) - p.x * p.x - p.y * p.y ) );

  return p;

}



////////////////////////////////////////////////////////////////
/// \brief solveQuadratic
/// \param a
/// \param b
/// \param c
/// \param pT1
/// \param pT2
/// \return
////////////////////////////////////////////////////////////////
template< typename T >
auto
solveQuadratic(
               const T a,   ///< quadratic term
               const T b,   ///< linear term
               const T c,   ///< constant term
               T      *pT1, ///< first solution (if it exists)
               T      *pT2  ///< second solution (if it exists)
               )
{

  constexpr T EPS = T( 1.0e-5 );
  constexpr T two = T( 2.0 );

  if ( glm::abs( a ) < EPS )
  {

    *pT1 = -c / b;
    return 1;

  }

  T disc = b * b - T( 4.0 ) * a * c;

  // one solution
  if ( glm::abs( disc ) < EPS )
  {

    *pT1 = -b / ( two * a );
    return 1;

  }

  // no solutions
  if ( disc < T( 0.0 ) )
  {

    return 0;

  }

  // two solutions (disc > 0)
  *pT1 = ( -b + glm::sqrt( disc ) ) / ( two * a );
  *pT2 = ( -b - glm::sqrt( disc ) ) / ( two * a );
  return 2;

} // solveQuadratic



////////////////////////////////////////////////////////////////
/// \brief solveQuadraticA1No2
/// \param b
/// \param c
/// \param pT1
/// \param pT2
/// \return
////////////////////////////////////////////////////////////////
template< typename T >
auto
solveQuadraticA1No2(
                    const T b,   ///< linear term
                    const T c,   ///< constant term
                    T      *pT1, ///< first solution (if it exists)
                    T      *pT2  ///< second solution (if it exists)
                    )
{

  constexpr T EPS = T( 1.0e-5 );

  T disc = b * b - c;

  // one solution
  if ( glm::abs( disc ) < EPS )
  {

    *pT1 = -b;
    return 1;

  }

  // no solutions
  if ( disc < T( 0.0 ) )
  {

    return 0;

  }

  // two solutions (disc > 0)
  *pT1 = ( -b + glm::sqrt( disc ) );
  *pT2 = ( -b - glm::sqrt( disc ) );
  return 2;

} // solveQuadratic



} // namespace



////////////////////////////////////////////////////////////////
/// \brief cosine_sample_hemisphere
/// \return
////////////////////////////////////////////////////////////////
template< typename T >
glm::tvec3< T >
cosine_sample_hemisphere(
                         const glm::tvec3< T > &dir, ///< normal of hemisphere plane
                         const T                z1,  ///< random value between 0.0 and 1.0 inclusive
                         const T                z2   ///< random value between 0.0 and 1.0 inclusive
                         )
{

  glm::tvec3< T > p = cosine_sample_hemisphere_z( z1, z2 );

  glm::tvec3< T > v1, v2;
  createONB( dir, &v1, &v2 );

  return v1 * p.x + v2 * p.y + dir * p.z;

}



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
glm::tvec4< T >
intersectSphere(
                const glm::tvec3< T > p, ///< ray origin point
                const glm::tvec3< T > d  ///< ray direction vector
                )
{

  constexpr T INF    = std::numeric_limits< T >::infinity( );
  constexpr T radius = 1.0;

  glm::tvec4< T > n = glm::tvec4< T >( 0.0, 0.0, 0.0, INF );


  T b = glm::dot( p, d );
  T c = glm::dot( p, p ) - radius * radius;


  glm::tvec4< T > v;
  T t1, t2;
  auto solutions = solveQuadraticA1No2< T >( b, c, &t1, &t2 );

  if ( solutions > 0 )
  {

    v = glm::tvec4< T >( p, 1.0 ) + glm::tvec4< T >( d, 0.0 ) * t1;

    if ( t1 < 0.0 )
    {

      t1 = INF;

    }

    if ( t1 < n.w )
    {

      v.w = t1;
      n   = v;

    }

    if ( solutions > 1 )
    {

      v = glm::tvec4< T >( p, 1.0 ) + glm::tvec4< T >( d, 0.0 ) * t2;

      if ( t2 < 0.0 )
      {

        t2 = INF;

      }

      if ( t2 < n.w )
      {

        v.w = t2;
        n   = v;

      }

    }

  }

  return n;

} // intersectSphere



//
// define allowed templated functions
//

template
glm::vec3 cosine_sample_hemisphere< float >(
                                            const glm::vec3 &dir,
                                            const float      z1,
                                            const float      z2
                                            );

template
glm::dvec3 cosine_sample_hemisphere< double >(
                                              const glm::dvec3 &dir,
                                              const double      z1,
                                              const double      z2
                                              );

template
glm::vec4 intersectSphere< float >(
                                   const glm::vec3 p,
                                   const glm::vec3 d
                                   );

template
glm::dvec4 intersectSphere< double >(
                                     const glm::dvec3 p,
                                     const glm::dvec3 d
                                     );


} // namespace hit
