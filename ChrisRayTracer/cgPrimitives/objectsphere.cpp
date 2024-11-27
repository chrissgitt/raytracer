#include "objectsphere.hpp"
#include <cmath>

//the default constructor
cgRT::ObjSphere::ObjSphere(){}

//the destructor
cgRT::ObjSphere::~ObjSphere(){}

//func to test for intersections

bool cgRT::ObjSphere::TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor){
    //copy the ray and apply the backwards transform
    cgRT::Ray bckRay = m_transformMatrix.Apply(castRay, cgRT::BCKTFORM);
    
    // Compute the values of a, b and c
    qbVector<double> vhat = bckRay.m_lab;
    vhat.Normalize();

    //Note that a is equal to the squared magintute of the direction of the cast ray as this would be the unit vector we can conclue that the value of a will always be 1
    // a = 1.0

    //calculate b
    double b = 2.0 * qbVector<double>::dot(bckRay.m_point1, vhat);

    // clculate c
    double c = qbVector<double>::dot(bckRay.m_point1, bckRay.m_point1) - 1.0;

    //test whether we have an intersection
    double intTest = (b*b) - 4.0 * c;

    qbVector<double> poi;

    if(intTest > 0.0){
        double numSQRT = sqrtf(intTest);
        double t1 = (-b + numSQRT) / 2.0;
        double t2 = (-b - numSQRT) / 2.0;

        // if either t1 or t2 are negative then at least part of the object os behind the camera and so we will ignore it

        if(( t1 < 0.0 ) && (t2 < 0.0)) return false;
        else {
            //determine which point of intersection was closest to the camera
            if(t1 < t2){
                if(t1 > 0.0){
                    poi = bckRay.m_point1 + (vhat * t1);
                }

                else{
                    if(t2 > 0.0){
                        poi = bckRay.m_point1 + (vhat * t2);
                    }
                    else{
                        return false;
                    }
                }
            }
            else{
                if(t2 > 0.0){
                    poi = bckRay.m_point1 + (vhat * t2);
                }
                else{
                    if(t1 > 0.0){
                        poi = bckRay.m_point1 + (vhat * t1);
                    }
                    else{
                        return false;
                    }
                }
            }

            //transofrm the intersection back into world coordinates
            intPoint = m_transformMatrix.Apply(poi, cgRT::FWDTFORM);

            //compute the local normal for a sphere at origin
            // qbVector<double> objOrigin = qbVector<double>{std::vector<double>{0.0,0.0,0.0}};
            // qbVector<double> newObjOrigin = m_transformMatrix.Apply(objOrigin, cgRT::FWDTFORM);
            // localNormal = intPoint - newObjOrigin;
            // localNormal.Normalize();

            qbVector<double> normalVector = poi;
            localNormal = m_transformMatrix.ApplyNorm(normalVector);
            localNormal.Normalize();

            //return the base color
            localColor = m_baseColor;

            //compute and store (u,v) coordinates for possible later use
            double x = poi.GetElement(0);
            double y = poi.GetElement(1);
            double z = poi.GetElement(2);
            double u = atan2(sqrtf(pow(x, 2.0) + pow(y, 2.0)), z);
            double v = atan2(y, x);

            u /= M_PI;
            v /= M_PI;

            m_uvCoords.SetElement(0, u);
            m_uvCoords.SetElement(1, v);
        }
    return true;
    }
return false;
}
