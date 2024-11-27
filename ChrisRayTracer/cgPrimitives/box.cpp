#include "box.hpp"
#include <cmath>

cgRT::Box::Box(){}

cgRT::Box::~Box(){}

//func to test for intersection
bool cgRT::Box::TestIntersection(const cgRT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor){
    if(!m_isVisible) return false;

    //copy the ray and apply the backwards transform
    cgRT::Ray bckRay = m_transformMatrix.Apply(castRay, cgRT::BCKTFORM);

    //extract the values of a.
    double ax = bckRay.m_point1.GetElement(0);
    double ay = bckRay.m_point1.GetElement(1);
    double az = bckRay.m_point1.GetElement(2);

    //extract the value of k
    qbVector<double> k = bckRay.m_lab;
    k.Normalize();
    double kx = k.GetElement(0);
    double ky = k.GetElement(1);
    double kz = k.GetElement(2);

    //test for intersections with each plane (side of the box)
    if(!CloseEnough(kz, 0.0)){
        t[0] = (az - 1.0) / -kz;
        t[1] = (az + 1.0) / -kz;
        u[0] = ax + kx * t[0];
        v[0] = ay + ky * t[0];
        u[1] = ax + kx * t[1];
        v[1] = ay + ky * t[1];
    }
    else{
        t[0] = 100e6;
        t[1] = 100e6;
        u[0] = 0.0;
        v[0] = 0.0;
        u[1] = 0.0;
        v[1] = 0.0;
    }

    // left and right
    if(!CloseEnough(kx, 0.0)){
        t[2] = (ax + 1.0) / -kx;
        t[3] = (ax - 1.0) / -kx;
        u[2] = az + kz * t[2];
        v[2] = ay + ky * t[2];
        u[3] = az + kz * t[3];
        v[3] = ay + ky * t[3];
    }

    else{
        t[2] = 100e6;
        t[3] = 100e6;
        u[2] = 0.0;
        v[2] = 0.0;
        u[3] = 0.0;
        v[3] = 0.0;
    }

    // front and back

    if (!CloseEnough(kx, 0.0)){
        t[4] = (ay + 1.0) / -ky;
        t[5] = (ay - 1.0) / -ky;
        u[4] = ax + kx * t[4];
        v[4] = az + kz * t[4];
        u[5] = ax + kx * t[5];
        v[5] = az + kz * t[5];
    }

    else{
        t[4] = 100e6;
        t[5] = 100e6;
        u[4] = 0.0;
        v[4] = 0.0;
        u[5] = 0.0;
        v[5] = 0.0;
    }

    // find the index of the smallest non-negative value of t
    double finalU = 0.0;
    double finalV = 0.0;
    double finalT = 100e6;
    int finalIndex = 0;
    bool validIntersection = false;

    for(int i = 0; i < 6; ++i){
        if((t[i] < finalT) && (t[i] > 0.0) && (abs(u[i]) <= 1.0) && (abs(v[i]) <= 1.0)){
            finalT = t[i];
            finalIndex = i;
            finalU = u[i];
            finalV = v[i];
            validIntersection = true;
        }
    }

    if(validIntersection){
        //compute the point of intersection
        qbVector<double> poi = bckRay.m_point1 + finalT * k;

        //compute the normal vector
        qbVector<double> normalVector {3};

        switch(finalIndex){
            case 0:
                normalVector = std::vector<double>{0.0, 0.0, 1.0};  // down
                break;
            
            case 1:
                normalVector = std::vector<double>{0.0, 0.0, -1.0}; // up
                break;
            
            case 2:
                normalVector = std::vector<double>{-1.0, 0.0, 0.0}; // left
                break;

            case 3:
                normalVector = std::vector<double>{1.0, 0.0, 0.0};  // right
                break;
            
            case 4:
                normalVector = std::vector<double>{0.0, -1.0, 0.0}; // back
                break;

            case 5:
                normalVector = std::vector<double>{0.0, 1.0, 0.0}; // front
                break;
        }

        //transform the intersection point back into world coordinates
        intPoint = m_transformMatrix.Apply(poi, cgRT::FWDTFORM);

        //transform the normal vector
        localNormal = m_transformMatrix.ApplyNorm(normalVector);
        localNormal.Normalize();

        //return the base color
        localColor = m_baseColor;

        //store the (u,v) coords for possible later use
        m_uvCoords.SetElement(0, finalU);
        m_uvCoords.SetElement(1, finalV);

    return true;
    }

    else{
        return false;
    }
}