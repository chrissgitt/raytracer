#ifndef LIGHTBASE_H
#define LIGHTBASE_H

#include <memory>
#include "../qbLinAlg/qbVector.h"
#include "../ray.hpp"
#include "../cgPrimitives/objectbase.hpp"

namespace cgRT{
    class LightBase{
        public:
            //constructor and destructor
            LightBase();
            virtual ~LightBase();

            //func to compute illumination
            virtual bool ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
            const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
            const std::shared_ptr<cgRT::ObjectBase> &currentObject,
            qbVector<double> &color, double &intensity);

        public:
            qbVector<double> m_color {3};
            qbVector<double> m_location {3};
            double m_intensity;
    };
}

#endif