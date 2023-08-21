#pragma once

#include <cmath>
#include <string>
#include "MeasureBrightness.h"
#include "SDLEssentials.h"
#include "Vector.h"

namespace _3DRenderer
{
	const Vector_3d CAMERA_POSITION{ 0, 0, 0 };

	const double VIEWPORT_WIDTH{ 1 };
	const double VIEWPORT_HEIGHT{ 1 };
	const Vector_3d VIEWPORT_POSITION{ -VIEWPORT_WIDTH / 2.0, 1, VIEWPORT_HEIGHT / 2.0 };

	const int SCREEN_WIDTH{ static_cast<int>( 48 ) };
	const int SCREEN_HEIGHT{ static_cast<int>( 48 ) };

	const double BACKGROUND_COLOR{ 0.0 };
}

struct Sphere
{
	Vector_3d position;
	double radius;
	double color;
};


struct LightPoint
{
	double intensity;
	Vector_3d position;
};

bool operator==(const LightPoint& a, const LightPoint& b);

struct LightDirectional
{
	double intensity;
	Vector_3d direction;
};


struct LightAmbient
{
	double intensity;
};

void renderScreen(std::vector<double>& screen, const std::vector<Sphere>& spheres, const std::vector<LightPoint>& point_lights = {}, const std::vector<LightDirectional>& directional_lights = {}, const std::vector<LightAmbient>& ambient_lights = {});
std::string screenToASCII(const std::vector<double>& screen = {}, const std::vector<BrightCharacter>& brightChars = {});