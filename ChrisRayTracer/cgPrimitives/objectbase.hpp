#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include <memory>
#include "../qbLinAlg/qbVector.h"
#include "../ray.hpp"
#include "../gtfm.hpp"

namespace cgRT{
    /*Forward declare the material base class this will be overridden later*/
    class MaterialBase;
    class ObjectBase{
        public:
            //constructor and destructor
            ObjectBase();
            virtual ~ObjectBase();

            // func to test for intersections
            virtual bool TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor);

            //func to set the transform matrix
            void SetTransformMatrix(const cgRT::GTform &transformMatrix);

            //func to test whether two floating point numbers are close to being equal
            bool CloseEnough(const double f1, const double f2);

            //func to assign materials
            bool AssignMaterial(const std::shared_ptr<cgRT::MaterialBase> &objectMaterial);

            //public member variable
        public:
            //base color of object
            qbVector<double> m_baseColor {3};

            //store the geometric transform applied to the object
            cgRT::GTform m_transformMatrix;

            //a reference to the material assigned to this object
            std::shared_ptr<cgRT::MaterialBase> m_pMaterial;

            // A flag to indicate whether this object is visible.
            bool m_isVisible = true;

            //A flag to indicated whther this obj has a material or not
            bool m_hasMaterial = false;

            // store the (u,v) coordinates from a detected intersection
            qbVector<double> m_uvCoords {2};
    };
}

#endif