#include "Vector.h"

/*	VECTOR_2D FUNCTIONS:	*/
Vector_2d operator-(const Vector_2d& v)
{
	return Vector_2d{ -v.x, -v.y };
}
Vector_2d operator+(const Vector_2d& v1, const Vector_2d& v2)
{
	return Vector_2d{ v1.x + v2.x, v1.y + v2.y };
}
Vector_2d operator-(const Vector_2d& v1, const Vector_2d& v2)
{
	return Vector_2d{ v1.x - v2.x, v1.y - v2.y };
}
Vector_2d operator*(double d, const Vector_2d& v)
{
	return Vector_2d{ v.x * d, v.y * d };
}
Vector_2d operator*(const Vector_2d& v, double d)
{
	return d * v;
}
Vector_2d operator/(const Vector_2d& v, double d)
{
	return Vector_2d{ v.x / d, v.y / d };
}
Vector_2d operator+=(Vector_2d& v1, const Vector_2d& v2)
{
	v1 = v1 + v2;
	return v1;
}
Vector_2d operator-=(Vector_2d& v1, const Vector_2d& v2)
{
	v1 = v1 - v2;
	return v1;
}
Vector_2d operator*=(Vector_2d& v, const double d)
{
	v = v * d;
	return v;
}
Vector_2d operator/=(Vector_2d& v, const double d)
{
	v = v / d;
	return v;
}

bool operator==(const Vector_2d& v1, const Vector_2d& v2)
{
	return (v1.x == v2.x) && (v1.y == v2.y);
}

bool operator!=(const Vector_2d& v1, const Vector_2d& v2)
{
	return !(v1 == v2);
}


double distanceSqr(double x1, double y1, double x2, double y2)
{
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}
double distanceSqr(Vector_2d v1, Vector_2d v2)
{
	return distanceSqr(v1.x, v1.y, v2.x, v2.y);
}
double lengthSqr(double x, double y)
{
	return distanceSqr(x, y);
}
double lengthSqr(Vector_2d v)
{
	return distanceSqr(v);
}
double dot(double x1, double y1, double x2, double y2)
{
	return x1 * x2 + y1 * y2;
}
double dot(Vector_2d v1, Vector_2d v2)
{
	return dot(v1.x, v1.y, v2.x, v2.y);
}
double cross(double x1, double y1, double x2, double y2)
{
	return x1 * y2 - x2 * y1;
}
double cross(Vector_2d v1, Vector_2d v2)
{
	return cross(v1.x, v1.y, v2.x, v2.y);
}
double length(Vector_2d v)
{
	return std::sqrt(lengthSqr(v));
}


//return a 90 deegres counterclockwise vector
Vector_2d getPerpendicularVector(Vector_2d vector)
{
	return Vector_2d(-vector.y, vector.x);
}
double rsqrt(double number)
{
	double y = number;
	double x2 = y * 0.5;
	std::int64_t i = *(std::int64_t*)&y;
	// The magic number is for doubles is from https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
	i = 0x5fe6eb50c7b537a9 - (i >> 1);
	y = *(double*)&i;
	y = y * (1.5 - (x2 * y * y));   // 1st iteration
	//      y  = y * ( 1.5 - ( x2 * y * y ) );   // 2nd iteration, this can be removed
	return y;
}
Vector_2d normalizeVector(Vector_2d& v)
{
	return v * rsqrt(lengthSqr(v));
}
Vector_2d projectVector(Vector_2d v1, Vector_2d v2)
{
	v2 = normalizeVector(v2);
	return dot(v1, v2) * v2;
}












/*	VECTOR_3D FUNCTIONS:	*/
Vector_3d operator-(const Vector_3d& v)
{
	return Vector_3d{ -v.x, -v.y, -v.z };
}
Vector_3d operator+(const Vector_3d& v1, const Vector_3d& v2)
{
	return Vector_3d{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}
Vector_3d operator-(const Vector_3d& v1, const Vector_3d& v2)
{
	return Vector_3d{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
Vector_3d operator*(const Vector_3d& v, const double d)
{
	return Vector_3d{ v.x * d, v.y * d, v.z * d };
}
Vector_3d operator*(const double d, const Vector_3d& v)
{
	return v * d;
}
Vector_3d operator/(const Vector_3d& v, const double d)
{
	return Vector_3d{ v.x / d, v.y / d, v.z / d };
}
Vector_3d operator+=(Vector_3d& v1, const Vector_3d& v2)
{
	v1 = v1 + v2;
	return v1;
}
Vector_3d operator-=(Vector_3d& v1, const Vector_3d& v2)
{
	v1 = v1 - v2;
	return v1;
}
Vector_3d operator*=(Vector_3d& v, const double d)
{
	v = v * d;
	return v;
}
Vector_3d operator/=(Vector_3d& v, const double d)
{
	v = v / d;
	return v;
}

bool operator==(const Vector_3d& v1, const Vector_3d& v2)
{
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

bool operator!=(const Vector_3d& v1, const Vector_3d& v2)
{
	return !(v1 == v2);
}

double lengthSqr(const Vector_3d& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
double length(const Vector_3d& v)
{
	return std::sqrt(lengthSqr(v));
}
double dot(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return x1 * x2 + y1 * y2 + z1 * z2;
}
double dot(const Vector_3d& v1, const Vector_3d& v2)
{
	return dot(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
}
Vector_3d cross(const Vector_3d& v1, const Vector_3d& v2)
{
	return Vector_3d{ v1.y * v2.z - v1.z * v2.y, v1.x * v2.z - v1.z * v2.x, v1.x * v2.y - v1.y * v2.x };
}
