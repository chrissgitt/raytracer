#ifndef RAYMARCHBASE_H
#define RAYMARCHBASE_H

#include <functional>
#include "sdfunc.hpp"
#include "../cgPrimitives/objectbase.hpp"
#include "../cgPrimitives/box.hpp"

namespace cgRT{
    namespace RM{
        class RayMarchBase : public ObjectBase{
            public:
                // default constructor
                RayMarchBase();

                // destructor
                virtual ~RayMarchBase() override;

                // override the the func to test for intersections
                virtual bool TestIntersection(const cgRT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;

                // func to set the object function
                void SetObjectFcn(std::function<double(qbVector<double>*, qbVector<double>*)> objectFcn);

                // func to evaluate the Signed Distance Function (SDF) at a given location
                double EvaluateSDF(qbVector<double> *location, qbVector<double> *parms);

            public:
                // bounding box
                cgRT::Box m_boundingBox = cgRT::Box();

                // parameters
                qbVector<double> m_parms {3};

            private:
                // pointer to object function
                std::function<double(qbVector<double>*, qbVector<double>*)> m_objectFcn;

                bool m_haveObjectFcn = false;
                double m_epsilon;
                int m_maxSteps;

                const double m_h = 0.01;
                qbVector<double> m_xDisp {std::vector<double>{m_h, 0.0, 0.0}};
                qbVector<double> m_yDisp{std::vector<double>{0.0, m_h, 0.0}};
                qbVector<double> m_xDisp{std::vector<double>{0.0, 0.0, m_h}};
        };
    }
}

#endif