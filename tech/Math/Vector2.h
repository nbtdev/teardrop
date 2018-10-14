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

#if !defined(VECTOR2_INCLUDED)
#define VECTOR2_INCLUDED

namespace Teardrop
{
	class Vector2
	{
	public:
		// public by intent
		float x, y;

		// default c'tor -- does not initialize anything, by intent
		Vector2() {}

		// copy c'tor - initialize data from provided params
		Vector2(const Vector2& other);

		// initialize data from provided params
		Vector2(float ax, float ay);

		// d'tor
		~Vector2();

		// copy operator
		Vector2& operator=(const Vector2& other);

		// mutating methods
		float normalize();

		// mutators sourced from two other vectors
		Vector2& add(const Vector2& a, const Vector2& b);
		Vector2& diff(const Vector2& a, const Vector2& b);

		// mutating operators sourced from one other vector
		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);

		float length() const;
		float length2() const; // returns squared
		float distance(const Vector2& other) const;
		float distance2(const Vector2& other) const; // returns squared

		static const Vector2& ZERO;
	};

	// operator+ for two vectors -- CREATES/RETURNS TEMPORARY BY VALUE!!!
	Vector2 operator+(const Vector2& a, const Vector2& b);
	// operator- for two vectors -- CREATES/RETURNS TEMPORARY BY VALUE!!!
	Vector2 operator-(const Vector2& a, const Vector2& b);
	// operator* (cross-product) for two vectors -- CREATES/RETURNS TEMPORARY BY VALUE!!!
	Vector2 operator*(const Vector2& a, const Vector2& b);
	// operator+ for two vectors
	void add(Vector2& result, const Vector2& l, const Vector2& r);
	// operator- for two vectors
	void diff(Vector2& result, const Vector2& l, const Vector2& r);
	// operator* (cross-product) for two vectors
	void mul(Vector2& result, const Vector2& l, const Vector2& r);
}

#endif // VECTOR2_INCLUDED
