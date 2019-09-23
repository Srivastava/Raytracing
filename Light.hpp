#ifndef LIGHT_H
#define LIGHT_H

#include "Geometry.hpp"
#include "Surface.hpp"

class Light
{
    public:
        Light() = default;
        Light(const double intensity, const Vec3f& color);

        virtual double distance(const Vec3f& point) const = 0;
        virtual Vec3f direction(const Vec3f& point) const = 0;
        virtual Vec3f color(const Vec3f& point) const = 0;
        
        Vec3f color() const;
        double intensity() const; 
    private:
        double _intensity;
        Vec3f _color;
};

class Point : public Light
{
    public:
        Point() = delete;
        Point(const Vec3f& position, const double intensity, const Vec3f& color);

        double distance(const Vec3f& point) const;
        Vec3f direction(const Vec3f& point) const;
        Vec3f color(const Vec3f& point) const;

        using Light::color;
        using Light::intensity;
        private:
            Vec3f _position;
};


class Area : public Light
{
    public:
        Area() = delete;
        Area(const Vec3f& A, const Vec3f& B, const Vec3f& C, const Vec3f& D, const Vec3f& direction, const double intensity, const Vec3f& color);

        double distance(const Vec3f& point) const;
        Vec3f direction(const Vec3f& point) const;
        Vec3f color(const Vec3f& point) const;

        using Light::color;
        using Light::intensity;
        private:
            Vec3f _position, _A,  _B, _C, _D, _direction;
            double _area;
};
#endif
