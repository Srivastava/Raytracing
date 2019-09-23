#include "OrthographicCamera.hpp"

OrthographicCamera::OrthographicCamera(const Vec3f& center, const Vec3f& lookAt, const Vec3f& up, float size)
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

    this->_size = size;
    this->horizontal = cross(this->direction, up).normalize();
    this->up = cross(this->horizontal, this->direction);
    
    float hsize = size / 2.0;
    _left = -hsize;
    _right = hsize;
    _top = hsize;
    _bottom = -hsize;
}

Ray OrthographicCamera::generateRay(const Vec2f& pixel)
{
    float u = _bottom+(_top-_bottom)*pixel[1];
    float v = _left+(_right-_left)*pixel[0];
    Vec3f origin = this->center + (this->up * u) + (this->horizontal * v);

    return Ray(origin, this->direction);
}