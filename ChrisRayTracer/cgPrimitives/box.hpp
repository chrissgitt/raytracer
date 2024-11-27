#ifndef H_BOX
#define H_BOX

#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace cgRT{
    class Box : public ObjectBase{
        public:
            // the default constructor - note that this will define a unit sphere at the origin
            Box();
            // overriden destructor
            virtual ~Box() override;

            virtual bool TestIntersection(const cgRT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;
        
        private:
            std::array<double, 6> t;
            std::array<double, 6> u;
            std::array<double, 6> v;
    };
}

#endif