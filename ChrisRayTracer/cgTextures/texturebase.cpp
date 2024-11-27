#include "texturebase.hpp"
#include <cmath>

//constructor / destructor

cgRT::Texture::TextureBase::TextureBase(){}
cgRT::Texture::TextureBase::~TextureBase(){}


//func to return the color at a given {u,v} location
qbVector<double> cgRT::Texture::TextureBase::GetColor(const qbVector<double> &uvCoords){
    //setup the output vector
    qbVector<double> outputColor {4};

    //return the output
return outputColor;
}

// func to set the transform matrix

void cgRT::Texture::TextureBase::setTransform(const qbVector<double> &translation, const double &rotation, const qbVector<double> &scale){
    //build the transform matrix
    qbMatrix2<double> rotationMatrix = {3, 3, std::vector<double>{
        cos(rotation), -sin(rotation), 0.0,
        sin(rotation), cos(rotation), 0.0,
        0.0, 0.0, 1.0
    }};

    qbMatrix2<double> scaleMatrix = {3, 3, std::vector<double>{
        scale.GetElement(0), 0.0, 0.0,
        0.0, scale.GetElement(1), 0.0,
        0.0, 0.0, 1.0
    }};

    qbMatrix2<double> translationMatrix = {3, 3, std::vector<double>{
        1.0, 0.0, translation.GetElement(0),
        0.0, 1.0, translation.GetElement(1),
        0.0, 0.0, 1.0
    }};

    // combine to form the final transformation matrix
    m_transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

// func to blend colors
qbVector<double> cgRT::Texture::TextureBase::BlendColors(const std::vector<qbVector<double>> &inputColorList){
    // setup the ouput color
    qbVector<double> outputColor {3};

    //return the output
return outputColor;
}

// func to apply the transform
qbVector<double>cgRT::Texture::TextureBase::ApplyTransform(const qbVector<double> &inputVector){
    //copy the input vector and modify to have three elements
    qbVector<double> newInput {3};
    newInput.SetElement(0, inputVector.GetElement(0));
    newInput.SetElement(1, inputVector.GetElement(1));

    //apply the transform
    qbVector<double> result = m_transformMatrix * newInput;

    //produce the output
    qbVector<double> output {2};
    output.SetElement(0, result.GetElement(0));
    output.SetElement(1, result.GetElement(1));

return output;
}