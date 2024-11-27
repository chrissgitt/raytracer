#include "pointlight.hpp"

// defauult constructor

cgRT::PointLight::PointLight(){
    m_color = qbVector<double> {std::vector<double>{1.0,1.0,1.0}};
    m_intensity = 1.0;
}

//dstructor
cgRT::PointLight::~PointLight(){}

//func to compute illumination

bool cgRT::PointLight::ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                            const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                            const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                            qbVector<double> &color, double &intensity){

    //construct a vector pointing from the intersection point to the light
    qbVector<double> lightDir = (m_location - intPoint).Normalized();
    double lightDist = (m_location-intPoint).norm();

    //compute starting point
    qbVector<double> startPoint = intPoint;

    //construct a ray from the poi to the light
    cgRT::Ray lighRay (startPoint, startPoint + lightDir);

    // check for all the objects in the scene, except for the current one
    qbVector<double> poi {3};
    qbVector<double> poiNormal {3};
    qbVector<double> poiColor {3};
    bool validInt = false;

    for(auto sceneObject : objectList){
        if(sceneObject != currentObject){
            validInt = sceneObject -> TestIntersection(lighRay, poi, poiNormal, poiColor);
            if(validInt){
                double dist = (poi - startPoint).norm();
                if(dist > lightDist)
                    validInt = false;
            }
        }

        // if we have an intersection then there is no point in checking further so we can break out the loop
        // ie. this object is blovking light from the light source

        if(validInt) break;
    }

    //only continue to compute illumination if the light ray didn't intersect with any objects in the scene
    // i.e. no objects are casting a shadow from this lightsource

    if(!validInt){
        //compute the angle between the local normal and the light ray
        //note that we assume that the localNormal is a unit vectore
        double angle = acos(qbVector<double>::dot(localNormal, lightDir));

        if(angle>1.5708){
            color = m_color;
            intensity = 0.0;
            return false;
        }

        else{
            color = m_color;
            intensity = m_intensity * (1.0 - (angle/1.5708));
            return true;
        }
    }

    else{
        //shadow, i.e. no illumination
        color = m_color;
        intensity = 0.0;
        return false;
    }
}