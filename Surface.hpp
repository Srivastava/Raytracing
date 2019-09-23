#ifndef SURFACE_H
#define SURFACE_H

#include "Geometry.hpp"

class Surface
{
    public:
        Surface();
        Surface(const Vec3f& color);

        const Vec3f& color() const;
        const Vec3f& phong() const;

        const Vec3f& diffuse() const;
        const Vec3f& specular() const;
        double refractiveIndex() const;
        double albedo() const;
        
        bool reflect() const;
        bool refract() const;
        bool isLight() const;

        void setPhong(const Vec3f& phong);
        void setColor(const Vec3f& color);
        
        void setDiffuse(const Vec3f& diffuse);
        void setSpecular(const Vec3f& specular);
        void setRI(const double ri);
        void setAlbedo(const double albedo);
       
        void setReflect(const bool reflect);
        void setRefract(const bool refract);

        void setLight(const bool light);

        virtual bool hit(const Vec3f& origin, const Vec3f& inputRay, Vec3f& hitPoint) = 0;
        virtual Vec3f getNormal(const Vec3f& point) = 0;
   
    protected:
        Vec3f _color;
        Vec3f _phong;

        Vec3f _diffuse;
        Vec3f _specular;
        double _ri;
        double _albedo;
        
        bool _isReflect;
        bool _isRefract;

        bool _isLight;
};

class Sphere : public Surface
{
    public:
        Sphere() = delete;
        Sphere(const Vec3f& center, double radius);
        Sphere(const Vec3f& center, double radius, const Vec3f& color);
        

        bool hit(const Vec3f& origin, const Vec3f& inputRay, Vec3f& hitPoint);
        Vec3f getNormal(const Vec3f& point);

    private:
        Vec3f _center;
        double _radius;
};

class Triangle : public Surface
{
    public:
        Triangle() = delete;
        Triangle(const Vec3f& a, const Vec3f& b, const Vec3f& c);

        bool hit(const Vec3f& origin, const Vec3f& inputRay, Vec3f& hitPoint);
        Vec3f getNormal(const Vec3f& point);

    private:
        Vec3f _a, _b, _c;

};

#endif