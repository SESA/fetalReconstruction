#include "EbbImageRigidRegistrationWithPadding.h"

double EbbImageRigidRegistrationWithPadding::Evaluate()
{
    int i, j, k, t;

    // Pointer to reference data
    irtkGreyPixel *ptr2target;
    
    // Create iterator
    irtkHomogeneousTransformationIterator iterator((irtkHomogeneousTransformation *)_transformation);
    
    // Initialize metric
    _metric->Reset();
    
    // Pointer to voxels in target image
    ptr2target = _target->GetPointerToVoxels();

    for (t = 0; t < _target->GetT(); t++)
    {
	// Initialize iterator
	iterator.Initialize(_target, _source);

	// Loop over all voxels in the target (reference) volume
	for (k = 0; k < _target->GetZ(); k++)
	{
	    for (j = 0; j < _target->GetY(); j++)
	    {
		for (i = 0; i < _target->GetX(); i++)
		{
		    
		    // Check whether reference point is valid
		    if (*ptr2target >= 0)
		    {
			// Check whether transformed point is inside source volume
			if ((iterator._x > _source_x1) && (iterator._x < _source_x2) &&
			    (iterator._y > _source_y1) && (iterator._y < _source_y2) &&
			    (iterator._z > _source_z1) && (iterator._z < _source_z2))
			{
			    // Add sample to metric. Note: only linear interpolation supported at present
			    //double value = (static_cast<irtkLinearInterpolateImageFunction*> (_interpolator))->EvaluateWithPadding(-1,iterator._x, iterator._y, iterator._z, t);
			    double value = _interpolator->EvaluateInside(iterator._x, iterator._y, iterator._z, t);
			    if (value >= 0)
				_metric->Add(*ptr2target, round(value));
			}
			iterator.NextX();
		    }
		    else
		    {
			// Advance iterator by offset
			iterator.NextX(*ptr2target * -1);
			i          -= (*ptr2target) + 1;
			ptr2target -= (*ptr2target) + 1;
		    }
		    ptr2target++;
		}
		iterator.NextY();
	    }
	    iterator.NextZ();
	}
    }
    
    // Evaluate similarity measure
    return _metric->Evaluate();
}

void EbbImageRigidRegistrationWithPadding::SetOutput(irtkRigidTransformation *transformation)
{
    _transformation = transformation;
}

void EbbImageRigidRegistrationWithPadding::SetInput(irtkGreyImage *target, irtkGreyImage *source)
{
    _target = target;
    _source = source;
}

void EbbImageRigidRegistrationWithPadding:: GuessParameterThickSlices()
{
    int i;
    double xsize, ysize, zsize,size;

    if ((_target == NULL) || (_source == NULL))
    {
	//ebbrt::kprintf("EbbImageRigidRegistrationWithPadding::GuessParameter: Target and source image not found\n");
	cout << "EbbImageRigidRegistrationWithPadding::GuessParameter: Target and source image not found\n" << endl;
	exit(1);
    }

    // Default parameters for registration
    _NumberOfLevels     = 3;
    _NumberOfBins       = 64;

    // Default parameters for optimization
    _SimilarityMeasure  = CC;
    _OptimizationMethod = GradientDescent;
    _Epsilon            = 0.0001;

    // Read target pixel size
    _target->GetPixelSize(&xsize, &ysize, &zsize);
  
    if (ysize<xsize)
    {
	size = ysize;
    }
    else
    {
	size = xsize;
    }
    
    // Default target parameters
    _TargetBlurring[0]      = size / 2.0;
    _TargetResolution[0][0] = size;
    _TargetResolution[0][1] = size;
    _TargetResolution[0][2] = zsize;

    for (i = 1; i < _NumberOfLevels; i++) {
	_TargetBlurring[i]      = _TargetBlurring[i-1] * 2;
	_TargetResolution[i][0] = _TargetResolution[i-1][0] * 2;
	_TargetResolution[i][1] = _TargetResolution[i-1][1] * 2;
	_TargetResolution[i][2] = _TargetResolution[i-1][2]; //* 2;
    }

    // Read source pixel size
    _source->GetPixelSize(&xsize, &ysize, &zsize);
  
    if (ysize<xsize)
    {
	size = ysize;
    }
    else
    {
	size = xsize;
    }
    
    // Default source parameters
    _SourceBlurring[0]      = size / 2.0;
    _SourceResolution[0][0] = size;
    _SourceResolution[0][1] = size;
    _SourceResolution[0][2] = zsize;

    for (i = 1; i < _NumberOfLevels; i++) {
	_SourceBlurring[i]      = _SourceBlurring[i-1] * 2;
	_SourceResolution[i][0] = _SourceResolution[i-1][0] * 2;
	_SourceResolution[i][1] = _SourceResolution[i-1][1] * 2;
	_SourceResolution[i][2] = _SourceResolution[i-1][2]; //* 2;
    }

    // Remaining parameters
    for (i = 0; i < _NumberOfLevels; i++) {
	_NumberOfIterations[i] = 20;
	_NumberOfSteps[i]      = 4;
	_LengthOfSteps[i]      = 2 * pow(2.0, i);
    }

    // Try to guess padding by looking at voxel values in all eight corners of the volume:
    // If all values are the same we assume that they correspond to the padding value
    _TargetPadding = MIN_GREY;
    if ((_target->Get(_target->GetX()-1, 0, 0) == _target->Get(0, 0, 0)) &&
	(_target->Get(0, _target->GetY()-1, 0)                                 == _target->Get(0, 0, 0)) &&
	(_target->Get(0, 0, _target->GetZ()-1)                                 == _target->Get(0, 0, 0)) &&
	(_target->Get(_target->GetX()-1, _target->GetY()-1, 0)                 == _target->Get(0, 0, 0)) &&
	(_target->Get(0, _target->GetY()-1, _target->GetZ()-1)                 == _target->Get(0, 0, 0)) &&
	(_target->Get(_target->GetX()-1, 0, _target->GetZ()-1)                 == _target->Get(0, 0, 0)) &&
	(_target->Get(_target->GetX()-1, _target->GetY()-1, _target->GetZ()-1) == _target->Get(0, 0, 0))) {
	_TargetPadding = _target->Get(0, 0, 0);
    }

    _SourcePadding = MIN_GREY;
    if ((_source->Get(_source->GetX()-1, 0, 0)                                 == _source->Get(0, 0, 0)) &&
	(_source->Get(0, _source->GetY()-1, 0)                                 == _source->Get(0, 0, 0)) &&
	(_source->Get(0, 0, _source->GetZ()-1)                                 == _source->Get(0, 0, 0)) &&
	(_source->Get(_source->GetX()-1, _source->GetY()-1, 0)                 == _source->Get(0, 0, 0)) &&
	(_source->Get(0, _source->GetY()-1, _source->GetZ()-1)                 == _source->Get(0, 0, 0)) &&
	(_source->Get(_source->GetX()-1, 0, _source->GetZ()-1)                 == _source->Get(0, 0, 0)) &&
	(_source->Get(_source->GetX()-1, _source->GetY()-1, _source->GetZ()-1) == _source->Get(0, 0, 0))) {
	_SourcePadding = _source->Get(0, 0, 0);
    }
}

void EbbImageRigidRegistrationWithPadding::Initialize()
{
    // Check that the t-dimensions are the same for both images.
    // Do not currently support different t-dimensions.
    if (_target->GetT() != _source->GetT())
    {
	//ebbrt::kprintf("Initialize() : Images have different t-dimensions.");
	cout << "Initialize() : Images have different t-dimensions." << endl;
	exit(1);
    }
}

void EbbImageRigidRegistrationWithPadding::Initialize(int level)
{
    int i, j, k, t;
    double dx, dy, dz, temp;
    irtkGreyPixel target_min, target_max;//, target_nbins;
    irtkGreyPixel source_min, source_max;//, source_nbins;
    
    // Copy source and target to temp space
    tmp_target = new irtkGreyImage(*_target);
    tmp_source = new irtkGreyImage(*_source);

    // Swap source and target with temp space copies
    swap(tmp_target, _target);
    swap(tmp_source, _source);

    // Blur images if necessary
    if (_TargetBlurring[level] > 0) {
	irtkGaussianBlurringWithPadding<irtkGreyPixel> blurring(_TargetBlurring[level], _TargetPadding);
	blurring.SetInput (_target);
	blurring.SetOutput(_target);
	blurring.Run();
    }

    if (_SourceBlurring[level] > 0) {
	irtkGaussianBlurring<irtkGreyPixel> blurring(_SourceBlurring[level]);
	blurring.SetInput (_source);
	blurring.SetOutput(_source);
	blurring.Run();
    }
    
    _target->GetPixelSize(&dx, &dy, &dz);
    temp = fabs(_TargetResolution[0][0]-dx) + fabs(_TargetResolution[0][1]-dy) + fabs(_TargetResolution[0][2]-dz);
    
    if (level > 0 || temp > 0.000001) {
	EbbResamplingWithPadding<irtkGreyPixel> resample(_TargetResolution[level][0],
							 _TargetResolution[level][1],
							 _TargetResolution[level][2],
							  _TargetPadding);
	resample.SetInput (_target);
	resample.SetOutput(_target);
	resample.Run();
    }
    
    // Find out the min and max values in target image, ignoring padding
    target_max = MIN_GREY;
    target_min = MAX_GREY;
    for (t = 0; t < _target->GetT(); t++) {
	for (k = 0; k < _target->GetZ(); k++) {
	    for (j = 0; j < _target->GetY(); j++) {
		for (i = 0; i < _target->GetX(); i++) {
		    if (_target->Get(i, j, k, t) > _TargetPadding) {
			if (_target->Get(i, j, k, t) > target_max)
			    target_max = _target->Get(i, j, k, t);
			if (_target->Get(i, j, k, t) < target_min)
			    target_min = _target->Get(i, j, k, t);
		    } else {
			_target->Put(i, j, k, t, _TargetPadding);
		    }
		}
	    }
	}
    }

    // Find out the min and max values in source image, ignoring padding
    source_max = MIN_GREY;
    source_min = MAX_GREY;
    for (t = 0; t < _source->GetT(); t++) {
	for (k = 0; k < _source->GetZ(); k++) {
	    for (j = 0; j < _source->GetY(); j++) {
		for (i = 0; i < _source->GetX(); i++) {
		    if (_source->Get(i, j, k, t) > source_max)
			source_max = _source->Get(i, j, k, t);
		    if (_source->Get(i, j, k, t) < source_min)
			source_min = _source->Get(i, j, k, t);
		}
	    }
	}
    }

    // Check whether dynamic range of data is not to large
    if (target_max - target_min > MAX_GREY) {
	//cerr << this->NameOfClass()
	//   << "::Initialize: Dynamic range of target is too large" << endl;
	exit(1);
    } else {
	for (t = 0; t < _target->GetT(); t++) {
	    for (k = 0; k < _target->GetZ(); k++) {
		for (j = 0; j < _target->GetY(); j++) {
		    for (i = 0; i < _target->GetX(); i++) {
			if (_target->Get(i, j, k, t) > _TargetPadding) {
			    _target->Put(i, j, k, t, _target->Get(i, j, k, t) - target_min);
			} else {
			    _target->Put(i, j, k, t, -1);
			}
		    }
		}
	    }
	}
    }
    
    if ((_SimilarityMeasure == SSD) || (_SimilarityMeasure == CC) ||
	(_SimilarityMeasure == LC)  || (_SimilarityMeasure == K) || (_SimilarityMeasure == ML)) {
	if (source_max - target_min > MAX_GREY) {
	    //cerr << this->NameOfClass()
	    //<< "::Initialize: Dynamic range of source is too large" << endl;
	    exit(1);
	} else {
	    for (t = 0; t < _source->GetT(); t++) {
		for (k = 0; k < _source->GetZ(); k++) {
		    for (j = 0; j < _source->GetY(); j++) {
			for (i = 0; i < _source->GetX(); i++) {
			    _source->Put(i, j, k, t, _source->Get(i, j, k, t) - target_min);
			}
		    }
		}
	    }
	}
    } else {
	if (source_max - source_min > MAX_GREY) {
	    //cerr << this->NameOfClass()
	    //<< "::Initialize: Dynamic range of source is too large" << endl;
	    exit(1);
	} else {
	    for (t = 0; t < _source->GetT(); t++) {
		for (k = 0; k < _source->GetZ(); k++) {
		    for (j = 0; j < _source->GetY(); j++) {
			for (i = 0; i < _source->GetX(); i++) {
			    _source->Put(i, j, k, t, _source->Get(i, j, k, t) - source_min);
			}
		    }
		}
	    }
	}
    }
    
    
    // Pad target image if necessary
//    EbbPadding(*_target, _TargetPadding);
    irtkPadding(*_target, _TargetPadding);

    // DEFAULT - Rescale images by an integer factor if necessary
    _metric = new EbbCrossCorrelationSimilarityMetric;

    _interpolator = irtkInterpolateImageFunction::New(Interpolation_Linear, _source);
    _interpolator->SetInput(_source);
    _interpolator->Initialize();
    
    _interpolator->Inside(_source_x1, _source_y1, _source_z1,
			  _source_x2, _source_y2, _source_z2);
    
    _optimizer = new EbbGradientDescentOptimizer;
    _optimizer->SetTransformation(_transformation);
    _optimizer->SetRegistration(this);
}

void EbbImageRigidRegistrationWithPadding::Run()
{
    int level, i, j;
    //char buffer[256];
    double step, epsilon = 0, delta, maxChange = 0;
    
    if (_source == NULL) {
	//ebbrt::kprintf("Registration::Run: Filter has no source input");
	cout << "Initialize() : Images have different t-dimensions." << endl;
	exit(1);
    }
    
    if (_target == NULL) {
	//ebbrt::kprintf("Registration::Run: Filter has no target input");
	cout << "Registration::Run: Filter has no target input" << endl;
	exit(1);
    }
    
    if (_transformation == NULL) {
	//ebbrt::kprintf("Registration::Run: Filter has no transformation output");
	cout << "Registration::Run: Filter has no transformation output" << endl;
	exit(1);
    }

    // Do the initial set up for all levels
    this->Initialize();

    // Loop over levels
    for (level = _NumberOfLevels-1; level >= 0; level--)
    {
	// Initial step size
	step = _LengthOfSteps[level];

	// Initial Delta
	delta = _Delta[level];

	// Initialize for this level
	this->Initialize(level);
	
	// Run the registration filter at this resolution
	for (i = 0; i < _NumberOfSteps[level]; i++)
	{
	    for (j = 0; j < _NumberOfIterations[level]; j++)
	    {
		// Optimize at lowest level of resolution
		_optimizer->SetStepSize(step);
		_optimizer->SetEpsilon(_Epsilon);
		_optimizer->Run(epsilon, maxChange);
	    }
	    step = step / 2;
	    delta = delta / 2.0;
	}
	
	// Do the final cleaning up for this level
	this->Finalize(level);
    }

    // Do the final cleaning up for all levels
    this->Finalize();
}

double EbbImageRigidRegistrationWithPadding::EvaluateGradient(float step, float *dx)
{
    int i;
    double s1, s2, norm, parameterValue;

    for (i = 0; i < _transformation->NumberOfDOFs(); i++)
    {
	if (_transformation->irtkTransformation::GetStatus(i) == _Active)
	{
	    parameterValue = _transformation->Get(i);
	    _transformation->Put(i, parameterValue + step);
	    s1 = this->Evaluate();
	    _transformation->Put(i, parameterValue - step);
	    s2 = this->Evaluate();
	    _transformation->Put(i, parameterValue);
	    dx[i] = s1 - s2;
	}
	else
	{
	    dx[i] = 0;
	}
    }
  
    // Calculate norm of vector
    norm = 0;
    for (i = 0; i < _transformation->NumberOfDOFs(); i++)
    {
	norm += dx[i] * dx[i];
    }

    // Normalize vector
    norm = sqrt(norm);
    if (norm > 0)
    {
	for (i = 0; i < _transformation->NumberOfDOFs(); i++)
	{
	    dx[i] /= norm;
	}
    }
    else
    {
	for (i = 0; i < _transformation->NumberOfDOFs(); i++)
	{
	    dx[i] = 0;
	}
    }

    return norm;
}


void EbbImageRigidRegistrationWithPadding::Finalize()
{}

void EbbImageRigidRegistrationWithPadding::Finalize(int level)
{
    // Swap source and target back with temp space copies (see Initialize)
    swap(tmp_target, _target);
    swap(tmp_source, _source);

    delete tmp_target;
    delete tmp_source;
    delete _metric;
    delete _optimizer;
    delete _interpolator;
}
