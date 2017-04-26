typedef struct unsigned_three
{
    unsigned int x, y, z;
} uint3;

struct timers {
  float coeffInit;
  float gaussianReconstruction;
  float simulateSlices;
  float initializeRobustStatistics;
  float eStepI;
  float eStepII;
  float eStepIII;
  float scale;
  float superResolution;
  float mStep;
  float restoreSliceIntensities;
  float scaleVolume;
  float sliceToVolumeRegistration;
  float totalExecutionTime;
};

inline void InitializeTimers(struct timers& t) {
  t.coeffInit = 0.0;
  t.gaussianReconstruction = 0.0;
  t.simulateSlices = 0.0;
  t.initializeRobustStatistics = 0.0;
  t.eStepI = 0.0;
  t.eStepII = 0.0;
  t.eStepIII = 0.0;
  t.scale = 0.0;
  t.superResolution = 0.0;
  t.mStep = 0.0;
  t.restoreSliceIntensities = 0.0;
  t.scaleVolume = 0.0;
  t.sliceToVolumeRegistration = 0.0;
  t.totalExecutionTime = 0.0;
}

inline void TimeReport(struct timers& t) {
  cout << endl;
  cout << endl;
  cout << "[time] CoeffInit " 
    << t.coeffInit << endl;
  cout << "[time] GaussianReconstruction " 
    << t.gaussianReconstruction << endl;
  cout << "[time] SimulateSlices " 
    << t.simulateSlices << endl;
  cout << "[time] InitializeRobustStatistics " 
    << t.initializeRobustStatistics << endl;
  cout << "[time] EStepI " 
    << t.eStepI << endl;
  cout << "[time] EStepII " 
    << t.eStepII << endl;
  cout << "[time] EStepIII " 
    << t.eStepIII << endl;
  cout << "[time] Scale " 
    << t.scale << endl;
  cout << "[time] SuperResolution " 
    << t.superResolution << endl;
  cout << "[time] MStep " 
    << t.mStep << endl;
  cout << "[time] RestoreSliceIntensities " 
    << t.restoreSliceIntensities << endl;
  cout << "[time] ScaleVolume " 
    << t.scaleVolume << endl;
  cout << "[time] SliceToVolumeRegistration " 
    << t.sliceToVolumeRegistration << endl;
  cout << endl;
}

inline struct timeval startTimer() {
  struct timeval start;
  gettimeofday(&start, NULL);
  return start;
}

inline float endTimer(struct timeval start) {
  struct timeval end;
  gettimeofday(&end, NULL);
  float seconds = (end.tv_sec - start.tv_sec) + 
    ((end.tv_usec - start.tv_usec) / 1000000.0);
  return seconds;
}
