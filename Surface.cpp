#include "Surface.hpp"

Surface::Surface()
{
    _color = {0.0, 0.0, 0.0};
    _phong = {0.0, 0.0, 0.0};

    _diffuse = {0.0, 0.0, 0.0};
    _specular = {0.0, 0.0, 0.0};
    _ri = 0.0;
    _albedo = 0.0;

    _isReflect = false;
    _isRefract = false;
    _isLight = false;
}

Surface::Surface(const Vec3f& color) : _color(color), _phong(0.0, 0.0, 0.0), _ri(0.0)
{
}

const Vec3f& Surface::color() const
{
    return this->_color;
}

const Vec3f& Surface::phong() const
{
    return this->_phong;
}

const Vec3f& Surface::diffuse() const
{
    return this->_diffuse;
}

const Vec3f& Surface::specular() const
{
    return this->_specular;
}

double Surface::refractiveIndex() const
{
    return this->_ri;
}

bool Surface::reflect() const
{
    return this->_isReflect;
}

bool Surface::refract() const
{
    return this->_isRefract;
}

bool Surface::isLight() const
{
    return this->_isLight;
}

void Surface::setPhong(const Vec3f& phong)
{
    this->_phong = phong;
}
void Surface::setColor(const Vec3f& color)
{
    this->_color = color;
}

void Surface::setDiffuse(const Vec3f& diffuse)
{
    this->_diffuse = diffuse;
}
void Surface::setSpecular(const Vec3f& specular)
{
    this->_specular = specular;
}
void Surface::setRI(const double ri)
{
    this->_ri = ri;
}

void Surface::setAlbedo(const double albedo)
{
    this->_albedo = albedo;
}

void Surface::setReflect(const bool reflect)
{
    this->_isReflect = reflect;
}
void Surface::setRefract(const bool refract)
{
    this->_isRefract = refract;
}

void Surface::setLight(const bool light)
{
    this->_isLight = light;
}

Sphere::Sphere(const Vec3f& center, double radius) : _center(center), _radius(radius)
{
}

Sphere::Sphere(const Vec3f& center, double radius, const Vec3f& color) : Surface(color), _center(center), _radius(radius)
{
}


bool Sphere::hit(const Vec3f& origin, const Vec3f& inputRay, Vec3f& hitPoint)
{
    Vec3f L = this->_center - origin;
    double tca = L.dot(inputRay);

    if(tca < 0)
        return false;
    
    double d2 = L.abs2() - tca*tca;
    if(d2 > this->_radius * this->_radius)
        return false;
    
    double thc = sqrtf(this->_radius * this->_radius - d2);
    
    double t0 = tca - thc;
    double t1 = tca + thc;

    if(t0 < 0 and t1 < 0)
        return false;
    if(t0 < 0)
        t0 = t1;
    hitPoint = origin + inputRay * t0;
    return true;
}

Vec3f Sphere::getNormal(const Vec3f& point)
{
    return (point - this->_center);
}


Triangle::Triangle(const Vec3f& a, const Vec3f& b, const Vec3f& c) : _a(a), _b(b), _c(c)
{

}

bool Triangle::hit(const Vec3f& origin, const Vec3f& inputRay, Vec3f& hitPoint)
{
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

    std::function<Vec3f(const Vec3f&, const Vec3f&)> cross =
    [](const Vec3f& a, const Vec3f& b)
    {
        return Vec3f(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
    };

    const float EPSILON = 0.0000001;
    Vec3f vertex0 = _a;
    Vec3f vertex1 = _b;  
    Vec3f vertex2 = _c;

    Vec3f edge1, edge2, h, s, q;
    float a,f,u,v;

    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    
    h = cross(inputRay, edge2);
    a = edge1.dot(h);
    
    if (a > -EPSILON and a < EPSILON)
        return false;

    f = 1.0/a;
    s = origin - vertex0;
    u = f * s.dot(h);
    
    if (u < 0.0 || u > 1.0)
        return false;

    q = cross(s, edge1);
    v = f * inputRay.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;

    float t = f * edge2.dot(q);
    if (t > EPSILON)
    {
        hitPoint = origin + inputRay * t;
        return true;
    }
    
    return false;
}

Vec3f Triangle::getNormal(const Vec3f& point)
{
    Vec3f U = _b - _a, V = _c - _a;

    std::function<Vec3f(const Vec3f&, const Vec3f&)> cross =
    [](const Vec3f& a, const Vec3f& b)
    {
        return Vec3f(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
    };
    return cross(U, V).normalize();
}