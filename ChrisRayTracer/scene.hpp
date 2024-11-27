#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include "cgImage.hpp"
#include "camera.hpp"
#include "./cgPrimitives/objectsphere.hpp"
#include "./cgPrimitives/objplane.hpp"
#include "./cgLights/pointlight.hpp"
#include "./cgPrimitives/cylinder.hpp"
#include "./cgPrimitives/cone.hpp"
#include "./cgPrimitives/box.hpp"

namespace cgRT{
    class Scene{
        public:
            Scene();

            //func to perform actual rendering
            bool Render(cgImage&outputImage);

            //func to casta ray into the scene
            bool CastRay (cgRT::Ray &castRay, std::shared_ptr<cgRT::ObjectBase> &closestObject, qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal, qbVector<double> &closestLocalColor);

        //private functions
        private:

        //private members
        private:
            //the camera we are gonna use
            cgRT::Camera m_camera;

            // list of objects in the scene
            std::vector<std::shared_ptr<cgRT::ObjectBase>> m_objectList;

            //list of lights in the scene
            std::vector<std::shared_ptr<cgRT::LightBase>> m_lightList;
    };
}

#endif