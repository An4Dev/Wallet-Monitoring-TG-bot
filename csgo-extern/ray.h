#pragma once
#include "math.h"

class Ray
{

public:
	Ray( Vector3 origin, Vector3 direction );

	bool IntersectWithOBB( Matrix3x4 modelMatrix, static Vector3 aabb_min, static Vector3 aabb_max, float& intersection_distance );
	bool IntersectWithAABB( const Vector3& origin, const Vector3& dir, static Vector3& min, static Vector3& max );

private:
	Vector3 m_origin;
	Vector3 m_direction;
	Vector3 m_directionInverse;
};