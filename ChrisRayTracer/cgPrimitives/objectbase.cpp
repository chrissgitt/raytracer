#include "objectbase.hpp"
#include <math.h>

#define EPSILON 1e-21f;

//constructor
cgRT::ObjectBase::ObjectBase(){}

//destructor
cgRT::ObjectBase::~ObjectBase(){}

//func to test intersections
bool cgRT::ObjectBase::TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor){
    return false;
}

void cgRT::ObjectBase::SetTransformMatrix(const cgRT::GTform &transformMatrix){
    m_transformMatrix = transformMatrix;
}

//func to asssign a material
bool cgRT::ObjectBase::AssignMaterial(const std::shared_ptr<cgRT::MaterialBase> &objectMaterial){
    m_pMaterial = objectMaterial;
    m_hasMaterial = true;
return m_hasMaterial;
}

//func to test whether two floating point numbers are close to being equal
bool cgRT::ObjectBase::CloseEnough(const double f1, const double f2){
    return fabs(f1-f2) < EPSILON;
}