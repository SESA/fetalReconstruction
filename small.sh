#!/bin/bash

set -x

if [ -z "${DEBUG}" ]
then
  DEBUG=0
else
  DEBUG=1
fi

if [ -n "${TEST}" ]
then
  TESTARGS=" --rec_iterations_first 1 --rec_iterations_last 1"
fi

./build/bin/reconstruction_GPU2 -o 3TStackReconstruction.nii -i data/masked_stack-1.nii data/masked_stack-2.nii data/masked_stack-3.nii data/masked_stack-4.nii --disableBiasCorrection --useAutoTemplate --useSINCPSF --resolution 2.0 --debug ${DEBUG} --numThreads $1 --useCPU --iterations $2 ${TESTARGS}
