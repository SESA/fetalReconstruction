#!/bin/sh

./reconstruction_GPU2 -o 3TReconstruction.nii.gz -i data/masked_stack-1.nii.gz data/masked_stack-2.nii.gz data/masked_stack-3.nii.gz data/masked_stack-4.nii.gz --disableBiasCorrection --useAutoTemplate --useSINCPSF --resolution 1.0 --useCPU

