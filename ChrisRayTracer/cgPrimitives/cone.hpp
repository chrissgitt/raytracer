#ifndef CONE_H
#define CONE_H

#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace cgRT{
    class Cone : public ObjectBase{
        public:
            //default constructor
            Cone();

            //Override the destructor
            virtual ~Cone() override;

            virtual bool TestIntersection(const cgRT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;
    };
}

#endif