/******************************************************************************
Copyright (c) 2015 Teardrop Games

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

#if !defined(TDQUERY_INCLUDED)
#define TDQUERY_INCLUDED

namespace Teardrop
{
	class AABB;
	class Plane;
	class Vector4;
	class Ray;
	class Sphere;
	class Triangle;

	class Query
	{
	public:
		/* check to see if AABB intersects plane; if it does not, 
			its position relative to the plane is returned in "inside"
			(true if the AABB is in the plane's positive halfspace, false 
			if the AABB is in the plane's negative halfspace)
		*/
		static bool AABB_Plane(const AABB& aabb, const Plane& plane, /*out*/bool& inside);

		/* check to see if AABB 'inner' fits entirely inside AABB 'outer'; returns true
			if the inner AABB fits entirely inside the outer one (within tolerances, and
			coplanar faces will return 'true' as well)
		 */
		static bool contains(const AABB& outer, const AABB& inner);

		/* check to see if the ray intersects a triangle; returns interval t along the ray
			if intersects
		 */
		static bool Ray_Triangle(const Ray& ray, const Triangle& tri, /*out*/ float& t);

		/* check to see if the ray intersects a triangle
		 */
		static bool Ray_AABB(const Ray& ray, const AABB& aabb);

		/* check to see if the ray intersects a sphere; on success, t contains the parametric
			of the intersection point (along ray), and q contains the actual point of 
			intersection.
		 */
		static bool Ray_Sphere(const Ray& ray, const Sphere& sphere, float& t, Vector4& q);

		/* find the closest point on a Ray to the 3D point provided in 'p'; if successful, returns
			the parametric in t and the point of intersection in q
		 */
		static bool ClosestPoint_Ray_Point(const Ray& ray, const Vector4& p, float& t, Vector4& q);

		/* find the closest points on two lines (rays); if successful, returns the closest points 
			on r1 and r2, in q1 and q2, resp, and the parameters for those points in s and t, resp.
		 */
		static bool ClosestPoints_Ray_Ray(const Ray& r1, const Ray& r2, float& s, float& t, Vector4& q1, Vector4& q2);
	};
}

#endif // TDQUERY_INCLUDED
