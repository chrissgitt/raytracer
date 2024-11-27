#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "lightbase.hpp"

namespace cgRT{
    class PointLight : public LightBase{
        public:
            // defualt constructor
            PointLight();
            
            //override defualt destructor
            virtual ~PointLight() override;

            //func to compute illumination
            virtual bool ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                             const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                             const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                             qbVector<double> &color, double &intensity) override;
    };
}

#endif