#include "lightbase.hpp"

//comstructor
cgRT::LightBase::LightBase(){}

//destructor
cgRT::LightBase::~LightBase(){}

//func to compute illumination
bool cgRT::LightBase::ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                          const std::vector<std::shared_ptr<cgRT::ObjectBase>> &objectList,
                                          const std::shared_ptr<cgRT::ObjectBase> &currentObject,
                                          qbVector<double> &color, double &intensity){
return false;
}