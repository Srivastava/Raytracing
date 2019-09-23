cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(main)

set(target main)

include_directories(./)

add_executable(${target}
            main
            ./Scene
            ./Camera
            ./OrthographicCamera
            ./Perspective
            ./Light
            ./Ray
            ./Scene
            ./Surface
            ./Geometry
            )           