#ifndef CHECKER_H
#define CHECKER_H
#include "texturebase.hpp"

namespace cgRT{
    namespace Texture{
        class Checker : public TextureBase{
            public:
                // constructor / destructor
                Checker();
                virtual ~Checker() override;

                // function to return the color
                virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;

                // func to set color
                void SetColor(const qbVector<double> &inputColor1, const qbVector<double> &inputColor2);

            private:
                qbVector<double> m_color1 {4};
                qbVector<double> m_color2 {4};
        };
    }
}


#endif