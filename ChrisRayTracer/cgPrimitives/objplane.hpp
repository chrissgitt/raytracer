#ifndef OBJPLANE_H
#define OBJPLANE_H

#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace cgRT{
    class ObjPlane : public ObjectBase{
        public:
            ObjPlane();

            // override the destructor
            virtual ~ObjPlane() override;

            // override the func to test for intersections
            virtual bool TestIntersection(const cgRT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;
        
        private:
        
    };
}

#endif