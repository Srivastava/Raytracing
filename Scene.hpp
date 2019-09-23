#ifndef SCENE_H
#define SCENE_H

#include "Geometry.hpp"
#include "Surface.hpp"
#include "Light.hpp"
#include "Camera.hpp"

#include <memory>
#include <vector>

class Scene 
{
    public:
        Scene();
        void setBackgroundColor(const Vec3f& color);
        void setDims(int width, int height);
        void setMaxBounces(int bounces);
        void setNumSamples(int numSamples);
        
        void setAmbience(const Vec3f& ambient);

        const Vec3f& getBackgroundColor() const;

        void setCamera(Camera& camera);

        void addLight(Light& light);
        
        void addSurface(Surface& surface);

        Vec3f shadowLighting(const Vec3f& hitPoint, const Vec3f& normal, Light* light) const;
        Vec3f getLighting(const Vec3f& point, const Vec3f& cameraDir, Surface* surface) const;
        
        
        Vec3f raytrace(const Ray& ray, int bounces = 1) const;
        void raytrace() const;

        void saveImage(const std::vector<std::vector<Vec3f>>& image) const;

    private:
        Vec3f _backgroundColor;
        Vec3f _ambient;

        int _width;
        int _height;

        int _maxbounces;
        int _numSamples;

        std::vector<std::unique_ptr<Light>> _lights;
        std::vector<std::unique_ptr<Surface>> _surfaces;
        std::unique_ptr<Camera> _camera;

        Vec3f getLighting(const Vec3f& hitPoint, Surface* surface);
};
#endif
