#ifndef RAY_H
#define RAY_H

#include "Geometry.hpp"

class Ray
{
    public:
        Ray(const Vec3f& origin, const Vec3f& dir);
        Ray(const Ray& r);

        const Vec3f& getOrigin() const;
        const Vec3f& getDirection() const;
        Vec3f pointAt(float t) const;

        friend std::ostream& operator <<(std::ostream& os, const Ray& r);

    private:
        Vec3f _origin;
        Vec3f _dir;
};

#endif