#include "simplematerial.hpp"

cgRT::SimpleMaterial::SimpleMaterial() {}
cgRT::SimpleMaterial::~SimpleMaterial() {}

// func to return the color

qbVector<double> cgRT::SimpleMaterial::ComputeColor(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                    const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                    const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                    const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                    const cgRT::Ray &cameraRay)
{
    // define the initial material colors

    qbVector<double> matColor{3};
    qbVector<double> refColor{3};
    qbVector<double> diffColor{3};
    qbVector<double> specColor{3};

    // compute the diffuse component
    (!m_hasTexture)?diffColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor): diffColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_textureList.at(0) -> GetColor(currentObject -> m_uvCoords));
    
    // compute the reflection component

    if (m_reflectivity > 0.0){
        refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);
    }

    // combine reflection and diffuse compnonts
    matColor = (refColor * m_reflectivity) + (diffColor * (1 - m_reflectivity));

    // compute the specular component
    if (m_shininess > 0.0){
        specColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);
    }

    // add the specual component to the color

    matColor = matColor + specColor;

    return matColor;
}

// func to compute the specualar highlights

qbVector<double> cgRT::SimpleMaterial::ComputeSpecular(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
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