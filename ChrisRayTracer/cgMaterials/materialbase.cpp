#include "materialbase.hpp"

// constructor and destructor
cgRT::MaterialBase::MaterialBase(){
    m_maxReflectionRays = 3;
    m_reflectionRayCount = 0;
}
cgRT::MaterialBase::~MaterialBase(){}

// func to compute the color of the material

qbVector<double> cgRT::MaterialBase::ComputeColor(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                  const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                  const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                  const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                  const cgRT::Ray &cameraRay){

// define inital material color
    qbVector<double> matColor {3};

return matColor;
}

// func to compute diffuse color

qbVector<double> cgRT::MaterialBase::ComputeDiffuseColor(   const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                            const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                            const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                            const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                            const qbVector<double> &baseColor){
// compute the color due to diffuse illumination

    qbVector<double> diffuseColor {3};
    double intensity;
    qbVector<double> color {3};
    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;
    bool validIllum = false;
    bool illumFound = false;

    for(auto currentLight : lightList){
        validIllum = currentLight -> ComputeIllumination(intPoint, localNormal, objectList, currentObject, color, intensity);
        if(validIllum){
            illumFound = true;
            red += color.GetElement(0) * intensity;
            green += color.GetElement(1) * intensity;
            blue += color.GetElement(2) * intensity;
        }
    }

    if(illumFound){
        diffuseColor.SetElement(0, red * baseColor.GetElement(0));
        diffuseColor.SetElement(1, green * baseColor.GetElement(1));
        diffuseColor.SetElement(2, blue * baseColor.GetElement(2));
    }

    else{
        //ambient lighting conditiom
        for(int i = 0; i<3; ++i)
            diffuseColor.SetElement(i, (m_ambientColor.GetElement(i)*m_ambientIntensity)*baseColor.GetElement(i));
    }

    //return the material color
return diffuseColor;
}

//func to compute color due to reflection

qbVector<double> cgRT::MaterialBase::ComputeReflectionColor(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                            const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                            const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                            const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                            const cgRT::Ray &incidentRay){

    qbVector<double> reflectionColor {3};

    // compute the reflection vector
    qbVector<double> d = incidentRay.m_lab;
    qbVector<double> reflectionVector = d - (2 * qbVector<double>::dot(d, localNormal)*localNormal);

    //construct the reflection ray
    cgRT::Ray reflectionRay(intPoint, intPoint + reflectionVector);

    //cast this ray into the scene and find the closest object that it intersects with
    std::shared_ptr<cgRT::ObjectBase> closestObject;
    qbVector<double> closestIntPoint {3};
    qbVector<double> closestLocalNormal {3};
    qbVector<double> closestLocalColor {3};
    bool intersectionFound = CastRay(reflectionRay, objectList, currentObject, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);

    // compute illumination for closest object assuming there was a valid intersection

    qbVector<double> matColor {3};
    if(intersectionFound && (m_reflectionRayCount < m_maxReflectionRays)){
        //increment the reflection ray count
        m_reflectionRayCount++;

        (closestObject->m_hasMaterial) ? matColor = closestObject->m_pMaterial->ComputeColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, reflectionRay) : matColor = cgRT::MaterialBase::ComputeDiffuseColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, closestObject->m_baseColor);
    }
    reflectionColor = matColor;
    
return reflectionColor;
}

//func to casta ray into a scene
bool cgRT::MaterialBase::CastRay(const cgRT::Ray &castRay, const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
             const std::shared_ptr<cgRT::ObjectBase> &thisObject, std::shared_ptr<cgRT::ObjectBase> &closestObject,
             qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal, qbVector<double> &closestLocalColor){
    //test for intersections with all of the objects within the sceen

    qbVector<double> intPoint{3};
    qbVector<double> localNormal{3};
    qbVector<double> localColor{3};

    double mindist = 1e6;
    bool intersectionFound = false;

    for(auto currentObject : objectList){
        if(currentObject != thisObject){
            bool validInt = currentObject -> TestIntersection(castRay, intPoint, localNormal, localColor);

            if(validInt){
                //set flag that we found an int point
                intersectionFound = true;

                //compute the distance between the src and int point
                double dist = (intPoint - castRay.m_point1).norm();

                //store a reference to the object if it is the closest
                if(dist<mindist){
                    mindist = dist;
                    closestObject = currentObject;
                    closestIntPoint = intPoint;
                    closestLocalNormal = localNormal;
                    closestLocalColor = localColor;
                }
            }
        }
    }
return intersectionFound;
}

//func to assign a texture
void cgRT::MaterialBase::AssignTexture(const std::shared_ptr<cgRT::Texture::TextureBase> &inputTexture){
    m_textureList.push_back(inputTexture);
    m_hasTexture = true;
}