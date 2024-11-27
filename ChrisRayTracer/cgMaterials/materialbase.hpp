#ifndef MATERIALBASE_H
#define MATERIALBASE_H

#include <memory>
#include "../cgPrimitives/objectbase.hpp"
#include "../cgTextures/texturebase.hpp"
#include "../cgLights/lightbase.hpp"
#include "../qbLinAlg/qbVector.h"
#include "../ray.hpp"

namespace cgRT{
    class MaterialBase{
        public:
            //constructor and destructor
            MaterialBase();
            virtual ~MaterialBase();

            //func to return the color of the material
            virtual qbVector<double> ComputeColor(  const std::vector<std::shared_ptr<cgRT::ObjectBase>>&objectList,
                                                    const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                    const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                    const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                    const cgRT::Ray &cameraRay);
            //func to compute diffuse color
            static qbVector<double> ComputeDiffuseColor(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                        const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                        const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                        const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                        const qbVector<double> &baseColor);

            //func to compute the refelction color
            qbVector<double> ComputeReflectionColor(const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                                    const std::vector<std::shared_ptr<cgRT::LightBase>> &lightList,
                                                    const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                                    const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                    const cgRT::Ray &incidentRay);

            // func to cast a ray into a scene
            bool CastRay(const cgRT::Ray &castRay, const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                             const std::shared_ptr<cgRT::ObjectBase> &thisObject, std::shared_ptr<cgRT::ObjectBase> &closestObject,
                             qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal, qbVector<double> &closestLocalColor);

            //func to assign a texture
            void AssignTexture(const std::shared_ptr<cgRT::Texture::TextureBase> &inputTexture);

        public:
            //counter for the number of reflection rays
            inline static int m_maxReflectionRays;
            inline static int m_reflectionRayCount;

            // the ambient lighting conditions
            inline static qbVector<double> m_ambientColor {std::vector<double>{1.0, 1.0, 1.0}};
            inline static double m_ambientIntensity = 0.2;


            //list of textures assigned to this material
            std::vector<std::shared_ptr<cgRT::Texture::TextureBase>> m_textureList;

            //flag to indicate whether at least one texture has been assgined
            bool m_hasTexture = false;

        private:

    };
}

#endif