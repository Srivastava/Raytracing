#include "Perspective.hpp"

Perspective::Perspective(const Vec3f& center, const Vec3f& lookAt, const Vec3f& up, const int width, const int height, const double angle)
{
    this->center = center;
    this->direction = (lookAt - center);
    this->direction.normalize();
    this->_lookAt = lookAt;

    std::function<Vec3f(const Vec3f&, const Vec3f&)> cross =
    [](const Vec3f& a, const Vec3f& b)
    {
        return Vec3f(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
    };

    float aspectRatio = float(height) / float(width);
    float theta = angle * (M_PI) / 180.f;
	float halfHeight = tanf(theta / 2.f);
	float halfWidth = halfHeight * aspectRatio;

    _w = (lookAt - center).normalize();
	_u = cross(_w, up).normalize();
	_v = cross(_u, _w).normalize();

	_lowerLeftCorner = center - (_u * halfWidth) - (_v * halfHeight) + _w;
	_horizontal = (_u * halfWidth * 2.0f);
	_vertical = (_v * 2.0f * halfHeight);
}

Ray Perspective::generateRay(const Vec2f& pixel)
{
    Vec3f origin = _lowerLeftCorner + _horizontal * pixel[0] + _vertical * pixel[1];
    return Ray(this->center, (origin - this->center).normalize());
}