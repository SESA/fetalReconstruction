#ifndef _EBBGRADIENTDESCENTOPTIMIZER_H
#define _EBBGRADIENTDESCENTOPTIMIZER_H

#include "EbbImageRigidRegistrationWithPadding.h"

//forward declare
class EbbImageRigidRegistrationWithPadding;

class EbbGradientDescentOptimizer
{    
protected:
    irtkRigidTransformation *_Transformation = NULL;
    EbbImageRigidRegistrationWithPadding *_Registration = NULL;
    
    double _StepSize = 0.1;
    double _Epsilon = 0.001;

    /// Storage for monitoring change in the transformation.
    double *transformationParams;
public:
    double Run();
    void Run(double &, double &);
    const char *NameOfClass();
    void SetTransformation(irtkRigidTransformation *);
    void SetRegistration(EbbImageRigidRegistrationWithPadding *);
    void SetStepSize(double s);
    void SetEpsilon(double e);
};

inline const char *EbbGradientDescentOptimizer::NameOfClass()
{
  return "EBBGradientDescentOptimizer";
}

inline void EbbGradientDescentOptimizer::SetTransformation(irtkRigidTransformation *transformation)
{
    int n;
    _Transformation = transformation;
    n = _Transformation->NumberOfDOFs();
    transformationParams = new double[n];
}

inline void EbbGradientDescentOptimizer::SetRegistration(EbbImageRigidRegistrationWithPadding *registration)
{
    _Registration = registration;
}

inline void EbbGradientDescentOptimizer::SetStepSize(double s)
{
    _StepSize = s;
}

inline void EbbGradientDescentOptimizer::SetEpsilon(double e)
{
    _Epsilon = e;
}

#endif
