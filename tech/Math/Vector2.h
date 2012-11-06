/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

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
