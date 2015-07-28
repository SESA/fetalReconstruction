#ifndef _EBBGAUSSIANBLURRINGWITHPADDING_H
#define _EBBGAUSSIANBLURRINGWITHPADDING_H

#include <irtkImage.h>
#include <irtkScalarFunctionToImage.h>
#include <irtkConvolution.h>
#include <irtkImageToImage.h>

//#include <ebbrt/Debug.h>

template <class VoxelType> class EbbGaussianBlurringWithPadding
{
protected:
    /// Padding value
    VoxelType _PaddingValue;

    //Sigma (standard deviation of Gaussian kernel)
    double _Sigma;

    /// Buffer
    irtkGenericImage<VoxelType> *_tmp;
    
    /// Input image for filter
    irtkGenericImage<VoxelType> *_input;
    
    /// Output image for filter
    irtkGenericImage<VoxelType> *_output;
    
public:
    /// Constructor
    EbbGaussianBlurringWithPadding(double, VoxelType);
    
    /// Run Gaussian blurring
    void Run();

    /// Set input image for filter
    void SetInput (irtkGenericImage<VoxelType> *);
    
    /// Set output image for filter
    void SetOutput(irtkGenericImage<VoxelType> *);

    /// Returns whether the filter requires buffering
    bool RequiresBuffering();
    
    //Initialize filter
    void Initialize();
};

#endif
