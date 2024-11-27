#ifndef CYLINDER_H
#define CYLINDER_H

#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace cgRT{
    class Cylinder : public ObjectBase{
        public:
            // defualt constructor
            Cylinder();
            //override the destructor
            virtual ~Cylinder() override;

            //override the funtion to test for intersection
            virtual bool TestIntersection(const cgRT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;
    };
}

#endif