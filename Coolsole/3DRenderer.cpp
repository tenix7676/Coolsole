#include "3DRenderer.h"

using namespace _3DRenderer;


bool operator==(const LightPoint& a, const LightPoint& b)
{
	return a.intensity == b.intensity && a.position == b.position;
}

Vector_3d screenToViewport(Vector_2d pos_screen)
{
	Vector_3d pos_viewport;
	pos_viewport.x = pos_screen.x * VIEWPORT_WIDTH / static_cast<double>(SCREEN_WIDTH);
	pos_viewport.y = pos_screen.y * VIEWPORT_HEIGHT / static_cast<double>(SCREEN_HEIGHT);
	pos_viewport.z = VIEWPORT_POSITION.z;

	return pos_viewport;
}

//returns negative if no solution
double intersectRaySphere(Vector_3d start, Vector_3d direction, const Sphere& sphere)
{
	Vector_3d SphereToStart{ start - sphere.position };
	double a = dot(direction, direction);
	double b = 2 * dot(direction, SphereToStart);
	double c = dot(SphereToStart, SphereToStart) - sphere.radius * sphere.radius;

	double discriminant = b * b - 4 * a * c;
	if(discriminant < 0)
	{
		return -1;
	}

	double t1 = (-b + std::sqrt(discriminant)) / static_cast<double>(2 * a);
	double t2 = (-b - std::sqrt(discriminant)) / static_cast<double>(2 * a);

	return std::min(t1, t2);
}

double computeLightingPoint(const Vector_3d point, Vector_3d normal, const std::vector<LightPoint>& lights)
{
	double intensity{ 0.0 };
	Vector_3d l{};
	for(auto& light : lights)
	{
		l = light.position - point;

		double n_dot_l = dot(normal, l);
		if(n_dot_l > 0)
		{
			intensity += light.intensity * n_dot_l / (length(normal) * length(l));
		}
	}

	return intensity;
}

double computeLightingDirectional(Vector_3d point, Vector_3d normal, const std::vector<LightDirectional>& lights)
{
	double intensity{ 0.0 };
	Vector_3d l{};
	for(auto& light : lights)
	{
		l = light.direction;

		double n_dot_l = dot(normal, l);
		if(n_dot_l > 0)
		{
			intensity += light.intensity * n_dot_l / (length(normal) * length(l));
		}
	}

	return intensity;
}

double computeLightingAmbient(const std::vector<LightAmbient>& lights)
{
	double intensity{ 0.0 };
	for(auto& light : lights)
	{
		intensity += light.intensity;
	}

	return intensity;
}

double traceRay(Vector_3d start, Vector_3d direction, double t_min, double t_max, const std::vector<Sphere>& spheres, const std::vector<LightPoint>& point_lights, const std::vector<LightDirectional>& directional_lights, const std::vector<LightAmbient>& ambient_lights)
{
	double color{ BACKGROUND_COLOR };

	for(const auto& sphere : spheres)
	{
		double t = intersectRaySphere(start, direction, sphere);
		Vector_3d point{ start + t * direction };
		Vector_3d normal{ point - sphere.position };
		normal = normal / length(normal);
		if(t < t_min)
		{
			;//color += BACKGROUND_COLOR;
		}
		else if(t > t_max)
		{
			;
		}
		else
		{
			color = sphere.color * (computeLightingPoint(point, normal, point_lights) + computeLightingDirectional(point, normal, directional_lights) + computeLightingAmbient(ambient_lights));
		}
	}
	
	return color;
}


void renderScreen(std::vector<double>& screen, const std::vector<Sphere>& spheres, const std::vector<LightPoint>& point_lights, const std::vector<LightDirectional>& directional_lights, const std::vector<LightAmbient>& ambient_lights)
{
	screen.clear();

	for(int y{ -SCREEN_HEIGHT / 2 }; y < SCREEN_HEIGHT / 2; ++y)
	{
		for(int x{ -SCREEN_WIDTH / 2 }; x < SCREEN_WIDTH / 2; ++x)
		{
			Vector_2d pos_screen{ static_cast<double>(x), static_cast<double>(y) };
			Vector_3d pos_viewport{ screenToViewport(pos_screen) };
			double color = traceRay(CAMERA_POSITION, pos_viewport - CAMERA_POSITION, 1, INFINITY, spheres, point_lights, directional_lights, ambient_lights);
			screen.push_back(color);
		}
	}
}

std::string screenToASCII(const std::vector<double>& screen, const std::vector<BrightCharacter>& brightChars)
{
	std::string str_screen{ "" };

	double last_pixel{ 0.0 };

	int y{}, x{};
	for(y = 0; y < _3DRenderer::SCREEN_HEIGHT; ++y)
	{
		for(x = 0; x < _3DRenderer::SCREEN_WIDTH; ++x)
		{
			double current_pixel{ screen[y * _3DRenderer::SCREEN_WIDTH + x] };
			str_screen += getBrightestUpTo(brightChars, current_pixel).character;
			str_screen += getBrightestUpTo(brightChars, current_pixel).character;
		}
		str_screen += '\n';
	}

	return str_screen;
}