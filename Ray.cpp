#include "Ray.hpp"

Ray::Ray(const Vec3f& origin, const Vec3f& dir) : _origin(origin), _dir(dir)
{
}

Ray::Ray(const Ray& r)
{
    this->_origin = r._origin;
    this->_dir  = r._dir;
}

const Vec3f& Ray::getOrigin() const
{
    return this->_origin;
}

const Vec3f& Ray::getDirection() const
{
    return this->_dir;
}

Vec3f Ray::pointAt(float t) const
{
    return this->_origin + this->_dir * t;
}

std::ostream& operator <<(std::ostream& os, const Ray& r)
{
    os << "[ " << r.getOrigin() << ", " << r.getDirection() << " ]";
    return os;  
}