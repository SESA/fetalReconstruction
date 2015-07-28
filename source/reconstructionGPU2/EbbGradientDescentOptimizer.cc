#include "EbbGradientDescentOptimizer.h"

double EbbGradientDescentOptimizer::Run()
{
    int i, n;
    double similarity, new_similarity, old_similarity;

    // Assume that the transformation is the optimal transformation
    old_similarity = new_similarity = similarity = _Registration->Evaluate();

    // Number of variables we have to optimize
    n = _Transformation->NumberOfDOFs();

    // Convert some stuff to NR
    float *dx = new float[n];

    _Registration->EvaluateGradient(_StepSize, dx);
  
    // Step along gradient direction until no further improvement is necessary
    do
    {
	new_similarity = similarity;
	for (i = 0; i < _Transformation->NumberOfDOFs(); i++)
	{
	    _Transformation->Put(i, _Transformation->Get(i) + _StepSize * dx[i]);
	}
	similarity = _Registration->Evaluate();
	//if (similarity > new_similarity + _Epsilon) cout << similarity << endl;
    } while (similarity > new_similarity + _Epsilon);
    
    // Last step was no improvement, so back track
    for (i = 0; i < _Transformation->NumberOfDOFs(); i++)
    {
	_Transformation->Put(i, _Transformation->Get(i) - _StepSize * dx[i]);
    }
    
    // Delete NR memory
    delete []dx;

    if (new_similarity > old_similarity)
    {
	return new_similarity - old_similarity;
    }
    else
    {
	return 0;
    }
}

void EbbGradientDescentOptimizer::Run(double &epsilon, double &maxChange)
{
    int i, n;
    double diff;
    n = _Transformation->NumberOfDOFs();

    // Store the current state of the transformation.
    for (i = 0; i < n; ++i)
    {
	transformationParams[i] = _Transformation->Get(i);
    }

    // Epsilon is the change in similarity over an iteration of the
    // optimiser.
    epsilon = this->Run();

    // MaxChange is the maximum change over the transformation parameters.
    maxChange = 0;
    for (i = 0; i < n; ++i)
    {
	diff = fabs(_Transformation->Get(i) - transformationParams[i]);

	if (maxChange < diff)
	{
	    maxChange = diff;
	}
    }
}
