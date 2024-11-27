#include "cone.hpp"
#include <cmath>

cgRT::Cone::Cone() {}
cgRT::Cone::~Cone() {}

// func to test for intersections
bool cgRT::Cone::TestIntersection(const cgRT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor)
{
    // copy the ray and apply the backwards transform
    cgRT::Ray bckRay = m_transformMatrix.Apply(castRay, cgRT::BCKTFORM);

    // copy the m_lab vector from bckRay and normalise it
    qbVector<double> v = bckRay.m_lab;
    v.Normalize();

    // get start point of the line
    qbVector<double> p = bckRay.m_point1;

    // compute the a, b and c
    double a = std::pow(v.GetElement(0), 2.0) + std::pow(v.GetElement(1), 2.0) - std::pow(v.GetElement(2), 2.0);
    double b = 2.0 * (p.GetElement(0) * v.GetElement(0) + p.GetElement(1) * v.GetElement(1) - p.GetElement(2) * v.GetElement(2));
    double c = std::pow(p.GetElement(0), 2.0) + std::pow(p.GetElement(1), 2.0) - std::pow(p.GetElement(2), 2.0);

    // compute b^2 - 4ac
    double numSQRT = sqrtf(std::pow(b, 2.0) - 4 * a * c);

    // test for intersections
    // first with cylinder itself

    std::array<qbVector<double>, 3> poi;
    std::array<double, 3> t;
    bool t1Valid, t2Valid, t3Valid;
    if (numSQRT > 0.0)
    {
        // there was an intersection
        // compute the values for t
        t.at(0) = (-b + numSQRT) / (2 * a);
        t.at(1) = (-b - numSQRT) / (2 * a);

        // compute the points of intersection
        poi.at(0) = bckRay.m_point1 + (v * t[0]);
        poi.at(1) = bckRay.m_point1 + (v * t[1]);

        // check if any of these are valid
        if ((t.at(0) > 0.0) && (poi.at(0).GetElement(2) > 0.0) && (poi.at(0).GetElement(2) < 1.0))
        {
            t1Valid = true;
        }

        else
        {
            t1Valid = false;
            t.at(0) = 100e6;
        }

        if ((t.at(1) > 0.0) && (poi.at(1).GetElement(2) > 0.0) && (poi.at(1).GetElement(2) < 1.0))
        {
            t2Valid = true;
        }

        else
        {
            t2Valid = false;
            t.at(1) = 100e6;
        }
    }

    else
    {
        t1Valid = false;
        t2Valid = false;
        t.at(0) = 100e6;
        t.at(1) = 100e6;
    }

    // and test the end caps
    if (CloseEnough(v.GetElement(2), 0.0))
    {
        t3Valid = false;
        t.at(2) = 100e6;
    }

    else
    {
        // compute the values of t
        t.at(2) = (bckRay.m_point1.GetElement(2) - 1.0) / -v.GetElement(2);

        // compute the points of intersection
        poi.at(2) = bckRay.m_point1 + t.at(2) * v;

        // check if these are valid
        if ((t.at(2) > 0.0) && (sqrt(std::pow(poi.at(2).GetElement(0), 2.0) + std::pow(poi.at(2).GetElement(1), 2.0)) < 1.0))
        {
            t3Valid = true;
        }

        else
        {
            t3Valid = false;
            t.at(2) = 100e6;
        }
    }

    // if no valid intersection found, then we can stop
    if ((!t1Valid) && (!t2Valid) && (!t3Valid))
        return false;

    // check for the smallest valid value of t
    int minIndex = 0;
    double minValue = 10e6;
    for (int i = 0; i < 3; ++i)
    {
        if (t.at(i) < minValue)
        {
            minValue = t.at(i);
            minIndex = i;
        }
    }

    // if minIndex is either 0 or 1 then we have a valid intersection with the cylinder itself
    qbVector<double> validPOI = poi.at(minIndex);
    if (minIndex < 2)
    {
        // Transform the intersection back in world coordinates
        intPoint = m_transformMatrix.Apply(validPOI, cgRT::FWDTFORM);

        // compute the local normal
        qbVector<double> orgNormal{3};
        qbVector<double> newNormal{3};
        qbVector<double> localOrigin{std::vector<double>{0.0, 0.0, 0.0}};
        qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, cgRT::FWDTFORM);

        double tX = validPOI.GetElement(0);
        double tY = validPOI.GetElement(1);
        double tZ = -sqrtf(pow(tX, 2.0) + pow(tY, 2.0));
        
        orgNormal.SetElement(0, tX);
        orgNormal.SetElement(1, tY);
        orgNormal.SetElement(2, tZ);
        // newNormal = m_transformMatrix.Apply(orgNormal, cgRT::FWDTFORM) - globalOrigin;
        // newNormal.Normalize();
        // localNormal = newNormal;

        localNormal = m_transformMatrix.ApplyNorm(orgNormal);
        localNormal.Normalize();

        // return the base color
        localColor = m_baseColor;

        //compute and store the (u,v) coords
        double x = validPOI.GetElement(0);
        double y = validPOI.GetElement(1);
        double z = validPOI.GetElement(2);
        double u = atan2(y, x)/M_PI;
        double v = (z * 2.0) + 1.0;

        m_uvCoords.SetElement(0, u);
        m_uvCoords.SetElement(1, v);

        return true;
    }

    else
    {
        if (!CloseEnough(v.GetElement(2), 0.0))
        {
            if (sqrtf(std::pow(validPOI.GetElement(0), 2.0) + std::pow(validPOI.GetElement(1), 2.0)) < 1.0)
            {
                // transform the matrix bak into world coordinates
                intPoint = m_transformMatrix.Apply(validPOI, cgRT::FWDTFORM);

                // compute the local normal
                qbVector<double> localOrigin{std::vector<double>{0.0, 0.0, 0.0}};
                qbVector<double> normalVector{std::vector<double>{0.0, 0.0, 1.0}};
                localNormal = m_transformMatrix.ApplyNorm(normalVector);
                localNormal.Normalize();

                // qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, cgRT::FWDTFORM);
                // localNormal = m_transformMatrix.Apply(normalVector, cgRT::FWDTFORM) - globalOrigin;
                // localNormal.Normalize();

                // return the base color
                localColor = m_baseColor;

                //compute and store the (u,v) coords
                double x = validPOI.GetElement(0);
                double y = validPOI.GetElement(1);
                double z = validPOI.GetElement(2);
                m_uvCoords.SetElement(0, x);
                m_uvCoords.SetElement(1, y);

                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}