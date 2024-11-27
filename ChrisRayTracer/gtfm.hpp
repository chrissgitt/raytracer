#ifndef GTFM_H
#define GTFM_H

#include "./qbLinAlg/qbVector.h"
#include "./qbLinAlg/qbMatrix.h"
#include "ray.hpp"

namespace cgRT{
    //define direction flag values
    constexpr bool FWDTFORM = true;
    constexpr bool BCKTFORM = false;

    class GTform{
        public:
            //constructor and destructor
            GTform();
            ~GTform();

            //construct from 3 vectors
            GTform(const qbVector<double> &translation,
                   const qbVector<double> &rotation,
                   const qbVector<double> &scale);

            // construct pair of matricies
            GTform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck);

            //func to set translation rotation and scaling components
            void SetTransform(  const qbVector<double> &translation,
                                const qbVector<double> &rotation,
                                const qbVector<double> &scale);

            void SetTransform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck);

            // func to return the transform matricies
            qbMatrix2<double> GetForward();
            qbMatrix2<double> GetBackward();

            //func to apply the transform
            cgRT::Ray Apply(const cgRT::Ray &inputRay, bool dirFlag);
            qbVector<double> Apply(const qbVector<double> &inputVector, bool dirFlag);
            qbVector<double> ApplyNorm(const qbVector<double> &inputVector);

            //function to return the normal transformation matrix
            qbMatrix2<double> GetNormalTransform();

            //overload operators
            friend cgRT::GTform operator* (const cgRT::GTform &lhs, const cgRT::GTform &rhs);

            // overload the assignment operator
            GTform operator = (const GTform &rhs);

            //func to print transform matrix to STDOUT
            void PrintMatrix(bool dirFlag);

            //func to allow printing of vectors
            static void PrintVector(const qbVector<double> &vector);

        private:
            void Print(const qbMatrix2<double> &matrix);
            void ExtractLinearTransform();
        
        private:
            qbMatrix2<double> m_fwdtfm {4, 4};
            qbMatrix2<double> m_bcktfm {4, 4};
            qbMatrix2<double> m_lintfm {3, 3};
    };

}

#endif