#include "checker.hpp"

cgRT::Texture::Checker::Checker(){
    m_color1 = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
    m_color2 = qbVector<double>{std::vector<double>{0.2, 0.2, 0.2}};
}

cgRT::Texture::Checker::~Checker(){}

// func to return the color
qbVector<double> cgRT::Texture::Checker::GetColor(const qbVector<double> &uvCoords){
    // apply the local transform to the (u,v) coords
    qbVector<double> inputLoc = uvCoords;
    qbVector<double> newLoc = ApplyTransform(inputLoc);
    double newU = newLoc.GetElement(0);
    double newV = newLoc.GetElement(1);

    qbVector<double> localColor {4};
    int check = static_cast<int>(floor(newU)) + static_cast<int>(floor(newV));

    (check%2 == 0)?localColor = m_color1:localColor = m_color2;

return localColor;
}

void cgRT::Texture::Checker::SetColor(const qbVector<double> &inputColor1, const qbVector<double> &inputColor2){
    m_color1 = inputColor1;
    m_color2 = inputColor2;
}