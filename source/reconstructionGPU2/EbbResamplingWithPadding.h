#ifndef _EBBRESAMPLINGWITHPADDING_H
#define _EBBRESAMPLINGWITHPADDING_H

#include <irtkImage.h>
#include <irtkImageToImage.h>

template <class VoxelType> class EbbResamplingWithPadding : public irtkImageToImage<VoxelType>
{
protected:
    /// Padding value
    VoxelType _PaddingValue;
    double _XSize;
    double _YSize;
    double _ZSize;
    
    /// Returns whether the filter requires buffering
    virtual bool RequiresBuffering();

    virtual const char *NameOfClass();
    
    /// Initialize the filter
    virtual void Initialize();
    
public:
    
  /// Constructor
  EbbResamplingWithPadding(double, double, double, VoxelType);

  /// Run the resampling filter
  virtual void Run();

  /// Set padding value
  SetMacro(PaddingValue, VoxelType);

  /// Get padding value
  GetMacro(PaddingValue, VoxelType);
  
};

#endif
