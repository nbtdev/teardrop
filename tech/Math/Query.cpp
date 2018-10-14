/******************************************************************************
Copyright (c) 2018 Teardrop Games

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************/

/*
Note: many of the algorithms in this file are based on the contents of the 
Morgan-Kaufmann book, "Real-Time Collision Detection" (Ericson)
*/

#include "Query.h"
#include "MathUtil.h"
#include "Vector4.h"
#include "AABB.h"
#include "Plane.h"
#include "Ray.h"
#include "Triangle.h"
#include "Sphere.h"

using namespace Teardrop;

//---------------------------------------------------------------------------
bool Query::AABB_Plane(const AABB& aabb, const Plane& plane, bool& inside)
{
	// SAT test -- project the box extents (r) onto the axis separating the
	// box and the plane. If this is less than the abs distance from the
	// box center to the plane (s) then there is no intersection, and we 
	// need to determine which halfspace the box is in (inside)
	const Vector4& c = aabb.center;
	const Vector4& e = aabb.extents;
	const Vector4& n = plane.n;

	float r = 
		e.x*MathUtil::abs(n.x) + 
		e.y*MathUtil::abs(n.y) + 
		e.z*MathUtil::abs(n.z);

	float s = dot(n, c) + plane.d;

	inside = (r <= s);
	return (MathUtil::abs(s) <= r);
}
//---------------------------------------------------------------------------
bool Query::contains(const AABB& outer, const AABB& inner)
{
	float x = outer.extents.x;
	float y = outer.extents.y;
	float z = outer.extents.z;

	// put inner in outer's space so that we can deal only with outer's extents
	Vector4 relOrigin(inner.center - outer.center);
	relOrigin.makeAbsolute();
	Vector4 relExtents(relOrigin + inner.extents);

	return ((relExtents.x <= x) && (relExtents.y <= y) && (relExtents.z <= z));
}
//---------------------------------------------------------------------------
// algorithm from Real Time Collision Detection (Ericson) pp. 191-192
//bool Query::Ray_Triangle(const Ray& ray, const Triangle& tri)
//{
//	// make a dummy line segment from the ray
//	Vector4 p(ray.end);
//	Vector4 q(ray.end + ray.dir * 10000); // assumes that the dir is normalized
//	Vector4 ab(tri.p[1] - tri.p[0]);
//	Vector4 ac(tri.p[2] - tri.p[0]);
//	Vector4 qp(q-p);
//
//	Vector4 n;
//	cross(n, ab, ac);
//
//	float d = dot(qp, n);
//	if (d < 0)
//		return false; // ray points away from tri
//
//	Vector4 ap(p - tri.p[0]);
//	float t = dot(ap, n);
//	if (t < 0)
//		return false; // single-sided test: ray end is behind tri
//
//	Vector4 e;
//	cross(e, qp, ap);
//	float v = dot(ac, e);
//	if (v < 0 || v > d)
//		return false; // intersection point lies outside one of the tri edges
//
//	float w = -dot(ab, e);
//	if (w < 0 || (v+w) > d)
//		return false; // intersection point lies outside
//
//	return true; // intersection must lie inside the tri bounds
//
//	// note that the math for returning the intersection point has been
//	// left out -- for purposes of telling whether a ray intersects a triangle,
//	// we need not go further than we have above
//}
//---------------------------------------------------------------------------
// algorithm borrowed from OgreMath.cpp -- find intersection point, then see
// if that point is inside the triangle
bool Query::Ray_Triangle(const Ray& ray, const Triangle& tri, float& t)
{
	// first find intersection point in plane defined by tri; first make 
	// normal from tri
	Vector4 n;
	cross(n, tri.p[1]-tri.p[0], tri.p[2]-tri.p[0]);

	// then see if we are parallel or not (this is two-sided intersection)
	float denom = dot(n, ray.dir);
	if (denom < MathUtil::EPSILON && denom > -MathUtil::EPSILON)
		return false; // parallel

	t = dot(n, tri.p[0] - ray.end) / denom;
	if (t < 0)
		return false; // tri is behind the ray (ray points away)

	size_t i0=1, i1=2;
	float nx = MathUtil::abs(n.x);
	float ny = MathUtil::abs(n.y);
	float nz = MathUtil::abs(n.z);

	if (ny > nz)
	{
		if (ny > nx)
			i0 = 0;
	}
	else
	{
		if (nz > nx)
			i1 = 0;
	}

	const float* a = &tri.p[0].x;
	const float* b = &tri.p[1].x;
	const float* c = &tri.p[2].x;
	const float* o = &ray.end.x;
	const float* d = &ray.dir.x;

	float u1 = b[i0] - a[i0];
	float v1 = b[i1] - a[i1];
	float u2 = c[i0] - a[i0];
	float v2 = c[i1] - a[i1];
	float u0 = t * d[i0] + o[i0] - a[i0];
	float v0 = t * d[i1] + o[i1] - a[i1];

	float alpha = u0 * v2 - u2 * v0;
	float beta =  u1 * v0 - u0 * v1;
	float area =  u1 * v2 - u2 * v1;
	float tol = - MathUtil::EPSILON * area;

	if (area > 0)
	{
		if (alpha < tol || beta < tol || alpha+beta > area-tol)
			return false;
	}
	else
	{
		if (alpha > tol || beta > tol || alpha+beta < area-tol)
			return false;
	}

	return true;
}
//---------------------------------------------------------------------------
// algorithm from the DirectX Pick sample
//bool Query::Ray_Triangle(const Ray& ray, const Triangle& tri, float& t)
//{
//	Vector4 edge1(tri.p[1] - tri.p[0]);
//	Vector4 edge2(tri.p[2] - tri.p[0]);
//
//	Vector4 pvec;
//	cross(pvec, ray.dir, edge2);
//
//	float det = dot(edge1, pvec);
//	if (MathUtil::abs(det) < MathUtil::EPSILON)
//		return false; // in plane of triangle
//
//	Vector4 tvec;
//	if (det > 0)
//	{
//		tvec = ray.end - tri.p[0];
//	}
//	else
//	{
//		tvec = tri.p[0] - ray.end;
//		det = -det;
//	}
//
//	float u = dot(tvec, pvec);
//	if (u < 0 || u > det)
//		return false;
//
//	Vector4 qvec;
//	cross(qvec, tvec, edge1);
//
//	float v = dot(ray.dir, qvec);
//	if (v < 0 || (u+v) > det)
//		return false;
//
//	t = dot(edge2, qvec);
//	t /= det;
//	return true;
//}
//---------------------------------------------------------------------------
// algorithm from Real Time Collision Detection (Ericson) pp. 180-181
bool Query::Ray_AABB(const Ray& ray, const AABB& aabb)
{
	Vector4 p0(ray.end);
	Vector4 p1(ray.dir * 100000);

	Vector4 m = (p0 + p1) * 0.5f;
	Vector4 d = p1 - m;
	m -= aabb.center;

	float adx = MathUtil::abs(d.x);
	if (MathUtil::abs(m.x) > aabb.extents.x + adx)
		return false;
	float ady = MathUtil::abs(d.y);
	if (MathUtil::abs(m.y) > aabb.extents.y + ady)
		return false;
	float adz = MathUtil::abs(d.z);
	if (MathUtil::abs(m.z) > aabb.extents.z + adz)
		return false;

	adx += MathUtil::EPSILON;
	ady += MathUtil::EPSILON;
	adz += MathUtil::EPSILON;

	if (MathUtil::abs(m.y * d.z - m.z * d.y) > (aabb.extents.y * adz + aabb.extents.z * ady))
		return false;
	if (MathUtil::abs(m.z * d.x - m.x * d.z) > (aabb.extents.x * adz + aabb.extents.z * adx))
		return false;
	if (MathUtil::abs(m.x * d.y - m.y * d.x) > (aabb.extents.x * ady + aabb.extents.y * adx))
		return false;

	return true;
}
//---------------------------------------------------------------------------
// algorithm from Real Time Collision Detection (Ericson) pp. 178
bool Query::Ray_Sphere(const Ray& ray, const Sphere& s, float& t, Vector4& q)
{
	Vector4 m = ray.end - s.m_center;
	float b = dot(m, ray.dir);
	float c = dot(m, m) - s.m_radius * s.m_radius;

	// exit if the origin is outside the sphere and pointing away
	if (c > 0 && b > 0)
		return false;

	float disc = b * b - c;

	// since this is a quadratic equaction, a negative here means ray missed
	if (disc < 0)
		return false;

	// otherwise, use the nearest (smaller) of the two parametrics
	float sq = MathUtil::sqrt(disc);
	t = -b - sq;

	// and if it's less than zero, then the ray started inside the sphere; use
	// the larger of the two roots
	t = b - sq;

	// and identify the point of intersection
	q = ray.end + ray.dir * t;

	return true;
}
//---------------------------------------------------------------------------
// algorithm from Real Time Collision Detection (Ericson) pp. 128
bool Query::ClosestPoint_Ray_Point(const Ray& ray, const Vector4& p, float& t, Vector4& q)
{
	t = dot(p - ray.end, ray.dir) / ray.dir.getLength2();
	q = ray.end + t * ray.dir;
	return true;
}
//---------------------------------------------------------------------------
// algorithm from Real Time Collision Detection (Ericson) pp. 149-150
bool Query::ClosestPoints_Ray_Ray(
	const Ray& r1, const Ray& r2, 
	float& s, float& t, 
	Vector4& q1, Vector4& q2)
{
	Vector4 r(r1.end - r2.end);
	float a = 1; // squared length of r1, since dir is normalized, = 1
	float e = 1; // squared length of r2, since dir is normalized, = 1
	float f = dot(r2.dir, r);

	if (r.getLength2() < MathUtil::EPSILON)
	{
		s = t = 0;
		q1 = r1.end;
		q2 = r2.end;
		return true;
	}

	float c = dot(r1.dir, r);
	float b = dot(r1.dir, r2.dir);
	float denom = a*e - b*b;
	
	if (denom > MathUtil::EPSILON)
		s = (b*f - c*e) / denom;
	else
		s = 0;

	t = (b*s + f) / e;

	q1 = r1.end + r1.dir * s;
	q2 = r2.end + r2.dir * t;

	return true;
}
