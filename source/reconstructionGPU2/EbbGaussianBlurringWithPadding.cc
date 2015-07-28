#include "EbbGaussianBlurringWithPadding.h"

template <class VoxelType> EbbGaussianBlurringWithPadding<VoxelType>::EbbGaussianBlurringWithPadding(double Sigma, VoxelType PaddingValue)
{
    _Sigma = Sigma;
    _PaddingValue = PaddingValue;
}

template <class VoxelType> void EbbGaussianBlurringWithPadding<VoxelType>::SetInput (irtkGenericImage<VoxelType> *image)
{
    if (image != NULL)
    {
	_input = image;
    } else {
	cout << "EbbGaussianBlurringWithPadding::SetInput: Input is not an image\n" << endl;
	exit(1);
    }
}

template <class VoxelType> void EbbGaussianBlurringWithPadding<VoxelType>::SetOutput (irtkGenericImage<VoxelType> *image)
{
    if (image != NULL)
    {
	_output = image;
    } else {
	cout << "EbbGaussianBlurringWithPadding::SetOutput: Output is not an image\n" << endl;
	exit(1);
    }
}

template <class VoxelType> bool EbbGaussianBlurringWithPadding<VoxelType>::RequiresBuffering(void)
{
  return false;
}

template <class VoxelType> void EbbGaussianBlurringWithPadding<VoxelType>::Initialize()
{
    // Check inputs and outputs
    if (_input == NULL) {
	
	cout << "EbbGaussianBlurringWithPadding::Run: Filter has no input" << endl;
	exit(1);
    }

    if (_output == NULL) {
	cout << "EbbGaussianBlurringWithPadding::Run: Filter has no output" << endl;
	exit(1);
    }

    if (_input->IsEmpty() == true) {
	cout << "EbbGaussianBlurringWithPadding::Run: Input is empty" << endl;
	exit(1);
    }

    // Check whether filter requires buffering
    //automatically false
/*    if (this->RequiresBuffering()) 
    {
	this->Debug("irtkImageToImage::Initialize: Filter requires buffering");

	// Check whether filter has external buffer
	if (_input == _output) {
	    this->Debug("irtkImageToImage::Initialize: Filter has internal buffer");
	    _tmp    = _output;
	    _output = new irtkGenericImage<VoxelType>;
	} else {
	    this->Debug("irtkImageToImage::Initialize: Filter has external buffer");
	    _tmp    = NULL;
	}
    } else {
	this->Debug("irtkImageToImage::Initialize: Filter requires no buffering");
    }
*/
    
    // Make sure that output has the correct dimensions
    if (_input != _output) _output->Initialize(_input->GetImageAttributes());
}

template <class VoxelType> void EbbGaussianBlurringWithPadding<VoxelType>::Run()
{
  double xsize, ysize, zsize;

  // Do the initial set up
  this->Initialize();

  // Get voxel dimensions
  this->_input->GetPixelSize(&xsize, &ysize, &zsize);

  // Create scalar function which corresponds to a 1D Gaussian function in X
  irtkScalarGaussian gaussianX(this->_Sigma/xsize, 1, 1, 0, 0, 0);

  // Create filter kernel for 1D Gaussian function in X
  irtkGenericImage<irtkRealPixel> kernelX(2*round(4*this->_Sigma/xsize)+1, 1, 1);

  // Do conversion from  scalar function to filter kernel
  irtkScalarFunctionToImage<irtkRealPixel> gaussianSourceX;
  gaussianSourceX.SetInput (&gaussianX);
  gaussianSourceX.SetOutput(&kernelX);
  gaussianSourceX.Run();

  
  // Do convolution
  irtkConvolutionWithPadding_1D<VoxelType> convolutionX(this->_PaddingValue);
  convolutionX.SetInput ( this->_input);
  convolutionX.SetInput2(&kernelX);
  convolutionX.SetOutput(this->_output);
  convolutionX.SetNormalization(true);
  convolutionX.irtkImageToImage<VoxelType>::Run();

  // Flip x and y axis of image
  this->_output->FlipXY(1);

  // Create scalar function which corresponds to a 1D Gaussian function in Y
  irtkScalarGaussian gaussianY(this->_Sigma/ysize, 1, 1, 0, 0, 0);

  // Create filter kernel for 1D Gaussian function in Y
  irtkGenericImage<irtkRealPixel> kernelY(2*round(4*this->_Sigma/ysize)+1, 1, 1);

  // Do conversion from  scalar function to filter kernel
  irtkScalarFunctionToImage<irtkRealPixel> gaussianSourceY;
  gaussianSourceY.SetInput (&gaussianY);
  gaussianSourceY.SetOutput(&kernelY);
  gaussianSourceY.Run();

  // Do convolution
  irtkConvolutionWithPadding_1D<VoxelType> convolutionY(this->_PaddingValue);
  convolutionY.SetInput (this->_output);
  convolutionY.SetInput2(&kernelY);
  convolutionY.SetOutput(this->_output);
  convolutionY.SetNormalization(true);
  convolutionY.irtkImageToImage<VoxelType>::Run();

  // Flip x and z axis of image
  this->_output->FlipXZ(1);

  if (this->_output->GetX() != 1) {
    // Create scalar function which corresponds to a 1D Gaussian function in Z
    irtkScalarGaussian gaussianZ(this->_Sigma/zsize, 1, 1, 0, 0, 0);

    // Create filter kernel for 1D Gaussian function in Z
    irtkGenericImage<irtkRealPixel> kernelZ(2*round(4*this->_Sigma/zsize)+1, 1, 1);

    // Do conversion from  scalar function to filter kernel
    irtkScalarFunctionToImage<irtkRealPixel> gaussianSourceZ;
    gaussianSourceZ.SetInput (&gaussianZ);
    gaussianSourceZ.SetOutput(&kernelZ);
    gaussianSourceZ.Run();

    // Do convolution
    irtkConvolutionWithPadding_1D<VoxelType> convolutionZ(this->_PaddingValue);
    convolutionZ.SetInput (this->_output);
    convolutionZ.SetInput2(&kernelZ);
    convolutionZ.SetOutput(this->_output);
    convolutionZ.SetNormalization(true);
    convolutionZ.irtkImageToImage<VoxelType>::Run();
  }

  // Flip image back, first x and z axis, then x and y axis
  this->_output->FlipXZ(1);
  this->_output->FlipXY(1);

  // Do the final cleaning up - don;t need it since no buffering required
  //this->Finalize();
}

template class EbbGaussianBlurringWithPadding<unsigned char>;
template class EbbGaussianBlurringWithPadding<short>;
template class EbbGaussianBlurringWithPadding<unsigned short>;
template class EbbGaussianBlurringWithPadding<float>;
template class EbbGaussianBlurringWithPadding<double>;
