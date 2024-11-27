#ifndef TEXTUREBASE_H
#define TEXTUREBASE_H
#include <memory>
#include "../qbLinAlg/qbMatrix.h"
#include "../qbLinAlg/qbVector.h"
#include "../ray.hpp"

namespace cgRT{
    namespace Texture{
        class TextureBase{
            public:
                // constructor / destructor
                TextureBase();
                virtual ~TextureBase();

                // func to return the color at a given point in the uv coordinate system
                // note that the color is returned as a 4-dimensional vector (RGBA)
                virtual qbVector<double> GetColor(const qbVector<double> &uvCoords);

                // func to set the transform
                void setTransform(const qbVector<double> &translation, const double &rotation, const qbVector<double> &scale);

                // func to blend RGBA colors, returning a 3-dimensional (RGB) results
                static qbVector<double> BlendColors(const std::vector<qbVector<double>> &inputColorList);

                //func to apply the local transform to the given input vector
                qbVector<double> ApplyTransform(const qbVector<double> &inputVector);

            private:
            
            private:
                //initialise the transform matrix to the identity matrix
                qbMatrix2<double> m_transformMatrix {3, 3, std::vector<double>{1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}};
        };
    }
}

#endif