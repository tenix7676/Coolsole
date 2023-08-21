#pragma once
#include <cmath>
#include <cstdint>

struct Vector_2d
{
	double x, y;
};
struct Vector_3d
{
	double x, y, z;
};

/*	VECTOR_2D FUNCTIONS:	*/
Vector_2d operator-(const Vector_2d& v);
Vector_2d operator+(const Vector_2d& v1, const Vector_2d& v2);
Vector_2d operator-(const Vector_2d& v1, const Vector_2d& v2);
Vector_2d operator*(double d, const Vector_2d& v);
Vector_2d operator*(const Vector_2d& v, double d);
Vector_2d operator/(const Vector_2d& v, double d);
Vector_2d operator+=(Vector_2d& v1, const Vector_2d& v2);
Vector_2d operator-=(Vector_2d& v1, const Vector_2d& v2);
Vector_2d operator*=(Vector_2d& v, const double d);
Vector_2d operator/=(Vector_2d& v, const double d);
bool operator==(const Vector_2d& v1, const Vector_2d& v2);
bool operator!=(const Vector_2d& v1, const Vector_2d& v2);

double distanceSqr(double x1, double y1, double x2 = 0.0, double y2 = 0.0);
double distanceSqr(Vector_2d v1, Vector_2d v2 = Vector_2d(0.0, 0.0));
double lengthSqr(double x, double y);
double lengthSqr(Vector_2d v);
double dot(double x1, double y1, double x2, double y2);
double dot(Vector_2d v1, Vector_2d v2);
double cross(double x1, double y1, double x2, double y2);
double cross(Vector_2d v1, Vector_2d v2);
double length(Vector_2d v);


//return a 90 deegres counterclockwise vector
Vector_2d getPerpendicularVector(Vector_2d vector);
double rsqrt(double number);
Vector_2d normalizeVector(Vector_2d& v);
Vector_2d projectVector(Vector_2d v1, Vector_2d v2);










/*	VECTOR_3D FUNCTIONS:	*/
Vector_3d operator-(const Vector_3d& v);
Vector_3d operator+(const Vector_3d& v1, const Vector_3d& v2);
Vector_3d operator-(const Vector_3d& v1, const Vector_3d& v2);
Vector_3d operator*(const Vector_3d& v, const double d);
Vector_3d operator*(const double d, const Vector_3d& v);
Vector_3d operator/(const Vector_3d& v, const double d);
Vector_3d operator+=(Vector_3d& v1, const Vector_3d& v2);
Vector_3d operator-=(Vector_3d& v1, const Vector_3d& v2);
Vector_3d operator*=(Vector_3d& v, const double d);
Vector_3d operator/=(Vector_3d& v, const double d);
bool operator==(const Vector_3d& v1, const Vector_3d& v2);
bool operator!=(const Vector_3d& v1, const Vector_3d& v2);

double lengthSqr(const Vector_3d& v);
double length(const Vector_3d& v);
double dot(double x1, double y1, double z1, double x2, double y2, double z2);
double dot(const Vector_3d& v1, const Vector_3d& v2);
Vector_3d cross(const Vector_3d& v1, const Vector_3d& v2);
