#include "flat.hpp"

cgRT::Texture::Flat::Flat(){
    m_color = qbVector<double>{std::vector<double>{1.0, 0.0, 0.0, 1.0}};
}

cgRT::Texture::Flat::~Flat(){}

//func to return the color
qbVector<double> cgRT::Texture::Flat::GetColor(const qbVector<double> &uvCoords){
    return m_color;
}

//func to accept color
void cgRT::Texture::Flat::SetColor(const qbVector<double> &inputColor){
    m_color = inputColor;
}