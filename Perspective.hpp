#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "Camera.hpp"

class Perspective : public Camera
{
    public:
        Perspective(const Vec3f& center, const Vec3f& lookAt, const Vec3f& up, const int width, const int height, const double angle);
        Ray generateRay(const Vec2f& pixel) override;

    private:
		Vec3f _lowerLeftCorner;
		Vec3f _horizontal;
		Vec3f _vertical;
		Vec3f _lookAt;
		
		Vec3f _u;
		Vec3f _v;
		Vec3f _w;
	
};

#endif