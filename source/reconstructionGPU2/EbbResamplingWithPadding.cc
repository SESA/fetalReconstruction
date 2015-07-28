#include "EbbResamplingWithPadding.h"

template <class VoxelType>
EbbResamplingWithPadding<VoxelType>::EbbResamplingWithPadding(double new_xsize, double new_ysize, double new_zsize, VoxelType PaddingValue)
{
    _PaddingValue = PaddingValue;
    _XSize = new_xsize;
    _YSize = new_ysize;
    _ZSize = new_zsize;
  
}

template <class VoxelType>
bool EbbResamplingWithPadding<VoxelType>::RequiresBuffering(void)
{
    return true;
}

template <class VoxelType>
const char *EbbResamplingWithPadding<VoxelType>::NameOfClass()
{
    return "EbbResamplingWithPadding";
}

template <class VoxelType>
void EbbResamplingWithPadding<VoxelType>::Initialize()
{
    int new_x, new_y, new_z;
    double xaxis[3], yaxis[3], zaxis[3];
    double new_xsize, new_ysize, new_zsize;
    double old_xsize, old_ysize, old_zsize;

    // Do the initial set up
    this->irtkImageToImage<VoxelType>::Initialize();

    // Determine the old dimensions of the image
    this->_input->GetPixelSize(&old_xsize, &old_ysize, &old_zsize);

    // Determine the new dimensions of the image
    new_x = round(this->_input->GetX() * old_xsize / this->_XSize);
    new_y = round(this->_input->GetY() * old_ysize / this->_YSize);
    new_z = round(this->_input->GetZ() * old_zsize / this->_ZSize);

    // Determine the new voxel dimensions
    if (new_x < 1) {
	new_x     =  1;
	new_xsize =  old_xsize;
    } else {
	new_xsize = this->_XSize;
    }
    if (new_y < 1) {
	new_y     =  1;
	new_ysize =  old_ysize;
    } else {
	new_ysize = this->_YSize;
    }
    if (new_z < 1) {
	new_z     =  1;
	new_zsize =  old_zsize;
    } else {
	new_zsize = this->_ZSize;
    }

    // Allocate new image
    *this->_output = irtkGenericImage<VoxelType>(new_x, new_y, new_z, this->_input->GetT());

    // Set new voxel size
    this->_output->PutPixelSize(new_xsize, new_ysize, new_zsize);

    // Set new orientation
    this->_input ->GetOrientation(xaxis, yaxis, zaxis);
    this->_output->PutOrientation(xaxis, yaxis, zaxis);

    // Set new origin
    this->_output->PutOrigin(this->_input->GetOrigin());
}

template <class VoxelType> void EbbResamplingWithPadding<VoxelType>::Run()
{
    int i, j, k, l, u, v, w, pad;
    double val, sum;
    double w1, w2, w3, w4, w5, w6, w7, w8, dx, dy, dz, x, y, z;

    // Do the initial set up
    this->Initialize();

    for (l = 0; l < this->_output->GetT(); l++) {
	for (k = 0; k < this->_output->GetZ(); k++) {
	    for (j = 0; j < this->_output->GetY(); j++) {
		for (i = 0; i < this->_output->GetX(); i++) {
		    x = i;
		    y = j;
		    z = k;
		    this->_output->ImageToWorld(x, y, z);
		    this->_input ->WorldToImage(x, y, z);
	  
		    // Calculate integer fraction of points
		    u = (int)floor(x);
		    v = (int)floor(y);
		    w = (int)floor(z);

		    // Calculate floating point fraction of points
		    dx = x - u;
		    dy = y - v;
		    dz = z - w;

		    // Calculate weights for trilinear interpolation
		    w1 = (1 - dx) * (1 - dy) * (1 - dz);
		    w2 = (1 - dx) * (1 - dy) * dz;
		    w3 = (1 - dx) * dy * (1 - dz);
		    w4 = (1 - dx) * dy * dz;
		    w5 = dx * (1 - dy) * (1 - dz);
		    w6 = dx * (1 - dy) * dz;
		    w7 = dx * dy * (1 - dz);
		    w8 = dx * dy * dz;

		    // Calculate trilinear interpolation, ignoring padded values
		    val = 0;
		    pad = 8;
		    sum = 0;
		    if ((u >= 0) && (u < this->_input->GetX()) &&
			(v >= 0) && (v < this->_input->GetY()) &&
			(w >= 0) && (w < this->_input->GetZ())) {
			if (this->_input->Get(u, v, w, l) != this->_PaddingValue) {
			    pad--;
			    val += this->_input->Get(u, v, w, l) * w1;
			    sum += w1;
			}
		    } else {
			pad--;
		    }
		    if ((u >= 0)   && (u < this->_input->GetX()) &&
			(v >= 0)   && (v < this->_input->GetY()) &&
			(w+1 >= 0) && (w+1 < this->_input->GetZ())) {
			if (this->_input->Get(u, v, w+1, l) != this->_PaddingValue) {
			    pad--;
			    val += this->_input->Get(u, v, w+1, l) * w2;
			    sum += w2;
			}
		    } else {
			pad--;
		    }
		    if ((u >= 0)   && (u < this->_input->GetX()) &&
			(v+1 >= 0) && (v+1 < this->_input->GetY()) &&
			(w >= 0)   && (w < this->_input->GetZ())) {
			if (this->_input->Get(u, v+1, w, l) != this->_PaddingValue) {
			    pad--;
			    val += this->_input->Get(u, v+1, w, l) * w3;
			    sum += w3;
			}
		    } else {
			pad--;
		    }
		    if ((u >= 0)   && (u < this->_input->GetX()) &&
			(v+1 >= 0) && (v+1 < this->_input->GetY()) &&
			(w+1 >= 0) && (w+1 < this->_input->GetZ())) {
			if (this->_input->Get(u, v+1, w+1, l) != this->_PaddingValue) {
			    pad--;
			    val += this->_input->Get(u, v+1, w+1, l) * w4;
			    sum += w4;
			}
		    } else {
			pad--;
		    }
		    if ((u+1 >= 0) && (u+1 < this->_input->GetX()) &&
			(v >= 0)   && (v < this->_input->GetY()) &&
			(w >= 0)   && (w < this->_input->GetZ())) {
			if (this->_input->Get(u+1, v, w, l) != this->_PaddingValue) {
			    pad--;
			    val += this->_input->Get(u+1, v, w, l) * w5;
			    sum += w5;
			}
		    } else {
			pad--;
		    }
		    if ((u+1 >= 0) && (u+1 < this->_input->GetX()) &&
			(v >= 0)   && (v < this->_input->GetY()) &&
			(w+1 >= 0) && (w+1 < this->_input->GetZ())) {
			if (this->_input->Get(u+1, v, w+1, l) != this->_PaddingValue) {
			    pad--;
			    val += this->_input->Get(u+1, v, w+1, l) * w6;
			    sum += w6;
			}
		    } else {
			pad--;
		    }
		    if ((u+1 >= 0) && (u+1 < this->_input->GetX()) &&
			(v+1 >= 0) && (v+1 < this->_input->GetY()) &&
			(w >= 0)   && (w < this->_input->GetZ())) {
			if (this->_input->Get(u+1, v+1, w, l) != this->_PaddingValue) {
			    pad--;
			    val += this->_input->Get(u+1, v+1, w, l) * w7;
			    sum += w7;
			}
		    } else {
			pad--;
		    }
		    if ((u+1 >= 0) && (u+1 < this->_input->GetX()) &&
			(v+1 >= 0) && (v+1 < this->_input->GetY()) &&
			(w+1 >= 0) && (w+1 < this->_input->GetZ())) {
			if (this->_input->Get(u+1, v+1, w+1, l) != this->_PaddingValue) {
			    pad--;
			    val += this->_input->Get(u+1, v+1, w+1, l) * w8;
			    sum += w8;
			}
		    } else {
			pad--;
		    }
		    if (pad < 4) {
			if (sum > 0) {
			    this->_output->PutAsDouble(i, j, k, l, val / sum);
			} else {
			    // Special case: Point lies on edge of padded voxels.
			    this->_output->Put(i, j, k, l, this->_PaddingValue);
			    // Rather than:
			    // //cerr << this->NameOfClass();
			    // //cerr << "::Run: This should never happen" << endl;
			    // exit(1);
			}
		    } else {
			this->_output->Put(i, j, k, l, this->_PaddingValue);
		    }
		}
	    }
	}
    }

    // Do the final cleaning up
    this->Finalize();
}

template class EbbResamplingWithPadding<char>;
template class EbbResamplingWithPadding<unsigned char>;
template class EbbResamplingWithPadding<short>;
template class EbbResamplingWithPadding<unsigned short>;
template class EbbResamplingWithPadding<int>;
template class EbbResamplingWithPadding<float>;
template class EbbResamplingWithPadding<double>;
