#!/bin/sh

set -x

export EBBRT_NODE_ALLOCATOR_DEFAULT_CPUS=$1
export EBBRT_NODE_ALLOCATOR_DEFAULT_RAM=64
export EBBRT_NODE_ALLOCATOR_DEFAULT_NUMANODES=1

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
./build/bin/reconstruction_GPU2 -o 3TReconstruction.nii -i data/14_3T_nody_001.nii data/10_3T_nody_001.nii data/21_3T_nody_001.nii data/23_3T_nody_001.nii -m data/mask_10_3T_brain_smooth.nii --disableBiasCorrection --useAutoTemplate --useSINCPSF --resolution 1.0 --debug ${DEBUG} --numThreads $1 --useCPU --iterations $2 ${TESTARGS} | grep -E "\[" > tmp
