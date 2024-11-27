#ifndef OBJECTSPHERE_H
#define OBJECTSPHERE_H

#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace cgRT{
    class ObjSphere : public ObjectBase{
        public:
            // The default constructor
            // N.B. this will define a unit sphere at the origin
            ObjSphere();
            // override the destructor
            virtual ~ObjSphere() override;

            //override the fun to test for intersections
            virtual bool TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;

        private:

    };
}

#endif