Fast Volume Reconstruction from Motion Corrupted Stacks of 2D Slices

Section IV-A: estimate relative amount of motion per stack of images - this is needed for reconstruction estimate correct alignment between slices
 * the code provides an automated method of finding the stack that all other stacks are aligned to (called the template stack)
 * in the example dataset, it seems they use the first stack as the stack that all other stacks are aligned to
 * in Figure 2, this is translated into 1) motion measurement of the stacks that produce a single template such that 2) image registration is done to align all other stacks to it (i.e. 3D rigid volumetric registration between all stacks and template stack to account for global transformations of region of interest)
 * In code: StackRegistrations(),

Section IV-B: motion compensated transformation of scanned 2D slices into a 3D reconstruction volume (this is the final image that is returned)
 * this steps figures out a transform such that a pixel in a 2D slice is able to be mapped to a particular voxel in the 3D reconstruction volume
 * it also updates the voxel value in the 3D volume based on some distribution of pixel values in the original 2D slices
 * in Figure 2, this would be the PSF-based volume update and the Slice to reconstructed volume transformation T
 * In code: CreateSlicesandTransformations(), CoeffInit(), GaussianReconstruction(),

Section IV-C: Slice outlier removal and bias correction
 * this part removes outlier slices, outlier slices are slices that are highly corrupt and not useful in the reconstruction of the 3D volume
 * in Figure 2 this is the Simulate Slices and update robust statistics, EM-MOdel and Bias field and scale factors estimation sections.
 * In code: InitializeEMValues(), SimulateSlices(), InitializeRobustStatistics(), EStep()

Section IV-D: Super-resolution reconstruction?
 * want to minimize error between the slice pixels
 * in Figure 2 this is the super resolution volume update + edge preserving regularization
 * in code: MStep(), Superresolution()

Section IV-E: Slice to Volume registration
 * at this point the algorithm produces a candidate approximate reconstruction volume of interest
 * if the resulting volume has not converged, it will rerun the steps above
 * in code: SliceToVolumeRegistration()

