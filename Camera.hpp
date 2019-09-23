#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.hpp"

class Camera
{
    public:
        Camera() = default;
        ~Camera() = default;
        virtual Ray generateRay(const Vec2f& pixel) = 0;

    // protected:
        Vec3f center, direction, up, horizontal;

    private:
};

#endif