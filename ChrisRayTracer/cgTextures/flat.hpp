#ifndef FLAT_H
#define FLAT_H
#include "texturebase.hpp"

namespace cgRT{
    namespace Texture{
        class Flat : public TextureBase{
            public:
                //constructor / destructor
                Flat();
                virtual ~Flat() override;
            
                //func to return the color
                virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;

                //func to set the color
                void SetColor(const qbVector<double> &inputColor);

            private:
                qbVector<double> m_color {4};
        };
    }
}

#endif