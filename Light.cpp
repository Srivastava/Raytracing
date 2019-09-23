#include "Light.hpp"

Light::Light(const double intensity, const Vec3f& color) : _intensity(intensity), _color(color)
{
}

Vec3f Light::color() const
{
    return this->_color;
}

double Light::intensity() const
{
    return this->_intensity;
}

Point::Point(const Vec3f& position, const double intensity, const Vec3f& color) : Light(intensity, color), _position(position)
{
}

double Point::distance(const Vec3f& point) const
{
    return (point - this->_position).abs();
}

Vec3f Point::direction(const Vec3f& point) const
{
    return (point - this->_position);
}

Vec3f Point::color(const Vec3f& point) const
{
    return color() * intensity() * (1.0/Point::distance(point)) * (1.0f / (4 * M_PI));
}

Area::Area(const Vec3f& A, const Vec3f& B, const Vec3f& C, const Vec3f& D, const Vec3f& direction, const double intensity, const Vec3f& color) : Light(intensity, color)
{
    _position = (A + C) * 0.5;
    _A  = A;
    _B = B;
    _C = C;
    _D = D;
    _direction = direction;
    _area = (A - C).abs2();
}

double Area::distance(const Vec3f& point) const
{
    return (point - this->_position).abs();
}

Vec3f Area::direction(const Vec3f& point) const
{
    // Randomizing a point on the plane so we get multiple
    std::function<Vec3f(const Vec3f&, const Vec3f&)> cross =
    [](const Vec3f& a, const Vec3f& b)
    {
        return Vec3f(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
    };
    
    Vec3f n = cross(_B - _A , _C - _A).normalize();
    Vec3f v1 = (_B - _A).normalize();
    Vec3f v2 = cross(n, v1).normalize();

    double horizontalDist = (_B - _A).abs();
    double verticalDist = (_B - _C).abs();

    Vec3f lowerBottom = this->_position - v1 * (horizontalDist / 2.0) - v2 * (verticalDist / 2.0);

    double random_x = ((double) rand() / (RAND_MAX)), random_y = ((double) rand() / (RAND_MAX));
    double rand_x = random_x * horizontalDist, rand_y = random_y * verticalDist;

    Vec3f newpoint = lowerBottom + v1 * rand_x + v2 * rand_y;
    return (point - newpoint);
}

Vec3f Area::color(const Vec3f& point) const
{
    Vec3f direction_point = (point - this->_position).normalize();
    double angle = std::max(0.0d, direction_point.dot(this->_direction));

    // If total intensity if sent in hemisphere of radius 1 which mean it is 2*pi steradians, so calculating the angle the point 
    // is in and taking the (cos(theta) / 2*PI)

    return color() * intensity() * (1.0/Area::distance(point)) * (angle / (2 * M_PI)) * (1.0f / this->_area) ;
}