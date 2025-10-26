#include "include.h"

/*gtfo out of here you can't handle the memes on this page*/

using std::min;
using std::max;

Ray::Ray( Vector3 origin, Vector3 direction )
{
	m_origin = origin;
	m_direction = direction;

	m_directionInverse.x = ( 1.0f / m_direction.x );
	m_directionInverse.y = ( 1.0f / m_direction.y );
	m_directionInverse.z = ( 1.0f / m_direction.z );
}

bool Ray::IntersectWithOBB( Matrix3x4 modelMatrix, static Vector3 aabb_min, static Vector3 aabb_max, float& intersection_distance )
{
	Vector3 ray_trans, dir_trans;

	g_pMath->VectorITransform( m_origin, modelMatrix, ray_trans );
	g_pMath->VectorIRotate( m_direction, modelMatrix, dir_trans ); //only rotate direction vector! no translation!

	return IntersectWithAABB( ray_trans, dir_trans, aabb_min, aabb_max );
}

bool Ray::IntersectWithAABB( const Vector3& origin, const Vector3& dir, static Vector3& min, static Vector3& max )
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	if( dir.x >= 0 )
	{
		tmin = ( min.x - origin.x ) / dir.x;
		tmax = ( max.x - origin.x ) / dir.x;
	}
	else
	{
		tmin = ( max.x - origin.x ) / dir.x;
		tmax = ( min.x - origin.x ) / dir.x;
	}

	if( dir.y >= 0 )
	{
		tymin = ( min.y - origin.y ) / dir.y;
		tymax = ( max.y - origin.y ) / dir.y;
	}
	else
	{
		tymin = ( max.y - origin.y ) / dir.y;
		tymax = ( min.y - origin.y ) / dir.y;
	}

	if( tmin > tymax || tymin > tmax )
		return false;

	if( tymin > tmin )
		tmin = tymin;

	if( tymax < tmax )
		tmax = tymax;

	if( dir.z >= 0 )
	{
		tzmin = ( min.z - origin.z ) / dir.z;
		tzmax = ( max.z - origin.z ) / dir.z;
	}
	else
	{
		tzmin = ( max.z - origin.z ) / dir.z;
		tzmax = ( min.z - origin.z ) / dir.z;
	}

	if( tmin > tzmax || tzmin > tmax )
		return false;

	if( tmin < 0 || tmax < 0 )
		return false;

	return true;
}