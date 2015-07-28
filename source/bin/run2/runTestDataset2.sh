#!/bin/sh

./reconstruction_GPU2 -o 3TReconstruction2.nii.gz -i data2/14_3T_nody_001.nii.gz data2/10_3T_nody_001.nii.gz data2/21_3T_nody_001.nii.gz data2/23_3T_nody_001.nii.gz -m data2/mask_10_3T_brain_smooth.nii.gz --disableBiasCorrection --useAutoTemplate --useSINCPSF --resolution 1.0 --useCPU

