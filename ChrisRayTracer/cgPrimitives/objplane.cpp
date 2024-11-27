#include "objplane.hpp"
#include <cmath>

//default constructor

cgRT::ObjPlane::ObjPlane(){}

//the destructor
cgRT::ObjPlane::~ObjPlane(){}

//the func to test for intersection

bool cgRT::ObjPlane::TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor){
    // copy the ray and apply the backwards transform
    cgRT::Ray bckRay = m_transformMatrix.Apply(castRay, cgRT::BCKTFORM);

    //copy the m_lab vector from bckRay and normalise

    qbVector<double> k = bckRay.m_lab;
    k.Normalize();

    //check if there is an intersection i.e. if the cast ray is not parallel to the plane
    if(!CloseEnough(k.GetElement(2), 0.0)){
        //there is an intersection
        double t = bckRay.m_point1.GetElement(2)/ -k.GetElement(2);

        // if t is negaice then the intersection point must be behind tthe camera and we can ignore it

        if(t > 0.0){
            //compute the values for u and v
            double u = bckRay.m_point1.GetElement(0) + (k.GetElement(0)*t);
            double v = bckRay.m_point1.GetElement(1) + (k.GetElement(1)*t);

            // if  the magintude of both u and v is ≤ 1 then we must be in the plane
            if((abs(u)<1.0)&&(abs(v)<1.0)){
                //compute point of intersection
                qbVector<double> poi = bckRay.m_point1 + t * k;

                //tranform the intersection point back into world coordinates
                intPoint = m_transformMatrix.Apply(poi, cgRT::FWDTFORM);

                //compute local normal
                // qbVector<double> localOrigin {std::vector<double>{0.0,0.0,0.0}};
                // qbVector<double> normalVector {std::vector<double>{0.0, 0.0, -1.0}};
                // qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, cgRT::FWDTFORM);
                // localNormal = m_transformMatrix.Apply(normalVector,cgRT::FWDTFORM) - globalOrigin;
                // localNormal.Normalize();

                qbVector<double> normalVector {std::vector<double>{0.0, 0.0, -1.0}};
                localNormal = m_transformMatrix.ApplyNorm(normalVector);
                localNormal.Normalize();

                //return base color
                localColor = m_baseColor;

                //stroe the (u,v) coordinates for possible later use
                m_uvCoords.SetElement(0, u);
                m_uvCoords.SetElement(1, v);
                return true;
            }
            else{
                    return false;
                }
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
return false;
}
