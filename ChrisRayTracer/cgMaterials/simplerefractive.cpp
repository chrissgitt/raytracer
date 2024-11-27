#include "simplerefractive.hpp"

cgRT::SimpleRefractive::SimpleRefractive() {};

cgRT::SimpleRefractive::~SimpleRefractive() {};

// func to return the color

qbVector<double> cgRT::SimpleRefractive::ComputeColor(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                      const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                      const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                      const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                      const cgRT::Ray &cameraRay)
{
    // define the initial colors
    qbVector<double> matColor{3};
    qbVector<double> refColor{3};
    qbVector<double> difColor{3};
    qbVector<double> spcColor{3};
    qbVector<double> trnColor{3};

    // compute the diffuse component
    if (!m_hasTexture)
        difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor);
    else
        difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_textureList.at(0)->GetColor(currentObject->m_uvCoords));

    // compute reflective component
    if (m_reflectivity > 0.0)
        refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

    // combine the reflection and diffuse components
    matColor = (refColor * m_reflectivity) + (difColor * (1.0 - m_reflectivity));

    // compute the refractive component
    if (m_translucency > 0.0)
        trnColor = ComputeTranslucency(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

    // combine with the current color
    matColor = (trnColor * m_translucency) + (matColor * (1.0 - m_translucency));

    // compute the specular component
    if (m_shininess > 0.0)
        spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);

    // add the specular component
    matColor = matColor + spcColor;

    return matColor;
}

// func to compute color due to translucency
qbVector<double> cgRT::SimpleRefractive::ComputeTranslucency(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                             const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                             const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                             const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                             const cgRT::Ray &incidentRay)
{
    qbVector<double> trnColor{3};

    // compute the refractive vector
    qbVector<double> p = incidentRay.m_lab;
    p.Normalize();
    qbVector<double> tempNormal = localNormal;
    double r = 1.0 / m_ior;
    double c = -qbVector<double>::dot(tempNormal, p);
    if (c < 0.0){
        tempNormal = tempNormal * -1.0;
        c = -qbVector<double>::dot(tempNormal, p);
    }

    qbVector<double> refractedVector = r*p + (r*c - sqrtf(1.0-pow(r, 2.0) * (1.0-pow(c, 2.0)))) * tempNormal;

    // construct the refracted ray
    cgRT::Ray refractedRay(intPoint + (refractedVector * 0.01), intPoint + refractedVector);

    // test for secondary intersection with this object
    std::shared_ptr<cgRT::ObjectBase> closestObject;
    qbVector<double> closestIntPoint{3};
    qbVector<double> closestLocalNormal{3};
    qbVector<double> closestLocalColor{3};
    qbVector<double> newIntPoint{3};
    qbVector<double> newLocalNormal{3};
    qbVector<double> newLocalColor{3};

    bool test = currentObject->TestIntersection(refractedRay, newIntPoint, newLocalNormal, newLocalColor);
    bool intersecionFound = false;
    cgRT::Ray finalRay;

    if (test){
        // compute the refracted vector
        qbVector<double> p2 = refractedRay.m_lab;
        p2.Normalize();
        qbVector<double> tempNormal2 = newLocalNormal;
        double r2 = m_ior;
        double c2 = -qbVector<double>::dot(tempNormal2, p2);

        if (c2 < 0.0){
            tempNormal2 = tempNormal2 * -1.0;
            c2 = -qbVector<double>::dot(tempNormal2, p2);
        }
        qbVector<double> refractedVector2 = r2*p2 + (r2*c2 - sqrtf(1.0-pow(r2, 2.0) * (1.0 - pow(c2, 2.0)))) * tempNormal2;

        // compute the refracted ray
        cgRT::Ray refractedRay2(newIntPoint + (refractedVector2 * 0.01), newIntPoint + refractedVector2);

        // cast this ray into the scene
        intersecionFound = CastRay(refractedRay2, objectList, currentObject, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);
        finalRay = refractedRay2;
    }

    else{
        // no secondary intersections were found -> continue original refracted ray
        intersecionFound = CastRay(refractedRay, objectList, currentObject, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);
        finalRay = refractedRay;
    }

    // compute the color for the closest object
    qbVector<double> matColor{3};
    if (intersecionFound){
        if (closestObject->m_hasMaterial){
            matColor = closestObject->m_pMaterial->ComputeColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, finalRay);
        }
        else{
            matColor = cgRT::MaterialBase::ComputeDiffuseColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, closestObject->m_baseColor);
        }
    }
    else
    {
        // leave matColor as it is
    }
    trnColor = matColor;
    return trnColor;
}

// func to compute the specular highlights

qbVector<double> cgRT::SimpleRefractive::ComputeSpecular(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                         const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                         const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                         const cgRT::Ray &cameraRay)
{
    qbVector<double> specColor{3};
    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;

    // loop through lights in the scene

    for (auto currentLight : lightList)
    {
        // check for intersections with all objects in the scene
        double intensity = 0.0;

        // construct a vector pointing from the intersection point to the light
        qbVector<double> lightDir = (currentLight->m_location - intPoint).Normalized();

        // compute start point
        qbVector<double> startPoint = intPoint + (lightDir * 0.001);

        // construct a ray from the point to the light
        cgRT::Ray lightRay(startPoint, startPoint + lightDir);

        // loop through all objects in the scene to check if any obstruct light from this source
        qbVector<double> poi{3};
        qbVector<double> poiNormal{3};
        qbVector<double> poiColor{3};
        bool validInt = false;

        for (auto sceneObject : objectList)
        {
            validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
            if (validInt)
                break;
        }

        // if no intersections were found then proceed with computing the specular component
        if (!validInt)
        {
            // compute the reflection vector
            qbVector<double> d = lightRay.m_lab;
            qbVector<double> r = d - (2 * qbVector<double>::dot(d, localNormal) * localNormal);
            r.Normalize();

            // compute the dot product
            qbVector<double> v = cameraRay.m_lab;
            v.Normalize();
            double dotProduct = qbVector<double>::dot(r, v);

            // only proceed if dot product is positiv
            if (dotProduct > 0.0)
            {
                intensity = m_reflectivity * std::pow(dotProduct, m_shininess);
            }
        }

        red += currentLight->m_color.GetElement(0) * intensity;
        green += currentLight->m_color.GetElement(1) * intensity;
        blue += currentLight->m_color.GetElement(2) * intensity;
    }
    specColor.SetElement(0, red);
    specColor.SetElement(1, green);
    specColor.SetElement(2, blue);

    return specColor;
}
