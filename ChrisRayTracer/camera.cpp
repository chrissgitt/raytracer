#include "camera.hpp"
#include "ray.hpp"
#include <math.h>

cgRT::Camera::Camera(){
    m_cameraPosition = qbVector<double> {std::vector<double>{0.0,-10.0,0.0}};
    m_cameraLookAt = qbVector<double> {std::vector<double>{0.0,0.0,0.0}};
    m_cameraUp = qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}};
    m_cameraLength = 1.0;
    m_cameraHorzSize = 1.0;
    m_cameraAspectRatio = 1.0;
}

void cgRT::Camera::SetPosition(const qbVector<double> &newPosition){
    m_cameraPosition = newPosition;
}

void cgRT::Camera::SetLookAt(const qbVector<double> &newLookAt){
    m_cameraLookAt = newLookAt;
}

void cgRT::Camera::SetUp(const qbVector<double> &upVector){
    m_cameraUp = upVector;
}

void cgRT::Camera::SetLength(double newLength){
    m_cameraLength = newLength;
}

void cgRT::Camera::SetHorzSize(double newHorzSize){
    m_cameraHorzSize = newHorzSize;
}

void cgRT::Camera::SetAspect(double newAspect){
    m_cameraAspectRatio = newAspect;
}

// func to return the position of the camera

qbVector<double> cgRT::Camera::GetPosition(){
    return m_cameraPosition;
}

qbVector<double> cgRT::Camera::GetLookAt(){
    return m_cameraLookAt;
}

qbVector<double> cgRT::Camera::GetUp(){
    return m_cameraUp;
}

double cgRT::Camera::GetLength(){
    return m_cameraLength;
}

double cgRT::Camera::GetHorzSize(){
    return m_cameraHorzSize;
}

double cgRT::Camera::GetAspect(){
    return m_cameraAspectRatio;
}

qbVector<double> cgRT::Camera::GetU(){
    return m_projectionScreenU;
}

qbVector<double> cgRT::Camera::GetV(){
    return m_projectionScreenV;
}

qbVector<double> cgRT::Camera::GetScreenCentre(){
    return m_projectionScreenCentre;
}

//func to compute camera geometry

void cgRT::Camera::UpdateCameraGeometry(){
    //first compute the vector from the camera position to the lookat postion
    m_alignmentVector = m_cameraLookAt - m_cameraPosition;
    m_alignmentVector.Normalize();

    //second compute u and v vectors
    m_projectionScreenU = qbVector<double>::cross(m_alignmentVector,m_cameraUp);
    m_projectionScreenU.Normalize();
    m_projectionScreenV = qbVector<double>::cross(m_projectionScreenU, m_alignmentVector);
    m_projectionScreenV.Normalize();

    //third compute the position of the centre point of the screen
    m_projectionScreenCentre = m_cameraPosition + (m_cameraLength * m_alignmentVector);

    //modify the u and v vectors to match the size and aspect ratio
    m_projectionScreenU = m_projectionScreenU * m_cameraHorzSize;
    m_projectionScreenV = m_projectionScreenV * (m_cameraHorzSize/m_cameraAspectRatio);
}

// func to retunr a ray
bool cgRT::Camera::GenerateRay(float proScreenX, float proScreenY, cgRT::Ray &cameraRay){
    // compute the loc of the screen ppoint in world coords
    qbVector<double> screenWorldPart1 = m_projectionScreenCentre + (m_projectionScreenU * proScreenX);
    qbVector<double> screenWorldCoordinate = screenWorldPart1 + (m_projectionScreenV * proScreenY);

    //use this point along with camera position to calc ray

    cameraRay.m_point1 = m_cameraPosition;
    cameraRay.m_point2 = screenWorldCoordinate;
    cameraRay.m_lab = screenWorldCoordinate - m_cameraPosition;

    return true;
}
