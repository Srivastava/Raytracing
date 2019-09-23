#include "Geometry.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Ray.hpp"
#include "Surface.hpp"
#include "Light.hpp"
#include "OrthographicCamera.hpp"
#include "Perspective.hpp"

#include <iostream>
#include <array>
#include <vector>

int main()
{
    const int width = 480, height = 480;
    Scene scene;
    scene.setDims(width, height);
    scene.setBackgroundColor({0.3, 0.3, 0.3});
    scene.setMaxBounces(10);
    scene.setNumSamples(20);
    scene.setAmbience({0.2, 0.2, 0.2});

    Perspective persp({7.0, 2.5, 0.0}, {0.0, 2.5, 0.0}, {0.0, -1.0, 0.0}, width, height, 45.0);
    scene.setCamera(persp);

    // Top Light Surface
    Vec3f A{-0.5, 5.0, -0.5}, B{-0.5,  5.0, 0.5}, C{0.5,  5.0, 0.5}, D{0.5,  5.0, -0.5};
    Vec3f A_light{-0.5, 4.99, -0.5}, B_light{-0.5, 4.99, 0.5}, C_light{0.5, 4.99, 0.5}, D_light{0.5, 4.99, -0.5};

    Vec3f areaLightColor{1.0, 1.0, 1.0};
    Area areaLight(A_light, B_light, C_light, D_light, {0.0, -1.0, 0.0}, 100.0, areaLightColor);
    scene.addLight(areaLight);

    Triangle topLight1(A, D, C), topLight2(A, C, B);
    topLight1.setColor(areaLightColor);
    topLight2.setColor(areaLightColor);
    topLight1.setLight(true);
    topLight2.setLight(true);
    scene.addSurface(topLight1);
    scene.addSurface(topLight2);
    
    //Box vertices
    Vec3f a{-2.0, 0.0, -2.0}, b{2.0, 5.0, -2.0}, c{2.0, 0.0, -2.0}, d{-2.0, 5.0, -2.0}, e{-2.0, 0.0, 2.0}, f{-2.0, 5.0, 2.0}, g{2.0, 0.0, 2.0}, h{2.0, 5.0, 2.0};
    
    // left
    Triangle t1(a, c, b), t2(a, b, d);
    t1.setDiffuse({1.0, 0.0, 0.0});
    t2.setDiffuse({1.0, 0.0, 0.0});
    
    t1.setRI(0.0);
    t1.setAlbedo(0.8);
    t1.setPhong({0.7, 0.5, 0.3});

    t2.setRI(0.0);
    t2.setAlbedo(0.8);
    t2.setPhong({0.7, 0.5, 0.3});

    t2.setReflect(false);
    t2.setRefract(false);
    t1.setReflect(false);
    t1.setRefract(false);

    // // back
    Triangle t3(a, f, e), t4(a, d, f);

    t3.setDiffuse({0.0, 0.0, 1.0});
    t4.setDiffuse({0.0, 0.0, 1.0});

    t3.setRI(0.0);
    t3.setAlbedo(0.8);
    t3.setPhong({0.7, 0.5, 0.3});

    t4.setRI(0.0);
    t4.setAlbedo(0.8);
    t4.setPhong({0.7, 0.5, 0.3});

    t3.setReflect(false);
    t3.setRefract(false);
    t4.setReflect(false);
    t4.setRefract(false);

    // bottom
    Triangle t5(a, g, c), t6(a, e, g);
    t5.setDiffuse({1.0, 1.0, 1.0});
    t6.setDiffuse({1.0, 1.0, 1.0});
    
    t5.setRI(0.0);
    t5.setAlbedo(0.8);
    t5.setPhong({0.7, 0.5, 0.3});

    t6.setRI(0.0);
    t6.setAlbedo(0.8);
    t6.setPhong({0.7, 0.5, 0.3});

    
    t5.setReflect(false);
    t5.setRefract(false);
    t6.setReflect(false);
    t6.setRefract(false);

    // top
    Triangle t7(d, h, f), t8(d, b, h);
    t7.setDiffuse({1.0, 1.0, 1.0});
    t8.setDiffuse({1.0, 1.0, 1.0});
    
    t7.setRI(0.0);
    t7.setAlbedo(0.8);
    t7.setPhong({0.7, 0.5, 0.0});

    t8.setRI(0.0);
    t8.setAlbedo(0.8);
    t8.setPhong({0.7, 0.5, 0.0});

    t7.setReflect(false);
    t7.setRefract(false);
    t8.setReflect(false);
    t8.setRefract(false);

    // // right
    Triangle t9(h, e, f), t10(h, g, e);
    t9.setDiffuse({0.0, 1.0, 0.0});
    t10.setDiffuse({0.0, 1.0, 0.0});
    

    t9.setRI(0.0);
    t9.setAlbedo(0.8);
    t9.setPhong({0.7, 0.5, 0.3});

    t10.setRI(0.0);
    t10.setAlbedo(0.8);
    t10.setPhong({0.7, 0.5, 0.3});
    
    t9.setReflect(false);
    t9.setRefract(false);
    t10.setReflect(false);
    t10.setRefract(false);

    scene.addSurface(t1);
    scene.addSurface(t2);
    scene.addSurface(t3);
    scene.addSurface(t4);
    scene.addSurface(t5);
    scene.addSurface(t6);
    scene.addSurface(t7);
    scene.addSurface(t8);
    scene.addSurface(t9);
    scene.addSurface(t10);


    Sphere sphere({0.0, 1.0, 0.0}, 1.0);
    sphere.setDiffuse({0.8, 0.3, 0.3});
    sphere.setSpecular({0.1, 0.4, 0.2});
    sphere.setRI(1.5);
    sphere.setAlbedo(0.8);
    sphere.setReflect(false);
    sphere.setRefract(false);
    sphere.setPhong({0.4, 0.6, 0.9});

    Sphere sphere2({-1.0, 4.0, 1.0}, 1.0);
    sphere2.setDiffuse({0.2, 0.3, 0.9});
    sphere2.setSpecular({0.1, 0.4, 0.2});
    sphere2.setRI(0.0);
    sphere2.setAlbedo(0.8);
    sphere2.setPhong({0.7, 0.5, 0.3});
    sphere2.setReflect(true);
    sphere2.setRefract(false);

    Sphere sphere3({1.0, 3.0, -1.0}, 1.0);
    sphere3.setDiffuse({0.2, 1.0, 0.2});
    sphere3.setSpecular({0.1, 0.4, 0.2});
    sphere3.setRI(1.5);
    sphere3.setAlbedo(0.8);
    sphere3.setPhong({0.7, 0.5, 0.3});
    sphere3.setReflect(false);
    sphere3.setRefract(true);

    scene.addSurface(sphere);
    scene.addSurface(sphere2);
    scene.addSurface(sphere3);
    scene.raytrace();

    return 0;
}
