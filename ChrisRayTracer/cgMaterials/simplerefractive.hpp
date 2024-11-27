#ifndef SIMPLEREFRACTIVE_H
#define SIMPLEREFRACTIVE_H

#include "materialbase.hpp"

namespace cgRT{
    class SimpleRefractive : public MaterialBase{
        public:
        //constructor / destructor
            SimpleRefractive();
            virtual ~SimpleRefractive() override;

        //func to return the color
            virtual qbVector<double> ComputeColor(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                  const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                  const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                  const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                  const cgRT::Ray &cameraRay) override;

            // func to compute specular highlights
            qbVector<double> ComputeSpecular(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                             const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                             const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                             const cgRT::Ray &cameraRay);

            // func to compute translucency
            qbVector<double> ComputeTranslucency(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                 const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                 const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                 const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                 const cgRT::Ray &incidentRay);

        public:
            qbVector<double> m_baseColor {std::vector<double> {1.0,0.0,1.0}};
            double m_reflectivity = 0.0;
            double m_shininess = 0.0;
            double m_translucency = 0.0;
            double m_ior = 1.0;
    };
}

#endif