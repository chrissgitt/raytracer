#include "scene.hpp"
#include "./cgMaterials/simplematerial.hpp"
#include "./cgMaterials/simplerefractive.hpp"
#include "./cgTextures/checker.hpp"
#include "./cgTextures/image.hpp"

cgRT::Scene::Scene(){


    // configure the camera
    m_camera.SetPosition(qbVector<double>{std::vector<double>{2.0, -5.0, -3.0}});
    m_camera.SetLookAt(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}});
    m_camera.SetUp(qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}});
    m_camera.SetHorzSize(1.0);
    m_camera.SetAspect(16.0 / 9.0);
    m_camera.UpdateCameraGeometry();

    // setup ambient lighting
    cgRT::MaterialBase::m_ambientColor = std::vector<double>{1.0,1.0,1.0};
    cgRT::MaterialBase::m_ambientIntensity = 0.2;

    // create some textures
    auto floorTexture = std::make_shared<cgRT::Texture::Checker>(cgRT::Texture::Checker());
    auto imageTexture = std::make_shared<cgRT::Texture::Image>(cgRT::Texture::Image());


    floorTexture -> setTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
    0.0,
    qbVector<double>{std::vector<double>{16.0, 16.0}});

    imageTexture->LoadImage("testImage.bmp");
    imageTexture->setTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
                                0.0,
                                qbVector<double>{std::vector<double>{1.0, 1.0}});

    
    auto floorMaterial = std::make_shared<cgRT::SimpleMaterial>(cgRT::SimpleMaterial());
    auto imageMaterial = std::make_shared<cgRT::SimpleMaterial>(cgRT::SimpleMaterial());
    auto sphereMaterial = std::make_shared<cgRT::SimpleMaterial>(cgRT::SimpleMaterial());
    auto sphereMaterial2 = std::make_shared<cgRT::SimpleMaterial>(cgRT::SimpleMaterial());
    auto sphereMaterial3 = std::make_shared<cgRT::SimpleMaterial>(cgRT::SimpleMaterial());
    auto coneMaterial = std::make_shared<cgRT::SimpleMaterial>(cgRT::SimpleMaterial());
    auto glassMaterial = std::make_shared<cgRT::SimpleRefractive>(cgRT::SimpleRefractive());

    // setup the materials

    floorMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
    floorMaterial->m_reflectivity = 0.25;
    floorMaterial->m_shininess = 0.0;
    floorMaterial->AssignTexture(floorTexture);

    imageMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 0.125, 0.125}};
    imageMaterial->m_reflectivity = 0.0;
    imageMaterial->m_shininess = 0.0;
    imageMaterial->AssignTexture(imageTexture);

    sphereMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 0.2, 0.2}};
    sphereMaterial->m_reflectivity = 0.8;
    sphereMaterial->m_shininess = 32.0;

    sphereMaterial2->m_baseColor = qbVector<double>{std::vector<double>{0.2, 1.0, 0.2}};
    sphereMaterial2->m_reflectivity = 0.8;
    sphereMaterial2->m_shininess = 32.0;

    sphereMaterial3->m_baseColor = qbVector<double>{std::vector<double>{0.2, 0.2, 1.0}};
    sphereMaterial3->m_reflectivity = 0.8;
    sphereMaterial3->m_shininess = 32.0;

    coneMaterial->m_baseColor = qbVector<double>{std::vector<double>{0.8, 0.8, 0.2}};
    coneMaterial->m_reflectivity = 0.15;
    coneMaterial->m_shininess = 32.0;

    glassMaterial->m_baseColor = qbVector<double>{std::vector<double>{0.7, 0.7, 0.2}};
    glassMaterial->m_reflectivity = 0.25;
    glassMaterial->m_shininess = 32.0;
    glassMaterial->m_translucency = 0.75;
    glassMaterial->m_ior = 1.333;

    //create and setup some objects

    auto floor = std::make_shared<cgRT::ObjPlane>(cgRT::ObjPlane());
    floor->SetTransformMatrix(cgRT::GTform{qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}},
                                           qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                                           qbVector<double>{std::vector<double>{16.0, 16.0, 1.0}}});
    floor->AssignMaterial(floorMaterial);

    auto imagePlane = std::make_shared<cgRT::ObjPlane>(cgRT::ObjPlane());
    imagePlane->SetTransformMatrix(cgRT::GTform{qbVector<double>{std::vector<double>{0.0, 5.0, -0.75}},
                                                qbVector<double>{std::vector<double>{-M_PI/2.0, 0.0, 0.0}},
                                                qbVector<double>{std::vector<double>{1.75, 1.75, 1.0}}});
    imagePlane->AssignMaterial(imageMaterial);

    auto sphere = std::make_shared<cgRT::ObjSphere>(cgRT::ObjSphere());
    sphere->SetTransformMatrix(cgRT::GTform{qbVector<double>{std::vector<double>{-2.0, -2.0, 0.25}},
                                            qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                                            qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}}});
    sphere->AssignMaterial(sphereMaterial);

    auto sphere2 = std::make_shared<cgRT::ObjSphere>(cgRT::ObjSphere());
    sphere2->SetTransformMatrix(cgRT::GTform{qbVector<double>{std::vector<double>{-2.0, -0.5, 0.25}},
                                             qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                                             qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}}});
    sphere2->AssignMaterial(sphereMaterial2);

    auto sphere3 = std::make_shared<cgRT::ObjSphere>(cgRT::ObjSphere());
    sphere3->SetTransformMatrix(cgRT::GTform{qbVector<double>{std::vector<double>{-2.0, -1.25, -1.0}},
                                             qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                                             qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}}});
    sphere3->AssignMaterial(sphereMaterial3);

    // auto sphere4 = std::make_shared<cgRT::ObjSphere>(cgRT::ObjSphere());
    // sphere4->SetTransformMatrix(cgRT::GTform{qbVector<double>{std::vector<double>{2.0, -1.25, 0.25}},
    //                                          qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
    //                                          qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}}});
    // sphere4->AssignMaterial(glassMaterial);

    // Put the objects into the scene.

    auto box = std::make_shared<cgRT::Box>(cgRT::Box());
    box->SetTransformMatrix(cgRT::GTform{qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                                         qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                                         qbVector<double>{std::vector<double>{0.5, 2.0, 1.0}}});
    box->AssignMaterial(coneMaterial);

    m_objectList.push_back(floor);
    m_objectList.push_back(imagePlane);
    m_objectList.push_back(sphere);
    m_objectList.push_back(sphere2);
    m_objectList.push_back(sphere3);
    m_objectList.push_back(box);
    //m_objectList.push_back(sphere4);

    // construct a test light
    m_lightList.push_back(std::make_shared<cgRT::PointLight>(cgRT::PointLight()));
    m_lightList.at(0)->m_location = qbVector<double>{std::vector<double>{3.0, -10.0, -5.0}};
    m_lightList.at(0)->m_color = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
    m_lightList.at(0)->m_intensity = 4.0;

    m_lightList.push_back(std::make_shared<cgRT::PointLight>(cgRT::PointLight()));
    m_lightList.at(1)->m_location = qbVector<double>{std::vector<double>{0.0, -10.0, -5.0}};
    m_lightList.at(1)->m_color = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
    m_lightList.at(1)->m_intensity = 2.0;

}

// function to perform rendering
bool cgRT::Scene::Render(cgImage &outputImage)
{
    int xSize = outputImage.GetXSize();
    int ySize = outputImage.GetYSize();

    // loop over each pixel in or image
    cgRT::Ray cameraRay;
    qbVector<double> intPoint(3);
    qbVector<double> localNormal(3);
    qbVector<double> localColor(3);
    double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
    double yFact = 1.0 / (static_cast<double>(ySize / 2.0));
    double minDist = 1e6;
    double maxDist = 0.0;

    for (int y = 0; y < ySize; ++y)
    {
        std::cout << "Processing by " << y << " of " << ySize << " \r";
        std::cout.flush();
        for (int x = 0; x < xSize; ++x)
        {
            // normalise the x and y coords
            double normX = (static_cast<double>(x) * xFact) - 1.0;
            double normY = (static_cast<double>(y) * yFact) - 1.0;

            // Generate a ray for the pixel
            m_camera.GenerateRay(normX, normY, cameraRay);

            std::shared_ptr<cgRT::ObjectBase> closestObject;
            qbVector<double> closestIntPoint{3};
            qbVector<double> closestLocalNormal{3};
            qbVector<double> closestLocalColor{3};
            bool intersectionFound = CastRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);

            if (intersectionFound)
            {
                if (closestObject->m_hasMaterial)
                {
                    cgRT::MaterialBase::m_reflectionRayCount = 0;
                    qbVector<double> color = closestObject->m_pMaterial->ComputeColor(m_objectList, m_lightList, closestObject, closestIntPoint, closestLocalNormal, cameraRay);
                    outputImage.SetPixel(x, y, color.GetElement(0), color.GetElement(1), color.GetElement(2));
                }

                else
                {
                    // define a static methoc as the base method to compute the color
                    qbVector<double> matColor = cgRT::MaterialBase::ComputeDiffuseColor(m_objectList, m_lightList, closestObject, closestIntPoint, closestLocalNormal, closestObject->m_baseColor);
                    outputImage.SetPixel(x, y, matColor.GetElement(0), matColor.GetElement(1), matColor.GetElement(2));
                }
            }
        }
    }
    std::cout << std::endl;
    return true;
}

// func to cast a ray into the scene

bool cgRT::Scene::CastRay(cgRT::Ray &castRay, std::shared_ptr<cgRT::ObjectBase> &closestObject, qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal, qbVector<double> &closestLocalColor)
{
    qbVector<double> intPoint{3};
    qbVector<double> localNormal{3};
    qbVector<double> localColor{3};
    double minDist = 1e6;
    bool intersectionFound = false;

    // test for intersections with all objects in the scene
    for (auto currentObject : m_objectList)
    {
        bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

        // if we have a valid intersection
        if (validInt)
        {
            // set flag to indicate intersection
            intersectionFound = true;

            // compute the distance between the camera and the poi
            double dist = (intPoint - castRay.m_point1).norm();

            // if this  object is closer to the camera than any one that we have seen before then store a reference to it
            if (dist < minDist)
            {
                minDist = dist;
                closestObject = currentObject;
                closestIntPoint = intPoint;
                closestLocalNormal = localNormal;
                closestLocalColor = localColor;
            }
        }
    }
    return intersectionFound;
}