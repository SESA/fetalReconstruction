/*=========================================================================
Library   : Image Registration Toolkit (IRTK)
Module    : $Id: irtkReconstructionCuda.cc 1 2013-11-15 14:36:30 bkainz $
Copyright : Imperial College, Department of Computing
Visual Information Processing (VIP), 2011 onwards
Date      : $Date: 2013-11-15 14:36:30 +0100 (Fri, 15 Nov 2013) $
Version   : $Revision: 1 $
Changes   : $Author: bkainz $

Copyright (c) 2014, Bernhard Kainz, Markus Steinberger,
Maria Murgasova, Kevin Keraudren
All rights reserved.

If you use this work for research we would very much appreciate if you
Bernhard Kainz, Markus Steinberger, Wolfgang Wein, Maria Kuklisova-Murgasova, 
Christina Malamateniou, Kevin Keraudren, Thomas Torsney-Weir, Mary Rutherford, 
Paul Aljabar, Joseph V. Hajnal, and Daniel Rueckert: Fast Volume Reconstruction 
from Motion Corrupted Stacks of 2D Slices. IEEE Transactions on Medical Imaging, 
in print, 2015. doi:10.1109/TMI.2015.2415453 

IRTK IS PROVIDED UNDER THE TERMS OF THIS CREATIVE
COMMONS PUBLIC LICENSE ("CCPL" OR "LICENSE"). THE WORK IS PROTECTED BY
COPYRIGHT AND/OR OTHER APPLICABLE LAW. ANY USE OF THE WORK OTHER THAN
AS AUTHORIZED UNDER THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.

BY EXERCISING ANY RIGHTS TO THE WORK PROVIDED HERE, YOU ACCEPT AND AGREE
TO BE BOUND BY THE TERMS OF THIS LICENSE. TO THE EXTENT THIS LICENSE MAY BE
CONSIDERED TO BE A CONTRACT, THE LICENSOR GRANTS YOU THE RIGHTS CONTAINED
HERE IN CONSIDERATION OF YOUR ACCEPTANCE OF SUCH TERMS AND CONDITIONS.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=========================================================================*/

#ifndef GPUWORKER_CUDA_CUH
#define GPUWORKER_CUDA_CUH

#if !USE_BOOST
//use c++11 in std in case of VS 2012
#include <thread>
#include <condition_variable>
#else
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/container/vector.hpp>
namespace bc = boost::container;
#endif



class GPUWorkerSync
{
#if !USE_BOOST
  std::mutex mutexController,
    mutexWorker;
  std::condition_variable conditionController,
    conditionWorker;
#else
  boost::mutex mutexController,
    mutexWorker;
  boost::condition_variable conditionController,
    conditionWorker;
#endif

  int GPU_count;
  volatile int count;
public:
  GPUWorkerSync() : GPU_count(0), count(0) { }
  void completed();
  void runNextRound();
  void runNoSync();
  template<class FLaunchData>
  void startup(int toLaunch, void(*starter)(int, FLaunchData&), FLaunchData& data)
  {
#if !USE_BOOST
    std::unique_lock<std::mutex> lockController(mutexController);
    {
      std::unique_lock<std::mutex> lockworker(mutexWorker);
      GPU_count = toLaunch;
      count = 0;
      for (int i = 0; i < toLaunch; ++i)
      {
        starter(i, data);
      }
    }
#else
    boost::unique_lock<boost::mutex> lockController(mutexController);
    {
      boost::unique_lock<boost::mutex> lockworker(mutexWorker);
      GPU_count = toLaunch;
      count = 0;
      for (int i = 0; i < toLaunch; ++i)
      {
        starter(i, data);
      }
    }

#endif
    conditionController.wait(lockController);
  }

};


#endif //GPUWORKER_CUDA_CUH
