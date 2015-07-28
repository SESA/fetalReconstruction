#ifndef _EBBIMAGERIGIDREGISTRATIONWITHPADDING_H
#define _EBBIMAGERIGIDREGISTRATIONWITHPADDING_H

//#include <ebbrt/Debug.h>
#include <irtkImage.h>
#include <irtkTransformation.h>
#include <irtkGaussianBlurring.h>
#include <irtkGaussianBlurringWithPadding.h>
#include <irtkInterpolateImageFunction.h>
#include <irtkHomogeneousTransformationIterator.h>
#include <irtkUtil.h>
#include <irtkOptimizer.h>
#include <irtkRegistration.h>

#include <irtkResampling.h>
#include <irtkImageRigidRegistration.h>
#include <irtkImageRigidRegistrationWithPadding.h>
#include <irtkImageFunction.h>

#include "EbbResamplingWithPadding.h"
#include "EbbCrossCorrelationSimilarityMetric.h"
//#include "EbbUtil.h"
#include "EbbGradientDescentOptimizer.h"

#define MAX_NO_RESOLUTIONS 10

// Definition of available optimization m
/*typedef enum { DownhillDescent,
               GradientDescent,
               GradientDescentConstrained,
               SteepestGradientDescent,
               ConjugateGradientDescent,
               ClosedForm
             } irtkOptimizationMethod;

// Definition of available similarity measures
typedef enum { JE, CC, MI, NMI, SSD, CR_XY, CR_YX, LC, K, ML, NGD, NGP, NGS } irtkSimilarityMeasure;
*/
//forward declare
class EbbGradientDescentOptimizer;

class EbbImageRigidRegistrationWithPadding
{
    irtkGreyImage *_target = NULL;
    irtkGreyImage *_source = NULL;
    irtkGreyImage *_tmp_target = NULL;
    irtkGreyImage *_tmp_source = NULL;
    irtkGreyImage *tmp_target = NULL;
    irtkGreyImage *tmp_source = NULL;
    
    irtkRigidTransformation *_transformation = NULL;
    
    /// Blurring of target image (in mm)
    double _TargetBlurring[MAX_NO_RESOLUTIONS];
    
    /// Resolution of target image (in mm)
    double _TargetResolution[MAX_NO_RESOLUTIONS][3];
    
    /// Blurring of source image (in mm)
    double _SourceBlurring[MAX_NO_RESOLUTIONS];
    
    /// Resolution of source image (in mm)
    double _SourceResolution[MAX_NO_RESOLUTIONS][3];

    /// Number of step sizes
    int    _NumberOfSteps[MAX_NO_RESOLUTIONS];

    /// Length of steps
    double _LengthOfSteps[MAX_NO_RESOLUTIONS];

    /// Max. number of iterations per step size
    int    _NumberOfIterations[MAX_NO_RESOLUTIONS];

    /// Padding value of target image
    short  _TargetPadding;

    /// Padding value of source image
    short _SourcePadding;
    
    /// Number of levels of multiresolution pyramid
    int    _NumberOfLevels;

    /// Max. number of bins for histogram
    int    _NumberOfBins;

    /// Similarity measure for registration
    irtkSimilarityMeasure  _SimilarityMeasure;

    /// Optimization method for registration
    irtkOptimizationMethod _OptimizationMethod;

    /// Convergence parameter for optimization based on change in similarity.
    double _Epsilon;
    
    /// Convergence parameter for optimization based on change in the transformation.
    double _Delta[MAX_NO_RESOLUTIONS];

    /// Source image domain which can be interpolated fast
    double _source_x1, _source_y1, _source_z1;
    double _source_x2, _source_y2, _source_z2;
  
    EbbCrossCorrelationSimilarityMetric *_metric;

    irtkInterpolateImageFunction *_interpolator;

    EbbGradientDescentOptimizer *_optimizer;
    
public:
    double Evaluate();

    // Initial set up for the registration
    void Initialize();
    // Initial set up for the registration at a multiresolution level
    void Initialize(int);

    void SetOutput(irtkRigidTransformation *);
    void SetInput(irtkGreyImage *, irtkGreyImage *);
    void GuessParameterThickSlices();
    void SetTargetPadding(short t) { _TargetPadding = t; };
    double EvaluateGradient(float, float *);
    void Run();

    void Finalize();
    void Finalize(int);

    virtual const char *NameOfClass();
};


inline const char *EbbImageRigidRegistrationWithPadding::NameOfClass()
{
    return "EbbImageRigidRegistrationWithPadding";
}

#endif
