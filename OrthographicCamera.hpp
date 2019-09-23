#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H
#include "Camera.hpp"

class OrthographicCamera : public Camera
{
    public:
        OrthographicCamera(const Vec3f& center, const Vec3f& lookAt, const Vec3f& up, float size);
        Ray generateRay(const Vec2f& pixel) override;


    private:
    float _left, _right, _top, _bottom, _size;
    Vec3f _lookAt;
};

#endif