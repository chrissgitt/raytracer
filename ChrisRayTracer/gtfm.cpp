#include "gtfm.hpp"

//constructor + destructor

cgRT::GTform::GTform(){
    //set fwd and bckwrd. transforms to identity matricies
    m_fwdtfm.SetToIdentity();
    m_bcktfm.SetToIdentity();
    ExtractLinearTransform();
}

cgRT::GTform::~GTform(){}

cgRT::GTform::GTform(const qbVector<double> &translation, const qbVector<double> &rotation, const qbVector<double> &scale){
    SetTransform(translation, rotation, scale);
    ExtractLinearTransform();
}

//construct from a pair of matricies
cgRT::GTform::GTform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck){
    //verify that inputs are 4x4
    if((fwd.GetNumRows()!=4)||(fwd.GetNumCols()!=4)||(bck.GetNumRows()!=4)||(bck.GetNumCols()!=4)){
        throw std::invalid_argument("Cannot construct GTForm, inputs are not 4x4");
    }

    m_fwdtfm = fwd;
    m_bcktfm = bck;
    ExtractLinearTransform();
}

//func to set the transform
void cgRT::GTform::SetTransform(const qbVector<double> &translation,
                                const qbVector<double> &rotation,
                                const qbVector<double> &scale){
    //define a matrix for each component of the transform
    qbMatrix2<double> translationMatrix {4, 4};
    qbMatrix2<double> rotationMatrixX {4, 4};
    qbMatrix2<double> rotationMatrixZ {4, 4};
    qbMatrix2<double> rotationMatrixY {4, 4};
    qbMatrix2<double> scaleMatrix {4, 4};

    //set these to identity
    translationMatrix.SetToIdentity();
    rotationMatrixX.SetToIdentity();
    rotationMatrixY.SetToIdentity();
    rotationMatrixZ.SetToIdentity();
    scaleMatrix.SetToIdentity();

    //populate these with appropriate values
    //first the translation matrix
    translationMatrix.SetElement(0, 3, translation.GetElement(0));
    translationMatrix.SetElement(1, 3, translation.GetElement(1));
    translationMatrix.SetElement(2, 3, translation.GetElement(2));

    //rotation matricies
    rotationMatrixZ.SetElement(0, 0, cos(rotation.GetElement(2)));
    rotationMatrixZ.SetElement(0, 1, -sin(rotation.GetElement(2)));
    rotationMatrixZ.SetElement(1, 0, sin(rotation.GetElement(2)));
    rotationMatrixZ.SetElement(1, 1, cos(rotation.GetElement(2)));

    rotationMatrixY.SetElement(0, 0, cos(rotation.GetElement(1)));
    rotationMatrixY.SetElement(0, 2, sin(rotation.GetElement(1)));
    rotationMatrixY.SetElement(2, 0, -sin(rotation.GetElement(1)));
    rotationMatrixY.SetElement(2, 2, cos(rotation.GetElement(1)));

    rotationMatrixX.SetElement(1, 1, cos(rotation.GetElement(0)));
    rotationMatrixX.SetElement(1, 2, -sin(rotation.GetElement(0)));
    rotationMatrixX.SetElement(2, 1, sin(rotation.GetElement(0)));
    rotationMatrixX.SetElement(2, 2, cos(rotation.GetElement(0)));

    //scale matrix
    scaleMatrix.SetElement(0, 0, scale.GetElement(0));
    scaleMatrix.SetElement(1, 1, scale.GetElement(1));
    scaleMatrix.SetElement(2, 2, scale.GetElement(2));

    //combine to give the final forward transform matrix
    m_fwdtfm = translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;

    //compute backwards transform
    m_bcktfm = m_fwdtfm;
    m_bcktfm.Inverse();
}

void cgRT::GTform::SetTransform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck){
    m_fwdtfm = fwd;
    m_bcktfm = bck;
    ExtractLinearTransform();
}

//functions to return the transform matricies
qbMatrix2<double> cgRT::GTform::GetForward() { return m_fwdtfm; }
qbMatrix2<double> cgRT::GTform::GetBackward() { return m_bcktfm; }

//func to apply the transform
cgRT::Ray cgRT::GTform::Apply(const cgRT::Ray &inputRay, bool dirFlag){
    //create an output object.
    cgRT::Ray outputRay;

    if(dirFlag){
        //Aplly the forward transform
        outputRay.m_point1 = this -> Apply(inputRay.m_point1, cgRT::FWDTFORM);
        outputRay.m_point2 = this -> Apply(inputRay.m_point2, cgRT::FWDTFORM);
        outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
    }

    else{
        outputRay.m_point1 = this -> Apply(inputRay.m_point1, cgRT::BCKTFORM);
        outputRay.m_point2 = this -> Apply(inputRay.m_point2, cgRT::BCKTFORM);
        outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
    }
return outputRay;
}

qbVector<double> cgRT::GTform::Apply(const qbVector<double> &inputVector, bool dirFlag){
    //convert inputVector to a 4-element vector
    std::vector<double> tempData {  inputVector.GetElement(0), 
                                    inputVector.GetElement(1), 
                                    inputVector.GetElement(2), 
                                    1.0};
    qbVector<double> tempVector {tempData};

    //create a vector for the result
    qbVector<double> resultVector;

    //apply the forward or backward transform depending on the flag value
    dirFlag?(resultVector = m_fwdtfm * tempVector):(resultVector = m_bcktfm * tempVector);

    //reform the output as a 3-element vector
    qbVector<double> outputVector{std::vector<double> { resultVector.GetElement(0),
                                                        resultVector.GetElement(1),
                                                        resultVector.GetElement(2)}};
    return outputVector;
}

qbVector<double> cgRT::GTform::ApplyNorm(const qbVector<double> &inputVector){
    //apply the transform and return the result
    qbVector<double> result = m_lintfm * inputVector;
    return result;
}

//overload operators
namespace cgRT{
    cgRT::GTform operator * (const cgRT::GTform &lhs, const cgRT::GTform &rhs){
        //form the product of the two forward transforms
        qbMatrix2<double> fwdResult = lhs.m_fwdtfm * rhs.m_fwdtfm;

        //compute the backward transform as the inverse of forward transform
        qbMatrix2<double> bckResult = fwdResult;
        bckResult.Inverse();

        //form the  final result
        cgRT::GTform finalResult (fwdResult, bckResult);

        return finalResult;
    }
}

//overload assignment operator
cgRT::GTform cgRT::GTform::operator=(const cgRT::GTform &rhs){
    //make sure that we are not assigning to ourself
    if(this != &rhs){
        m_fwdtfm = rhs.m_fwdtfm;
        m_bcktfm = rhs.m_bcktfm;
    }
return *this;
}

//function to print the transform matris to STDOUT
void cgRT::GTform::PrintMatrix(bool dirFlag){
    dirFlag?Print(m_fwdtfm):Print(m_bcktfm);
}

void cgRT::GTform::Print(const qbMatrix2<double> &matrix){
    int nRows = matrix.GetNumRows();
    int nCols = matrix.GetNumCols();

    for(int row = 0; row < nRows; ++row){
        for(int col = 0; col < nCols; ++col){
            std::cout << std::fixed << std::setprecision(3) << matrix.GetElement(row, col) << " ";
        }
    std::cout << std::endl;
    }
}

//func to print vectors
void cgRT::GTform::PrintVector(const qbVector<double> &inputVector){
    int nRows = inputVector.GetNumDims();
    for(int row = 0; row < nRows; ++row){
        std::cout << std::fixed << std::setprecision(3) << inputVector.GetElement(row) << std::endl;
    }
}

void cgRT::GTform::ExtractLinearTransform(){
    //copy the linear portion of the transform
    for(int i=0; i<3; ++i){
        for(int j=0; j<3; ++j){
            m_lintfm.SetElement(i, j, m_fwdtfm.GetElement(i, j));
        }
    }

    //invert and transpose
    m_lintfm.Inverse();
    m_lintfm = m_lintfm.Transpose();
}

qbMatrix2<double> cgRT::GTform::GetNormalTransform(){
    return m_lintfm;
}