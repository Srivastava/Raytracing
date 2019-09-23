#include "Scene.hpp"
#include <limits>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <omp.h>

Scene::Scene() : _backgroundColor(0.0f, 0.0f, 0.0f)
{
}

void Scene::addLight(Light& light)
{
    _lights.emplace_back(std::unique_ptr<Light>());
    _lights.back().reset(&light);
}

void Scene::addSurface(Surface& surface)
{
    _surfaces.emplace_back(std::unique_ptr<Surface>());
    _surfaces.back().reset(&surface);
}

void Scene::setDims(int width, int height)
{
    this->_width = width;
    this->_height = height;
}

void Scene::setCamera(Camera& camera)
{
    this->_camera.reset(&camera);
}

const Vec3f& Scene::getBackgroundColor() const
{
    return _backgroundColor;
}

void Scene::setBackgroundColor(const Vec3f& color)
{
    this->_backgroundColor = color;
}

void Scene::setMaxBounces(int bounces)
{
    this->_maxbounces = bounces;
}

void Scene::setNumSamples(int numSamples)
{
    this->_numSamples = numSamples;
}
void Scene::setAmbience(const Vec3f& ambient)
{
    this->_ambient = ambient;
}

Vec3f Scene::shadowLighting(const Vec3f& hitPoint, const Vec3f& normal, Light* light) const
{
    float tmin = light->distance(hitPoint);
    tmin *= tmin;

    Vec3f modifiedHit = hitPoint + normal * 1e-3;
    const int shadowSamples = 15;
    Vec3f color{0.0f, 0.0f, 0.0f};

    for(int i = 0; i < shadowSamples; ++i)
    {
        Ray ray(modifiedHit, light->direction(modifiedHit).normalize() * -1.0f);

        Vec3f shadowHitPoint;
        bool shadow = false;
        for(size_t i = 0; i < this->_surfaces.size(); ++i)
        {
            if(not _surfaces[i]->hit(ray.getOrigin(), ray.getDirection(), shadowHitPoint) or (shadowHitPoint - ray.getOrigin()).abs2() >= tmin)
                continue;
            
            shadow = true;
            break;
        }
        if(not shadow)
            color += light->color(hitPoint);
    }
    return color/(float)shadowSamples;
}

Vec3f Scene::getLighting(const Vec3f& hitPoint, const Vec3f& cameraDir, Surface* surface) const
{
    Vec3f color(0.0, 0.0, 0.0);
    Vec3f phong = surface->phong();

    Vec3f normal = surface->getNormal(hitPoint).normalize();
    // Vec3f modifiedHit = hitPoint + normal * 1e-3;

    for(int i = 0; i < this->_lights.size(); ++i)
    {
        Vec3f intensity = shadowLighting(hitPoint, normal, _lights[i].get());
        Vec3f incidentDir = this->_lights[i]->direction(hitPoint) * -1.0f;

        double direction = normal.dot(incidentDir.normalize());
        Vec3f diffuse = intensity * std::max(0.0, direction);
        
        Vec3f h = cameraDir * -1.0f + incidentDir;
        Vec3f specular = intensity * std::pow(std::max(double(0), h.normalize().dot(normal)), 15);

        color += diffuse * phong[1] + specular * phong[2];
    }

    return (color) * surface->diffuse() + _ambient;
}

float fresnal(const Vec3f& incidentRay, const Vec3f& normal, const double ri)
{
    float cosi = incidentRay.dot(normal);
    cosi = cosi < -1 ? -1.0f : cosi > 1 ? 1.0f : cosi;
    float air = 1, material = ri;

    if(cosi > 0)
        std::swap(air, material);
    
    float sint = (air / material) * sqrtf((1.0f - cosi * cosi));
    
    //Total internal reflection
    if(sint >= 1)
        return 1;

    float cost = sqrtf(std::max(0.0f, 1.0f - sint * sint));
    cosi = fabsf(cosi);
    float Fpar = ((material * cosi) - (air * cost)) / ((material * cosi) + (air * cost));
    float Fperp = ((air * cosi) - (material * cost)) / ((air * cosi) + (material * cost));

    float Fr = 0.5 * (Fpar * Fpar + Fperp * Fperp);

    return Fr;
}

Ray reflectDirection(const Vec3f& incidentRay, const Vec3f& normal, const Vec3f& hitPoint)
{
    Vec3f reflectDir = incidentRay - normal * (2.0f * normal.dot(incidentRay));
    float direction = reflectDir.dot(normal);
    Vec3f modifiedHit = hitPoint + normal * 1e-3f * (direction > 0 ? -1.0f : 1.0f);
    return Ray(modifiedHit, reflectDir.normalize());
}

Ray refractDirection(const Vec3f& incidentRay, const Vec3f& normal, const Vec3f& hitPoint, const double ri)
{
    double cosi = incidentRay.dot(normal);
    double air = 1.0f, material = ri;
    Vec3f norm = normal;
    if(cosi < 0)
        cosi = -cosi;
    else
    {
        std::swap(air, material);
        norm *= -1.0f;
    }

    double eta = air / material;
    double k = 1.0f - eta * eta * (1.0f - cosi * cosi);

    Vec3f refractDir = incidentRay * eta + norm * (eta * cosi - sqrtf(k));
    float direction = incidentRay.dot(normal);
    Vec3f modifiedHit = hitPoint + normal * 1e-3f * (direction < 0 ? -1.0f : 1.0f);

    return Ray(modifiedHit, refractDir.normalize());
}

Vec3f Scene::raytrace(const Ray& ray, int bounces) const
{
    if(bounces > _maxbounces)
        return {0.0f, 0.0f, 0.0f};

    float tmin = std::numeric_limits<float>::max();
    Vec3f hitPoint;
    bool hitOnce = false;
    

    Vec3f color(0.0f, 0.0f, 0.0f);

    for(size_t i = 0; i < this->_surfaces.size(); ++i)
    {
        if(not _surfaces[i]->hit(ray.getOrigin(), ray.getDirection(), hitPoint) or (hitPoint - ray.getOrigin()).abs2() >= tmin)
            continue;

        color = {0.0f, 0.0f, 0.0f};
        hitOnce = true;
        tmin    = (hitPoint - ray.getOrigin()).abs2();

        if(_surfaces[i]->isLight())
        {
            color = _surfaces[i]->color();
            continue;
        }

        bool reflect = _surfaces[i]->reflect();
        bool refract = _surfaces[i]->refract();

        if(reflect)
        {
            Vec3f normal =  _surfaces[i]->getNormal(hitPoint).normalize();;
            Ray reflectRay = reflectDirection(ray.getDirection(), normal, hitPoint);
            color += raytrace(reflectRay, bounces + 1);/* * _surfaces[i]->diffuse();*/
        }
        else if(refract)
        {
            Vec3f normal =  _surfaces[i]->getNormal(hitPoint).normalize();
            double fr = fresnal(ray.getDirection(),  normal, _surfaces[i]->refractiveIndex());
            
            Ray reflectRay = reflectDirection(ray.getDirection(), normal, hitPoint);
            color += raytrace(reflectRay, bounces + 1) * fr;
            if(fr < 1)
            {
                Ray refractedRay = refractDirection(ray.getDirection(), normal, hitPoint, _surfaces[i]->refractiveIndex());
                color += raytrace(refractedRay, bounces + 1) * (1.0f - fr);
            }
        }
        else
        {
            // Direct Illumination using Phong Model
            color += getLighting(hitPoint, ray.getDirection(), this->_surfaces[i].get());
        }
    }
    return hitOnce ? color : _backgroundColor;
}

void Scene::raytrace() const
{
    std::vector<std::vector<Vec3f>> image(_width, std::vector<Vec3f>(_height, Vec3f(0.0f,0.0f,0.0f)));
    for(int k = 0; k < _numSamples; ++k)
    {
        #pragma omp parallel
        #pragma omp parallel for
        for(int i = 0; i < _width; ++i)
        {
            for(int j = 0; j < _height; ++j)
            {
                // std::cout << i << " " << j << std::endl;
                double random = ((double) rand() / (RAND_MAX)) + 1;
                Vec2f point(((double)i + random )/ (double)_width, ((double)j + random)/ (double)_height);
                Ray ray(this->_camera->generateRay(point));
                image[i][j] += (raytrace(ray) - image[i][j]) / (k + 1);
            }
        }
        saveImage(image);
    }
}

void Scene::saveImage(const std::vector<std::vector<Vec3f>>& image) const
{
    std::string filename = "test.ppm";
    std::ofstream inp(filename.c_str(), std::ios::out | std::ios::binary);
    if (inp.is_open()) 
    {
        inp << "P6\n";
        inp << _width << "\n";
        inp << _height << "\n";
        inp << 255 << "\n";

        unsigned int size = _width * _height;
        int i = 0;
        std::vector<unsigned char> temp(size * 3);
        for (int y = 0; y < _height; ++y) 
        {
            for (int x = 0; x < _width; ++x) 
            {
                Vec3f v = image[x][y];
                v *= 255;
                v[0] = v[0] < 0 ? 0 : v[0] > 255 ? 255 : v[0];
                v[1] = v[1] < 0 ? 0 : v[1] > 255 ? 255 : v[1];
                v[2] = v[2] < 0 ? 0 : v[2] > 255 ? 255 : v[2];

                temp[i * 3]     = static_cast<unsigned char>(v[0]);
                temp[i * 3 + 1] = static_cast<unsigned char>(v[1]);
                temp[i * 3 + 2] = static_cast<unsigned char>(v[2]);
                ++i;  
            }
        }
        inp.write(reinterpret_cast<char*>(&temp[0]), size * 3);
    }
    inp.close();
}